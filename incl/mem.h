
#ifndef MEM_H
#define MEM_H

#include <mod.h>

#define CACHE_LINE(c, s, b) (&(c->data[sizeof(b32) * (b + s * c->n_blocks) * c->n_words]))
#define CACHE_SET(c, a) ((size_t)(a & (b32)(c->n_sets - 1)))

// Look up address in cache.
// If found, returns block number within set, and pointer to block
// If not, returns -1
size_t _cache_lookup(CACHE_t *, b32 , BLOCK_t **);

// Find a cache_line most suitable to new data
// Finds either the oldest or the first invalid
size_t _cache_replacement(CACHE_t *, b32 , BLOCK_t **);

// Write cache to main memory, invalidating it in the process
void _cache_writeback(CACHE_t *, size_t , BLOCK_t *, MEMBUF_t *);


b32 _cache_read(CACHE_t *, BLOCK_t *, b32);
void _cache_write(CACHE_t *, BLOCK_t *, b32, b32);

#endif //ndef MEM_H
