
#define UTILS_C
#include <utils.h>
#undef UTILS_C

// Works well when very few bits are set
int popcnt(size_t s) {
    int count;
    for(count = 0; 0 != s; count++) {
        s &= s-1;
    }
    return count;
}
