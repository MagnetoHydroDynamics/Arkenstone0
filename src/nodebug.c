#include <stdlib.h>
#include <stdio.h>

#define DEBUG_C
#include <debug.h>

void set_debug(const char *_) { _ = _; }
void set_debugf(FILE *_) { _ = _; }
void debug(const char *_, ...) { _ = _; }
