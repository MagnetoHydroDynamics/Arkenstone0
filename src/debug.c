
#define DEBUG_C
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <ioutils.h>
#include <mod.h>
#include <mips32.h>
#undef DEBUG_c

void set_debug(const char *fname) {
    FILE *f;
    if (NULL == (f = fopen(fname, "a")))
        errorf("ERROR: set_debug - Unable to open debug log file `%s'\nReason: %s\n",
                fname, strerror(errno)); 
    set_debugf(f);
}

void set_debugf(FILE *file) {
    debug_stream = file;
    if (NULL != debug_stream)
    if (0 > fprintf(debug_stream, " --- DEBUG ON\n"))
        errorf("ERROR: set_debugf - Unable to write to debug stream.");
}

void debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (NULL != debug_stream) {
        if (0 > fprintf(debug_stream, " --- ")) {
            errno = 0;
            return;
        }
        if (0 > vfprintf(debug_stream, fmt, args)) {
            errno = 0; // ignore all errors
            return;
        }
    }
}

void disassemble(disas_t *dis, const char *phase) {
        b32 pc = dis->addr;
        b32 inst = dis->inst;

        if (dis->bubble) {
            debug("%s = bubble\n", phase);
            return;
        }

// debug(fmt, pc, mne, rd, rs, rt, shamt, imm, addr, addr + imm);
//             1    2   3  4    5     6     7     8      9

#define PRINT \
    do { debug(dfmt, \
            pc, \
            mne, \
            reg_names[GET_RD(inst)], \
            reg_names[GET_RS(inst)], \
            reg_names[GET_RT(inst)], \
            GET_SHAMT(inst), \
            SIGN_EXTEND(GET_IMM(inst)), \
            GET_ADDRESS(inst) * 4, \
            (GET_ADDRESS(inst) + SIGN_EXTEND(GET_IMM(inst))) * 4, \
            phase); \
        } while (0)

#define RFMT  "%10$s = %1$08x:  %2$s $%3$s, $%4$s, $%5$s\n"
#define RsFMT "%10$s = %1$08x:  %2$s $%3$s, $%4$s, $%6$u\n"
#define RjFMT "%10$s = %1$08x:  %2$s $%3$s\n"
#define R_FMT "%10$s = %1$08x:  %2$s\n"
#define JFMT  "%10$s = %1$08x:  %2$s %8$08x\n"
#define IxFMT "%10$s = %1$08x:  %2$s $%5$s, $%4$s, %7$x\n"
#define IuFMT "%10$s = %1$08x:  %2$s $%5$s, $%4$s, %7$u\n"
#define IdFMT "%10$s = %1$08x:  %2$s $%5$s, $%4$s, %7$d\n"
#define IbFMT "%10$s = %1$08x:  %2$s $%5$s, $%4$s, %9$08x\n"
#define ImFMT "%10$s = %1$08x:  %2$s $%5$s, %7$d($%4$s)\n"

    const char *mne = NULL;
    const char *dfmt = NULL;
#define MNE(s) do { if (NULL == mne) mne = #s; } while (0)
#define FMT(f) do { if (NULL == dfmt) dfmt = f; } while (0)
     
    switch (GET_OPCODE(inst)) {
        case OPCODE_R        :

            switch (GET_FUNCT(inst)) {
                case FUNCT_JR        :
                    MNE(jr);
                    FMT(RjFMT);
                    break;

                case FUNCT_SYSCALL   :
                    MNE(syscall);
                    FMT(R_FMT);
                    break;

                case FUNCT_ADD       :
                    MNE(add);
                case FUNCT_ADDU      :
                    MNE(addu);
                case FUNCT_SUB       :
                    MNE(sub);
                case FUNCT_SUBU      :
                    MNE(subu);
                case FUNCT_AND       :
                    MNE(and);
                case FUNCT_OR        :
                    MNE(or);
                case FUNCT_NOR       :
                    MNE(nor);
                case FUNCT_SLT       :
                    MNE(slt);
                case FUNCT_SLTU      :
                    MNE(sltu);
                    FMT(RFMT);
                    break;

                case FUNCT_SLL       :
                    MNE(sll);
                case FUNCT_SRL       :
                    MNE(stl);
                    FMT(RsFMT);
                    break;

                default:
                    MNE(unknown);
                    FMT(R_FMT);
                    break;
            }
            break;

        case OPCODE_J        :
            MNE(j);
        case OPCODE_JAL      :
            MNE(jal);
            FMT(JFMT);
            break;

        case OPCODE_BEQ      :
            MNE(beq);
        case OPCODE_BNE      :
            MNE(bne);
            FMT(IbFMT);
            break;

        case OPCODE_ADDI     :
            MNE(addi);
        case OPCODE_SLTI     :
            MNE(slti);
        case OPCODE_LW       :
            MNE(lw);
        case OPCODE_SW       :
            MNE(sw);
            FMT(ImFMT);
            break;

        case OPCODE_ADDIU    :
            MNE(addiu);
        case OPCODE_SLTIU    :
            MNE(sltiu);
            FMT(IuFMT);
            break;

        case OPCODE_ANDI     :
            MNE(andi); 
        case OPCODE_ORI      :
            MNE(ori);
        case OPCODE_LUI      :
            MNE(lui);
            FMT(IxFMT);
            break;

        default:
            MNE(unknown);
            FMT(R_FMT);
            break;
    }
    
    PRINT;

#undef PRINT
#undef MNE
#undef FMT

#undef RFMT
#undef RsFMT
#undef R_FMT
#undef RjFMT
#undef JFMT
#undef IxFMT
#undef IuFMT
#undef IdFMT
#undef IbFMT

}

void dis_MIPS(MIPS_t *mips) {
    
    disassemble(& mips->if_id.dis, "if_id ");
    disassemble(& mips->id_ex.dis, "id_ex ");
    disassemble(& mips->ex_mem.dis, "ex_mem");
    disassemble(& mips->mem_wb.dis, "mem_wb");

}

void wait(void) {
    int c = 0;
    do {
        c = getchar();
        if (EOF == c)
            errorf("ERROR: hit end of stdin while waiting.\n");
    } while ('\n' != c);
}
