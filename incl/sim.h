
#ifndef SIM_H
#define SIM_H

#include <mod.h>
#include <mem.h>

// Interpretation infrastructure
interp_r interpret(MIPS_t*);
interp_r cycle(MIPS_t*);
interp_r stage_if(MIPS_t*);
interp_r stage_id(MIPS_t*);
interp_r stage_ex(MIPS_t*);
interp_r stage_mem(MIPS_t*);
interp_r stage_wb(MIPS_t*);

// Create bubble
void stall(MIPS_t*);

#endif //ndef SIM_H
