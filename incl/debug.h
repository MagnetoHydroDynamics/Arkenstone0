
#ifndef DEBUG_H
#define DEBUG_H

#include <mod.h>

#ifndef DEBUG_C
extern FILE *debug_stream;
#else 
FILE *debug_stream = NULL;
#endif

// Set the debug stream
void set_debug(const char *);
void set_debugf(FILE *);

// Debug printf alias
void debug(const char *, ...);

// Hang until user presses enter
void wait(void);

// Disassemble an instruction
void disassemble(disas_t *, const char*);

// Disassemble the pipeline registers
void dis_MIPS(MIPS_t *);

#endif //ndef DEBUG_H
