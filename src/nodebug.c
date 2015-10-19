#define DEBUG_C
#include <stdlib.h>
#include <stdio.h>

#include <debug.h>
#include <mod.h>
#undef DEBUG_C

void set_debug(const char *_) { _ = _; }
void set_debugf(FILE *_) { _ = _; }
void debug(const char *_, ...) { _ = _; }

void disassemble(b32 _, b32 __) { _ = _; __ = __; }

void wait(void) { }
