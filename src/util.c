
#define UTIL_C
#include <util.h>

// Taken from wikipedia
// works well when most bits are 0
int popcnt(size_t s) {
    int count;
    for(count = 0; 0 != s; count++) {
        s &= s-1;
    }
    return count;
}
