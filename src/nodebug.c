#define DEBUG_C
#include <stdlib.h>
#include <stdio.h>

#include <debug.h>
#include <mod.h>
#undef DEBUG_C

void set_debug(const char *_) { _ = _; }
void set_debugf(FILE *_) { _ = _; }
void debug(const char *_, ...) { _ = _; }

void disassemble(disas_t *_, const char *__) { _ = _; __ = __; }
void dis_MIPS(MIPS_t *_) { _ = _; }

void wait(void) { }
