
#define SIM_C
#include <string.h>

#include <mod.h>
#include <debug.h>
#include <sim.h>
#include <mem.h>
#include <mips32.h>
#undef SIM_C

interp_r interpret(MIPS_t *mips) {
    
    interp_r res = CONTINUE;

    do {
        res = cycle(mips);
    } while (CONTINUE == res);

    debug("\n");
    
    return res;
}

interp_r cycle(MIPS_t *mips) {
    
    debug("\n");

    interp_r code;

#define GUARD(call) do { if (CONTINUE != (code = call)) return code; } while (0)
    
    debug("xx_if  = mem[%08X]\n", mips->pc);
    dis_MIPS(mips);

    GUARD(stage_wb(mips));
    GUARD(stage_mem(mips));
    GUARD(stage_ex(mips));
    GUARD(stage_id(mips));
    GUARD(stage_if(mips));

#undef GUARD

    mips->if_id.bubble = false;
    mips->id_ex.stall = false;

    if (mips->id_ex.jump && mips->ex_mem.jump)
        return DELAY_SLOT_JUMP;

    else if (mips->id_ex.jump)
        mips->pc = mips->id_ex.jump_target;

    else if (mips->ex_mem.jump && mips->ex_mem.cond)
        mips->pc = mips->ex_mem.jump_target;
    
    else
        mips->pc = mips->if_id.next_pc;

    mips->cycle_cnt += 1;

    return CONTINUE;
}

interp_r stage_if(MIPS_t *mips) {
    IFID_t *ifid = &mips->if_id;
    
    if (!ifid->bubble) {
        // TODO: caching
        
        ifid->inst = GET_BIGWORD(*(mips->mem), mips->pc);
    
        ifid->opcode = GET_OPCODE(ifid->inst);
        ifid->immed = GET_IMM(ifid->inst);
        ifid->address = GET_ADDRESS(ifid->inst);

        ifid->next_pc = mips->pc + 4;
    }

    ifid->dis.bubble = ifid->bubble;
    ifid->dis.addr = mips->pc;
    ifid->dis.inst = ifid->inst;

    return CONTINUE;
}

interp_r stage_id(MIPS_t *mips) {

    
    IFID_t *ifid = &mips->if_id;
    IDEX_t *idex = &mips->id_ex;
    MEMWB_t *memwb = &mips->mem_wb;
    b32 inst = ifid->inst;
    b16 immed = ifid->immed;

    idex->jump = false;
    idex->jump_next = false;
    idex->invert = false;
    idex->jump_target = 0x0;
    idex->immed = immed;
    idex->funct = 0x0;
    idex->shamt = 0x0;
    idex->rd = ZERO;
    idex->rs = GET_RS(inst);
    idex->rt = GET_RT(inst);
    idex->rs_val = mips->regs[idex->rs];
    idex->rt_val = mips->regs[idex->rt];
    idex->dis = ifid->dis;
    
    // Register forwarding
    if (idex->rs == memwb->rd) idex->rs_val = memwb->rd_val;
    if (idex->rt == memwb->rd) idex->rt_val = memwb->rd_val;

    if(idex->rs == ZERO) idex->rs_val = 0;
    if(idex->rt == ZERO) idex->rt_val = 0;


    switch (ifid->opcode) {
        case OPCODE_R        :
            idex->immed = 0x0;
            idex->funct = GET_FUNCT(inst);
            idex->shamt = GET_SHAMT(inst);
            idex->rd = GET_RD(inst);


            return CONTINUE;

        case OPCODE_JAL      :
            idex->funct = FUNCT_OR;
            idex->rd = RA;
            idex->rs = ZERO;
            idex->rs_val = ifid->next_pc;
            idex->rt = ZERO;
            idex->rt_val = 0x0;
        case OPCODE_J        :
            idex->jump = true;
            idex->jump_target = ifid->address * 4;

            return CONTINUE;

        case OPCODE_BNE      :
            idex->invert = true;
        case OPCODE_BEQ      :
            idex->immed = SIGN_EXTEND(immed) * 4;
            idex->jump_next = true;
            idex->jump_target = ifid->next_pc;
            idex->funct = FUNCT_SUBU;

            return CONTINUE;

        case OPCODE_ADDI     :
            idex->rd = idex->rt;
            idex->rt_val = SIGN_EXTEND(immed);
            idex->funct = FUNCT_ADD;

            return CONTINUE;

        case OPCODE_ADDIU    :
            idex->rd = idex->rt;
            idex->rt_val = ZERO_EXTEND(ifid->immed);
            idex->funct = FUNCT_ADDU;

            return CONTINUE;

        case OPCODE_SLTI     :
            idex->rd = idex->rt;
            idex->rt_val = SIGN_EXTEND(immed);
            idex->funct = FUNCT_SLT;

            return CONTINUE;

        case OPCODE_SLTIU    :
            idex->rd = idex->rt;
            idex->rt_val = ZERO_EXTEND(immed);
            idex->funct = FUNCT_SLTU;

            return CONTINUE;

        case OPCODE_ANDI     :
            idex->rd = idex->rt;
            idex->rt_val = ZERO_EXTEND(immed);
            idex->funct = FUNCT_AND;

            return CONTINUE;

        case OPCODE_ORI      :
            idex->rd = idex->rt;
            idex->rt_val = ZERO_EXTEND(immed);
            idex->funct = FUNCT_OR;

            return CONTINUE;

        case OPCODE_LUI      :
            idex->rd = idex->rt;
            idex->rt_val = ZERO_EXTEND(immed) << 16;
            idex->rs = 0;
            idex->rs_val = 0;
            idex->funct = FUNCT_OR;

            return CONTINUE;

        case OPCODE_LW       :
            idex->access = WORD;
            idex->action = MEM_LOAD;
            idex->jump_target = idex->rs_val;
            idex->immed = SIGN_EXTEND(immed);

            idex->rd = idex->rt;
            idex->rt = ZERO;
            idex->rt_val = 0x0;
            idex->rs = ZERO;
            idex->rs_val = 0x0;
            idex->funct = FUNCT_OR;
            
            // stall twice to avoid hazards
            stall(mips);
            idex->stall = true;

            return CONTINUE;

        case OPCODE_SW       :
            idex->access = WORD;
            idex->action = MEM_STORE;
            idex->jump_target = idex->rs_val;
            idex->immed = SIGN_EXTEND(immed);

            idex->rd = idex->rt;
            idex->rt = ZERO;
            idex->rs = ZERO;
            idex->rs_val = 0x0;
            idex->funct = FUNCT_OR;

            // stall twice to avoid hazards
            stall(mips);
            idex->stall = true;

            return CONTINUE;

        default:
            return BAD_INSTRUCTION;
    }

    return INTERNAL_ERROR;

}

interp_r stage_ex(MIPS_t *mips) {

    IDEX_t *idex = &mips->id_ex;
    EXMEM_t *exmem = &mips->ex_mem;
    b64 I = 0;

    if (idex->stall) {
        stall(mips);
    }

    exmem->jump = idex->jump_next;
    exmem->jump_target = idex->jump_target + SIGN_EXTEND(idex->immed);
    exmem->cond = false;
    exmem->action = idex->action;
    exmem->access = idex->access;
    exmem->rd = idex->rd;
    exmem->rd_val = 0x0;

    exmem->dis = idex->dis;

    if (!exmem->dis.bubble)
        mips->inst_cnt++;


    switch (idex->funct) {
        case FUNCT_JR        :
            exmem->jump = true;
            exmem->cond = true;
            exmem->jump_target = idex->rs_val;

            return CONTINUE;

        case FUNCT_SYSCALL   :
            return SYSCALL_HIT;

        case FUNCT_ADD       :
            I = idex->rs_val;
            I += idex->rt_val;
            if (I > UINT32_MAX) return OVERFLOW;

            exmem->rd_val = (b32) I;

            return CONTINUE;

        case FUNCT_SUB       :
            I = idex->rs_val;
            I -= idex->rt_val;
            if (I > UINT32_MAX) return OVERFLOW;

            exmem->rd_val = (b32) I;
            return CONTINUE;

        case FUNCT_ADDU      :
            exmem->rd_val = idex->rs_val + idex->rt_val;
            return CONTINUE;



        case FUNCT_SUBU      :
            exmem->rd_val = idex->rs_val - idex->rt_val;

            // BEQ and BNE
            exmem->cond = idex->invert ^ (exmem->rd_val == 0);

            return CONTINUE;

        case FUNCT_AND       :
            exmem->rd_val = idex->rs_val & idex->rt_val;

            return CONTINUE;

        case FUNCT_OR        :
            exmem->rd_val = idex->rs_val | idex->rt_val;

            return CONTINUE;

        case FUNCT_NOR       :
            exmem->rd_val = ~(idex->rs_val | idex->rt_val);

            return CONTINUE;

        case FUNCT_SLT       :
            exmem->rd_val = ((int32_t)idex->rs_val) < ((int32_t)idex->rt_val);

            return CONTINUE;

        case FUNCT_SLTU      :
            exmem->rd_val = idex->rs_val < idex->rt_val;

            return CONTINUE;

        case FUNCT_SLL       :
            exmem->rd_val = idex->rt_val << idex->shamt;

            return CONTINUE;

        case FUNCT_SRL       :
            exmem->rd_val = idex->rt_val >> idex->shamt;

            return CONTINUE;

        default:
            return BAD_INSTRUCTION;
    }

    return INTERNAL_ERROR;

}

interp_r stage_mem(MIPS_t *mips) {

    
    EXMEM_t *exmem = &mips->ex_mem;
    MEMWB_t *memwb = &mips->mem_wb;
    MEMBUF_t *mem = mips->mem;

    b32 address = exmem->jump_target;

    memwb->rd = exmem->rd;
    memwb->rd_val = exmem->rd_val;

    memwb->dis = exmem->dis;

    // TODO: caching
    switch (exmem->action) {
        case MEM_IDLE:
            return CONTINUE;
        case MEM_LOAD:

            switch (exmem->access) {
                case WORD:
                    if ((address & ~0x3) != address)
                        return UNALIGNED_ACCESS;

                    memwb->rd_val = GET_BIGWORD(*mem, address);
                    return CONTINUE;

                case BYTE:
                    memwb->rd_val =
                    (*mips->mem)[address - MIPS_RESERVE];
                    return CONTINUE;

                case SBYTE:
                    memwb->rd_val =
                    SIGN_EXTEND((*mips->mem)[address - MIPS_RESERVE]);

                    return CONTINUE;

                default:
                    return INTERNAL_ERROR;
            }

        case MEM_STORE:
            memwb->rd_val = 0x0;
            memwb->rd = ZERO;
            switch (exmem->access) {
                case WORD:
                    if ((address & ~0x3) != address)
                        return UNALIGNED_ACCESS;

                    SET_BIGWORD(*mem, address, exmem->rd_val);
                    return CONTINUE;

                case BYTE:
                case SBYTE:
                    (*mips->mem)[address - MIPS_RESERVE] = (b8)exmem->rd_val;
                    return CONTINUE;

                default:
                    return INTERNAL_ERROR;
            }
        default:
            return INTERNAL_ERROR;
    }
    
    return CONTINUE;
}

interp_r stage_wb(MIPS_t *mips) {
    
    MEMWB_t *memwb = &mips->mem_wb;

    if (ZERO != memwb->rd) {
        mips->regs[memwb->rd] = memwb->rd_val;
    }

    return CONTINUE;
}

void stall(MIPS_t *mips) {

    IFID_t *ifid = &mips->if_id;
    
    ifid->next_pc = mips->pc;
    ifid->inst = 0;
    ifid->immed = 0;
    ifid->address = 0;
    ifid->opcode = 0;

    ifid->dis.bubble = true;
    ifid->dis.addr = 0x0;
    ifid->dis.inst = 0x0;
}



