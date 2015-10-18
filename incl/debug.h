
#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG_C
extern FILE *debug_steam;
#else 
FILE *debug_steam = NULL;
#endif

void set_debug(const char *);
void set_debugf(FILE *);

void debug(const char *, ...);

#endif //ndef DEBUG_H
