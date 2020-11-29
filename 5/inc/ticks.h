#ifndef __TICKS_H__
#define __TICKS_H__

#include <stdint.h>

extern __inline__ uint64_t curr_ticks() {
    uint64_t x;
    __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
    return x;
}

#endif // __TICKS_H__
