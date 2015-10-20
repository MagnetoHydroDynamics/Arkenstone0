
#define SIM_C
#include <sim.h>
#include <mod.h>
#undef SIM_C

interp_r interpret(MIPS_t *mips) { mips = mips; return CONTINUE; }

interp_r cycle(MIPS_t *mips) {
    
    interp_r code;

#define GUARD(call) do { if (CONTINUE != (code = call)) return code; } while (0)

    GUARD(stage_wb(mips));
    GUARD(stage_mem(mips));
    GUARD(stage_ex(mips));
    GUARD(stage_id(mips));
    GUARD(stage_if(mips));

#undef GUARD;

    if (mips->id_ex.jump)
        mips->pc = mips->id_ex.jump_target;

    else if (mips->ex_mem.jump)
        mips->pc = mips->ex_mem.jump_target;

    else if (mips->id_ex.jump && mips->ex_mem.jump)
        return DELAY_SLOT_JUMP;
    
    else
        mips->pc = mips->id_ex.next_pc;
}

interp_r stage_if(MIPS_t *mips) {
    
}

interp_r stall(MIPS_t *mips) {

    IFID_t *ifid = &mips->if_id;
    
    ifid->next_pc = mips->pc;
    ifid->inst = 0;
    ifid->immed = 0;
    ifid->address = 0;
    ifid->opcode = 0;

    ifid->dis.addr = 0xFFFFFFF;
    ifid->dis.inst = 0x0;
}

