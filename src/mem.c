
#define MEM_C
#include <string.h>

#include <mod.h>
#include <mem.h>
#include <mips32.h>
#include <debug.h>
#undef MEM_C

size_t _cache_lookup(CACHE_t *cache, b32 addr, BLOCK_t **block_ret) {
   
    debug("Cache lookup of %08X in %scache\n", addr, cache->name);

    size_t set = CACHE_SET(cache, addr);

    BLOCK_t *range = &cache->blocks[set * cache->n_blocks];

    size_t block;
    for (block = 0; block < cache->n_blocks; block++) {
        if (!range[block].valid) continue;

        if (range[block].tag <= addr && addr < range[block].tag + sizeof(b32)*cache->n_words) {

            debug("Hit block %zu:%08X of set %zu of %scache\n", block, range[block].tag, set, cache->name);
            
            *block_ret = &range[block];
            cache->hits++;

            return block;
        }
    } 

    debug("miss\n");
    cache->misses++;
    *block_ret = NULL;
    return -1;
}

size_t _cache_replacement(CACHE_t *cache, b32 addr, BLOCK_t **block_ret) {
    
    size_t set = CACHE_SET(cache, addr);
    debug("Finding suitable replacement block in set %zu of %scache.\n", set, cache->name);

    BLOCK_t *range = &cache->blocks[set * cache->n_blocks];

    size_t block;
    size_t least = 0;
    for (block = 0; block < cache->n_blocks; block++) {
        if (!range[block].valid) break;
        if (range[block].age < range[least].age) least = block;
    } 
    if (block < cache->n_blocks) least = block;
    block = least;

    *block_ret = &range[block];
    return block;
    
}

void _cache_writeback(CACHE_t *cache, size_t block_n, BLOCK_t *block, MEMBUF_t *mem) {

    debug("Writing back block %zu of set %zu from %scache to %08X.\n",
            block_n, block->tag & (cache->n_sets - 1), cache->name, block->tag);

    size_t set = block->tag & (cache->n_sets - 1);

    memcpy(&(*mem)[block->tag - MIPS_RESERVE],
        CACHE_LINE(cache, set, block_n), 
        cache->n_words * sizeof(b32));
}
