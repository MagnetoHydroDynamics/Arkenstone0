
#ifndef UTILIO_H
#define UTILIO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {

    uint64_t t_regs[8];

    struct {
        size_t sets, blocks, words;
    } inst_cache, data_cache, l2_cache;

} config_file;

#ifndef IOUTILS_C
extern const int version[3];
extern const char *usage_message[];
extern const char *const config_file_format;
#else
const int version[3] = { 0, 0, 1 };
const char *usage_message[] = {
    "Usage:",  
    "    %1$s <config> <elf>",
    "",
    "Options:",
    "    none",
    "",
    "Arguments:",
    "    <config>    configuration file to read",
    "    <elf>       elf file to simulate",
    "",
    "Return code:",
    "    EXIT_SUCCESS for success",
    "    EXIT_FAILURE for failure",
    "    42 for fatal failure",
    "",
    "Description:",
    "    MIPS32 simulator",
    NULL
};

const char *const config_file_format =
"%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n"
"%zu,%zu,%zu\n"
"%zu,%zu,%zu\n"
"%zu,%zu,%zu\n" ;
#endif

#define EXIT_FATAL (42)
void usage() __attribute__((noreturn));
void errorf(const char *, ...) __attribute__((noreturn));

void read_config_file(const char *, config_file *);
void read_config_filef(FILE *, config_file *);
void verify_config(config_file *);

#endif //ndef UTILIO_H
