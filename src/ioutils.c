#define IOUTILS_C
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include <ioutils.h>
#include <utils.h>
#include <debug.h>
#undef IOUTILS_C

void usage(const char *pname) {
    
    if (NULL == pname)
        errorf("ERROR: usage - NULL program name.\n");

    for (const char **umsg = &usage_message[0]; NULL != *umsg; umsg++) {
        if (0 > printf(*umsg, pname)) exit(EXIT_FATAL);
        if (EOF == putchar('\n')) exit(EXIT_FATAL);
    }

    exit(EXIT_FAILURE);
}

void errorf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (0 > vfprintf(stderr, fmt, args)) {
        exit(EXIT_FATAL);
    }
    exit(EXIT_FAILURE);
}

void clear_config_file(config_file *conf) {
    memset(conf, 0, sizeof(config_file));
    conf->name = NULL;
}

void read_config_file(const char *name, config_file *conf) {
    FILE *f;

    debug("Opening configuration file `%s'\n", name);

    if (NULL == (f = fopen(name, "r")))
        errorf("ERROR: read_config_file - Unable to open confuguration file `%s'\nReason: %s", name, strerror(errno));
    
    conf->name = name;
    read_config_filef(f, conf);
    if (EOF == fclose(f))
        errorf("ERROR: read_config_file - unable to close configuration file handle.\nReason: %s", strerror(errno));
}

void read_config_filef(FILE *file, config_file *conf) {

    if (NULL == conf)
        errorf("ERROR: read_config_file - config_file struct pointer NULL.\n");

    debug("Reading configuration file `%s'.\n", NULL == conf->name ? "<unknown>" : conf->name );
    
    int ret = fscanf(file, config_file_format,
        &conf->t_regs[0],
        &conf->t_regs[1],
        &conf->t_regs[2],
        &conf->t_regs[3],
        &conf->t_regs[4],
        &conf->t_regs[5],
        &conf->t_regs[6],
        &conf->t_regs[7],
        &conf->icache.sets,
        &conf->icache.blocks,
        &conf->icache.words,
        &conf->dcache.sets,
        &conf->dcache.blocks,
        &conf->dcache.words,
        &conf->l2cache.sets,
        &conf->l2cache.blocks,
        &conf->l2cache.words);

    if (17 != ret)
        errorf("ERROR: read_config_file - configuration file %s%snot in correct format\n",
                NULL == conf->name ? "" : conf->name, 
                NULL == conf->name ? " " : "");
}

void verify_config(config_file *conf) {
    bool ok = true; 
    for (int i = 0; i < 8; i++) {
        ok = ok && conf->t_regs[i] <= UINT_MAX;        
    }
    ok = ok && popcnt(conf->icache.sets) < 2;
    ok = ok && popcnt(conf->icache.blocks) < 2;
    ok = ok && popcnt(conf->icache.words) < 2;

    ok = ok && popcnt(conf->dcache.sets) < 2;
    ok = ok && popcnt(conf->dcache.blocks) < 2;
    ok = ok && popcnt(conf->dcache.words) < 2;

    ok = ok && popcnt(conf->l2cache.sets) < 2;
    ok = ok && popcnt(conf->l2cache.blocks) < 2;
    ok = ok && popcnt(conf->l2cache.words) < 2;

    if (!ok)
        errorf("ERROR: read_config_file - configuration file %s%scontains illegal values\n",
                NULL == conf->name ? "" : conf->name, 
                NULL == conf->name ? " " : "");
}
