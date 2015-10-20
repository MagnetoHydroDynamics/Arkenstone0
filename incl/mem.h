
#ifndef MEM_H
#define MEM_H

#include <mod.h>

#define CACHE_LINE(c, s, b) (c->data + sizeof(b32) * (b + s * c->n_blocks) * c->n_words)
#define CACHE_SET(c, a) ((size_t)(a & (b32)(c->n_sets - 1)))

// Look up address in cache.
// If found, returns block number within set, and pointer to block
// If not, returns -1
size_t _cache_lookup(CACHE_t *, b32 , BLOCK_t **);

// Find a cache_line most suitable to new data
size_t _cache_replacement(CACHE_t *, b32 , BLOCK_t **);

// Write cache to main memory
void _cache_writeback(CACHE_t *, size_t , BLOCK_t *, MEMBUF_t *);

#endif //ndef MEM_H
