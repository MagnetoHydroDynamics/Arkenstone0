
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define UTILIO_C
#include <utilio.h>

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

void read_config_file(const char *name, config_file *conf) {
    FILE *f;
    if (NULL == (f = fopen(name, "r")))
        errorf("ERROR: Unable to open confuguration file `%s'\nReason: %s", name, strerror(errno));
    
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
        conf->inst_cache.sets,
        conf->inst_cache.blocks,
        conf->inst_cache.words,
        conf->data_cache.sets,
        conf->data_cache.blocks,
        conf->data_cache.words,
        conf->l2_cache.sets,
        conf->l2_cache.blocks,
        conf->l2_cache.words);

    if (17 != ret)
        errorf("ERROR: Configuration file not in correct format\n");

}

void verify_config(config_file *conf) {
    bool ok = true; 
    for (int i = 0; i < 8; i++) {
        ok = ok && conf->t_regs[i] <= UINT_MAX;        
    }
    of = ok && popcnt(conf->inst_cache.sets) < 2;
    of = ok && popcnt(conf->inst_cache.blocks) < 2;
    of = ok && popcnt(conf->inst_cache.words) < 2;

    of = ok && popcnt(conf->data_cache.sets) < 2;
    of = ok && popcnt(conf->data_cache.blocks) < 2;
    of = ok && popcnt(conf->data_cache.words) < 2;

    of = ok && popcnt(conf->l2_cache.sets) < 2;
    of = ok && popcnt(conf->l2_cache.blocks) < 2;
    of = ok && popcnt(conf->l2_cache.words) < 2;

    if (!ok)
        errorf("ERROR: Configuration file contains illegal values\n");
}
