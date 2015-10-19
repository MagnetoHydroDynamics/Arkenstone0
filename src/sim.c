
#include <stdlib.h>

#define SIM_C
#include <sim.h>
#include <ioutils.h>

interp_r interpret(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r cycle(MIPS_t *mips) { mips = mips; return CONTINUE; }
interp_r stall(MIPS_t *mips, phase_t ph) { mips = mips; ph = ph; return CONTINUE; }

void init_MIPS(MIPS_t *mips, config_file *conf, size_t buf) {
    verify_config(conf);
    if (0 == buf)
        errorf("ERROR: init_MIPS - zero buffer size\n");
    
    for (int i = T0; i < TEND; i++) {
        mips->regs[i] = conf->t_regs[i - T0];
    }

    if (NULL == (mips->mem = malloc(mips->mem_sz = buf)))
        errorf("ERROR: init_MIPS - unable to allocate memory buffer\n");
    
#define CACHESZ(cache) (cache.sets * cache.blocks * cache.words * 4)
    mips->icache_sz = CACHESZ(conf->inst_cache);
    mips->dcache_sz = CACHESZ(conf->data_cache);
    mips->l2cache_sz = CACHESZ(conf->l2_cache);
#undef CACHESZ

#define ALLOC(cache) do if (NULL == (mips->cache = malloc(mips->cache##_sz))) \
        errorf("ERROR: init_MIPS - unable to allocate cache buffer\n"); while (0)

    ALLOC(icache);
    ALLOC(dcache);
    ALLOC(l2cache);

#undef ALLOC
    
}

void free_MIPS(MIPS_t *mips) {
#define SAFE_FREE(cache) do if (NULL != cache) free(cache); while (0)
    SAFE_FREE(mips->mem);
    SAFE_FREE(mips->icache);
    SAFE_FREE(mips->dcache);
    SAFE_FREE(mips->l2cache);
#undef SAFE_FREE
}
