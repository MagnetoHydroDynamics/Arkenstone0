#define MAIN_C
#include <stdlib.h>
#include <string.h>

#include <utils.h>
#include <mod.h>
#include <ioutils.h>
#include <debug.h>
#include <sim.h>
#include <elf.h>
#include <mips32.h>
#undef MAIN_C

int main(int argc, char *argv[]) {
    
    config_file conf;
    MIPS_t mips;

    set_debugf(stderr);

    if (3 != argc) usage(argv[0]);
    
    clear_config_file(&conf);
    read_config_file(argv[1], &conf);
    
    clear_MIPS(&mips);
    configure_MIPS(&mips, &conf, 640*KB);
    memset(&(*mips.mem)[0], 0, mips.mem_sz);
    
    read_MIPS(&mips, argv[2]);    

    report_status(&mips);

    free_MIPS(&mips);
}
