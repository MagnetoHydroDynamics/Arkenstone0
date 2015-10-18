
#include <utilio.h>

#define DEBUG_C
#include <debug.h>

void set_debug(const char *fname) {
    FILE *f;
    if (NULL == (f = fopen(fname)))
        errorf("ERROR: Unable to open debug log file `%s'", fname); 
}

void set_debugf(FILE *file) {
    debug_stream = file;
    if (NULL != debug_steam)
    if (0 > fprintf(debug_stream, "DEBUG ON\n"))
        errorf("ERROR: Unable to write to debug stream.");
}

void debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (NULL != debug_stream)
    if (0 > vfprintf(debug_stream, fmt, args))
        errno = 0; // ignore all errors
}
