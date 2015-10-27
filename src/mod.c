
#define MOD_C
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <mips32.h>
#include <mod.h>
#include <ioutils.h>
#include <debug.h>
#undef MOD_C

void configure_MIPS(MIPS_t *mips, config_file *conf, size_t buf) {

    disas_t q;
    q.bubble = true;
    q.addr = 0x0;
    q.inst = 0x0;

    memset(mips, 0, sizeof(MIPS_t));
    
    debug("Initializing MIPS model. Memory size: %zu\n", buf);

    verify_config(conf);
    if (0 == buf)
        errorf("ERROR: init_MIPS - zero buffer size\n");
    
    for (int i = T0; i < TEND; i++) {
        mips->regs[i] = conf->t_regs[i - T0];
    }

    if (NULL == (mips->mem = malloc(mips->mem_sz = buf)))
        errorf("ERROR: init_MIPS - unable to allocate memory buffer\nReason: %s", strerror(errno));

    mips->regs[SP] = MIPS_RESERVE + mips->mem_sz;
    
    mips->if_id.dis = q;
    mips->id_ex.dis = q;
    mips->ex_mem.dis = q;
    mips->mem_wb.dis = q;
}

void free_MIPS(MIPS_t *mips) {
    if (NULL == mips) return;
    if (NULL != mips->mem) free(mips->mem);
}

void report_status(MIPS_t *mips) {
    printf("Executed %zu instruction(s).\n"
           "%zu cycle(s) elapsed.\n"
           "pc = 0x%x\n",

            mips->inst_cnt,
            mips->cycle_cnt,

            mips->pc);
    
    for (const b6 *p = &report_regs[0]; *p != ZERO; p++)
        printf("%s = 0x%x\n", reg_names[*p], mips->regs[*p]);
}

void read_MIPS(MIPS_t *mips, const char *fname) {

    switch (elf_dump(fname, &(mips->pc), &(*mips->mem)[0], mips->mem_sz)) {
        case ELF_ERROR_IO_ERROR        :
            errorf("ERROR: read_MIPS - problems reading file `%s'.\n", fname);
        case ELF_ERROR_OUT_OF_MEM      :
            errorf("ERROR: read_MIPS - ELF file larger than allocated memory buffer.\n");
        case ELF_ERROR_NOT_ELF         :
            errorf("ERROR: read_MIPS - not an ELF file.\n");
        case ELF_ERROR_NOT_SUPPORTED   :
            errorf("ERROR: read_MIPS - unsupported ELF features detected.\n");
    }

}
