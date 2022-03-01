#include <fpu.h>

void fpu_init() {
    size_t t;

    __asm__("clts");
    __asm__("mov %%cr0, %0" : "=r"(t));
    t &= ~(1 << 2);
    t |= (1 << 1);
    __asm__("mov %0, %%cr0" :: "r"(t));
    __asm__("mov %%cr4, %0" : "=r"(t));
    t |= 3 << 9;
    __asm__("mov %0, %%cr4" :: "r"(t));
    __asm__("fninit");
}
