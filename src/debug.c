
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_C
#include <debug.h>
#include <ioutils.h>

void set_debug(const char *fname) {
    FILE *f;
    if (NULL == (f = fopen(fname, "a")))
        errorf("ERROR: set_debug - Unable to open debug log file `%s'\nReason: %s\n",
                fname, strerror(errno)); 
    set_debugf(f);
}

void set_debugf(FILE *file) {
    debug_stream = file;
    if (NULL != debug_stream)
    if (0 > fprintf(debug_stream, "DEBUG ON\n"))
        errorf("ERROR: set_debugf - Unable to write to debug stream.");
}

void debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (NULL != debug_stream)
    if (0 > vfprintf(debug_stream, fmt, args))
        errno = 0; // ignore all errors
}
