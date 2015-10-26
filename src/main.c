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
    
    configure_MIPS(&mips, &conf, 640*KB);

    dis_MIPS(&mips);

    memset(&(*mips.mem)[0], 0, mips.mem_sz);

    read_MIPS(&mips, argv[2]);    

    switch (interpret(&mips)) {
        case CONTINUE:
        case INTERNAL_ERROR:
            errorf("ERROR: interpret - Internal error in simulation\n");

        case UNIMPLEMENTED:
            errorf("ERROR: unimplemented feature.\n");

        case OVERFLOW:
            printf("Overflow in arithmetic operation.\n");
            dis_MIPS(&mips);
            break;

        case DELAY_SLOT_JUMP:
            printf("Delay slot jump.\n");
            dis_MIPS(&mips);
            break;

        case ACCESS_VIOLATION:
            printf("Access violation.\n");
            dis_MIPS(&mips);
            break;

        case UNALIGNED_ACCESS:
            printf("Unaligned access.\n");
            dis_MIPS(&mips);
            break;

        case BAD_INSTRUCTION:
            printf("Bad instruction.\n");
            dis_MIPS(&mips);
            break;

        case SYSCALL_HIT:
            break;

    }

    report_status(&mips);

    free_MIPS(&mips);
}
