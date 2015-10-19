
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define IOUTILS_C
#include <ioutils.h>
#include <utils.h>

void usage(const char *pname) {

    for (const char **umsg = &usage_message[0]; NULL != *umsg; umsg++) {
        if (0 > printf(*umsg, pname)) exit(EXIT_FATAL);
        umsg++;
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

void init_config_file(config_file *conf) {
    memset(conf, 0, sizeof(config_file));
    conf->name = NULL;
}

void read_config_file(const char *name, config_file *conf) {
    FILE *f;
    if (NULL == (f = fopen(name, "r")))
        errorf("ERROR: read_config_file - Unable to open confuguration file `%s'\nReason: %s", name, strerror(errno));
    
    conf->name = name;
    read_config_filef(f, conf);
}

void read_config_filef(FILE *file, config_file *conf) {
    
    int ret = fscanf(file, config_file_format,
        conf->t_regs[0],
        conf->t_regs[1],
        conf->t_regs[2],
        conf->t_regs[3],
        conf->t_regs[4],
        conf->t_regs[5],
        conf->t_regs[6],
        conf->t_regs[7],
        conf->t_regs[8],
        conf->icache.sets,
        conf->icache.blocks,
        conf->icache.words,
        conf->dcache.sets,
        conf->dcache.blocks,
        conf->dcache.words,
        conf->l2cache.sets,
        conf->l2cache.blocks,
        conf->l2cache.words);

    if (17 != ret)
        errorf("error: read_config_file - configuration file %s%snot in correct format\n",
                null == conf->name ? "" : conf->name, 
                null == conf->name ? " " : "");
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
        errorf("error: read_config_file - configuration file %s%scontains illegal values\n",
                null == conf->name ? "" : conf->name, 
                null == conf->name ? " " : "");
}
