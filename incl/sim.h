
#ifndef SIM_H
#define SIM_H

#include <mod.h>

interp_r interpret(MIPS_t*);
interp_r cycle(MIPS_t*);
interp_r stall(MIPS_t*, phase_t);

#endif //ndef SIM_H
