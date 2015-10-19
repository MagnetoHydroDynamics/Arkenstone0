
#ifndef DEBUG_H
#define DEBUG_H

#include <mod.h>

#ifndef DEBUG_C
extern FILE *debug_stream;
#else 
FILE *debug_stream = NULL;
#endif

void set_debug(const char *);
void set_debugf(FILE *);

void debug(const char *, ...);
void wait(void);

void disassemble(b32, b32);

#endif //ndef DEBUG_H
