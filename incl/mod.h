
#ifndef SIM_H
#define SIM_H

#include <stdint.h>
#include <stdbool.h>
#include <ioutils.h>

typedef bool b1;
typedef uint8_t b8;
typedef uint8_t b5;
typedef uint8_t b6; 
typedef uint16_t b16;
typedef uint32_t b26;
typedef uint32_t b32;
typedef float f32;

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

typedef uint8_t mem_action_t;
#define MEM_IDLE  ((mem_action_t)0)
#define MEM_LOAD  ((mem_action_t)1)
#define MEM_STORE ((mem_action_t)2)

typedef int access_t;
#define BYTE ((scalarwidth_t)1)
#define HALF ((scalarwidth_t)2)
#define WORD ((scalarwidth_t)4)

typedef int phase_t;
#define IF ((phase_t)0)
#define ID ((phase_t)1)
#define EX ((phase_t)2)
#define MEM ((phase_t)3)
#define WB ((phase_t)4)

#define KB (1024)
#define MEMSZ (640*KB)

typedef b8 MEMBUF_t[];

typedef struct {
    bool valid;
    bool modified;
    b32 tag;
} BLOCK_t;

typedef struct {
    int misses;
    size_t n_sets;
    size_t n_blocks;
    size_t n_words;
    size_t sz;
    b8 *data;
    BLOCK_t blocks[];
} CACHE_t;

typedef b32 REGS_t[32];

typedef f32 FREGS_t[32];

typedef struct  {
    b32 LO, HI;   
} MREGS_t;

typedef struct {
    b6 opcode;
    b26 address;
    b32 inst;
} IFID_t;

typedef struct {
    b1 jump;
    b32 jump_target;
} IDEX_t;

typedef struct {
    access_t acess;
    mem_action_t action;
    
    b32 address;
    b32 value;
    b6 reg;
} EXMEM_t;

typedef struct {
    b1 nop;
    b32 value;
    b6 reg;
} MEMWB_t;

typedef struct {
    int cycle_cnt;
    int inst_cnt;

    b32 PC;
    REGS_t regs;
    FREGS_t fregs;
    MREGS_t mregs;

    IFID_t if_id;
    IDEX_t id_ex;
    EXMEM_t ex_mem;
    MEMWB_t mem_wb;
    
    CACHE_t *icache;
    CACHE_t *dcache;
    CACHE_t *l2cache;
    
    size_t mem_sz;
    MEMBUF_t *mem;

} MIPS_t;

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
};
#endif

interp_r interpret(MIPS_t*);
interp_r cycle(MIPS_t*);
interp_r stall(MIPS_t*, phase_t);

CACHE_t *new_CACHE(size_t, size_t, size_t);
void init_MIPS(MIPS_t*, config_file*, size_t);
void free_MIPS(MIPS_t*);

#endif //ndef SIM_H
