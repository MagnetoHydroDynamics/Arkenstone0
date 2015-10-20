
#ifndef SIM_H
#define SIM_H

#include <mod.h>

interp_r interpret(MIPS_t*);
interp_r cycle(MIPS_t*);
interp_r stage_if(MIPS_t*);
interp_r stage_id(MIPS_t*);
interp_r stage_ex(MIPS_t*);
interp_r stage_mem(MIPS_t*);
interp_r stage_wb(MIPS_t*);

interp_r stall(MIPS_t*);

// NULL if cache miss
b8 *cache_lookup(MIPS_t*, b32, access_t);

#endif //ndef SIM_H
