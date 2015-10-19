
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SIM_C
#include <sim.h>
#include <ioutils.h>
#include <debug.h>

interp_r interpret(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r cycle(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r stall(MIPS_t *mips, phase_t ph) { mips = mips; ph = ph; return CONTINUE; }

CACHE_t *new_CACHE(size_t sets, size_t blocks, size_t words) {

    debug("Allocating new cache. Sets: %zu, blocks: %zu, words: %zu\n", sets, blocks, words);

    CACHE_t *res = malloc(sizeof(CACHE_t) + sets * blocks * sizeof(BLOCK_t));

    if (NULL == res)
        errorf("ERROR: new_CACHE - unable to allocate memory\nReason: %s", strerror(errno));
    
    res->n_sets = sets;
    res->n_blocks = blocks;
    res->n_words = words;

    res->misses = 0;

    if (NULL == (res->data = malloc(res->sz = sizeof(b32) * sets * blocks * words)))
        errorf("ERROR: new_CACHE - unable to allocate memory\nReason: %s", strerror(errno));

    memset(res->data, 0, res->sz);
    memset(res->blocks, 0, res->n_sets * res->n_blocks);
    
    return res;
}

void init_MIPS(MIPS_t *mips, config_file *conf, size_t buf) {
    
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
