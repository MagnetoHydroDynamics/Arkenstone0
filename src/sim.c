
#define SIM_C
#include <sim.h>
#include <mod.h>
#undef SIM_C

interp_r interpret(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r cycle(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r stall(MIPS_t *mips, phase_t ph) { mips = mips; ph = ph; return CONTINUE; }
