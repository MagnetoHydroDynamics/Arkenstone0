
#define MOD_C
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <mod.h>
#include <ioutils.h>
#include <debug.h>
#undef MOD_C

CACHE_t *new_CACHE(size_t sets, size_t blocks, size_t words) {

    debug("Allocating new cache. Sets: %zu, blocks: %zu, words: %zu\n", sets, blocks, words);

    CACHE_t *res = malloc(sizeof(CACHE_t) + sets * blocks * sizeof(BLOCK_t));

    if (NULL == res)
        errorf("ERROR: new_CACHE - unable to allocate memory\nReason: %s", strerror(errno));
    
    res->n_sets = sets;
    res->n_blocks = blocks;
    res->n_words = words;

    res->misses = 0;
    res->hits = 0;

    if (NULL == (res->data = malloc(res->sz = sizeof(b32) * sets * blocks * words)))
        errorf("ERROR: new_CACHE - unable to allocate memory\nReason: %s", strerror(errno));

    memset(res->data, 0, res->sz);
    memset(res->blocks, 0, res->n_sets * res->n_blocks);
    
    return res;
}

void clear_MIPS(MIPS_t *mips) {
    memset(mips, 0, sizeof(MIPS_t));
}

void configure_MIPS(MIPS_t *mips, config_file *conf, size_t buf) {
    
    debug("Initializing MIPS model. Memory size: %zu\n", buf);

    verify_config(conf);
    if (0 == buf)
        errorf("ERROR: init_MIPS - zero buffer size\n");
    
    for (int i = T0; i < TEND; i++) {
        mips->regs[i] = conf->t_regs[i - T0];
    }

    if (NULL == (mips->mem = malloc(mips->mem_sz = buf)))
        errorf("ERROR: init_MIPS - unable to allocate memory buffer\nReason: %s", strerror(errno));
    
    mips->icache = new_CACHE(conf->icache.sets, conf->icache.blocks, conf->icache.words);
    mips->dcache = new_CACHE(conf->dcache.sets, conf->dcache.blocks, conf->dcache.words);
    mips->l2cache = new_CACHE(conf->l2cache.sets, conf->l2cache.blocks, conf->l2cache.words);
}

void free_MIPS(MIPS_t *mips) {
    if (NULL == mips) return;
    if (NULL != mips->mem) free(mips->mem);
    if (NULL != mips->icache) {
        if (NULL != mips->icache->data)
            free(mips->icache->data);
        free(mips->icache);
    }
    if (NULL != mips->dcache) {
        if (NULL != mips->dcache->data)
            free(mips->dcache->data);
        free(mips->dcache);
    }
    if (NULL != mips->l2cache) {
        if (NULL != mips->l2cache->data)
            free(mips->l2cache->data);
        free(mips->l2cache);
    }
}

void report_status(MIPS_t *mips) {
    printf("Executed %zu instruction(s).\n"
           "%zu cycle(s) elapsed.\n"
           "icache hits: %zu, misses: %zu.\n"
           "dcache hits: %zu, misses: %zu.\n"
           "l2cache hits: %zu, misses: %zu.\n"
           "pc = 0x%x\n",

            mips->inst_cnt,
            mips->cycle_cnt,

            mips->icache->hits,
            mips->icache->misses,
            mips->dcache->hits,
            mips->dcache->misses,

            mips->l2cache->hits,
            mips->l2cache->misses,

            mips->pc);
    
    for (const b6 *p = &report_regs[0]; *p != ZERO; p++)
        printf("%s = 0x%x\n", reg_names[*p], mips->regs[*p]);
}

void read_MIPS(MIPS_t *mips, const char *fname) {

    switch (elf_dump(fname, &(mips->pc), &(*mips->mem)[0], mips->mem_sz)) {
        case ELF_ERROR_IO_ERROR        :
            errorf("ERROR: read_MIPS - problems reading file `%s'.\n", fname);
        case ELF_ERROR_OUT_OF_MEM      :
            errorf("ERROR: read_MIPS - ELF file larger than allocated memory buffer.\n");
        case ELF_ERROR_NOT_ELF         :
            errorf("ERROR: read_MIPS - not an ELF file.\n");
        case ELF_ERROR_NOT_SUPPORTED   :
            errorf("ERROR: read_MIPS - unsupported ELF features detected.\n");
    }

}
