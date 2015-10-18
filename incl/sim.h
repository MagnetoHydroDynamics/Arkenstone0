
#include <stdint.h>

#ifndef SIM_H
#define SIM_H

typedef bool b1;
typedef uint8_t b8;
typedef uint8_t b5;
typedef uint8_t b6; 
typedef uint16_t b16;
typedef uint32_t b26;
typedef uint32_t b32;
typedef float f32;
typedef uint32_t inst_t;

#define ZERO ((b5)0)
#define AT ((b5)1)
#define V0 ((b5)2)
#define V1 ((b5)3)
#define A0 ((b5)4)
#define A1 ((b5)5)
#define A2 ((b5)6)
#define A3 ((b5)7)
#define T0 ((b5)8)
#define T1 ((b5)9)
#define T2 ((b5)10)
#define T3 ((b5)11)
#define T4 ((b5)12)
#define T5 ((b5)13)
#define T6 ((b5)14)
#define T7 ((b5)15)
#define TEND ((b5)16) // For iteration purposes
#define S0 ((b5)16)
#define S1 ((b5)17)
#define S2 ((b5)18)
#define S3 ((b5)19)
#define S4 ((b5)20)
#define S5 ((b5)21)
#define S6 ((b5)22)
#define S7 ((b5)23)
#define T8 ((b5)24)
#define T9 ((b5)25)
#define K0 ((b5)26)
#define K1 ((b5)27)
#define GP ((b5)28)
#define SP ((b5)29)
#define FP ((b5)30)
#define RA ((b5)31)

typedef int interp_r;
#define CONTINUE         ((interp_r)0)
#define SYSCALL_HIT      ((interp_r)1)
#define OVERFLOW         ((interp_r)2)
#define DELAY_SLOT_JUMP  ((interp_r)3)
#define ACCESS_VIOLATION ((interp_r)4)
#define UNALIGNED_ACCESS ((interp_r)5)
#define BAD_INSTRUCTION  ((interp_r)6)
#define INTERNAL_ERROR  ((interp_r)7)
#define UNIMPLEMENTED  ((interp_r)8)

typedef uint8_t mem_action;
#define MEM_IDLE  ((mem_action)0)
#define MEM_LOAD  ((mem_action)1)
#define MEM_STORE ((mem_action)2)

typedef int pipeline_phase;
#define IF ((pipeline_phase)0)
#define ID ((pipeline_phase)1)
#define EX ((pipeline_phase)2)
#define MEM ((pipeline_phase)3)
#define WB ((pipeline_phase)4)

#define KB (1024)
#define MEMSZ (640*KB)

typedef u8 MEMBUF_t[MEMSZ];

typedef b32 REGS_t[32];

typedef b32 PC_t;

typedef f32 FREGS_t[32];

typedef struct  {
    b32 LO, HI;   
} MULREGS_t;

#ifndef SIM_C
extern const char *reg_names[32];
#else
const char *reg_names[32] = {
    "0", "at", "v0", "v1", 
    "a0", "a1", "a2", "a3", 
    "t0", "t1", "t2", "t3", 
    "t4", "t5", "t6", "t7", 
    "s0", "s1", "s2", "s3", 
    "s4", "s5", "s6", "s7", 
    "t8", "t9", "k0", "k1", 
    "gp", "sp", "fp", "ra",
}
#endif

#endif //ndef SIM_H
