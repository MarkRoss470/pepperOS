#include <idt.h>
#include <stdint.h>

struct __attribute__((__packed__)) IDTEntry{
    u16 offset_low;
    u16 selector;
    u8 __ignored;
    u8 type;
    u16 offset_high;
};

struct __attribute__((__packed__)) IDTPointer{
    u16 limit;
    uintptr_t base;
};

static struct {
    struct IDTEntry entries[256];
    struct IDTPointer pointer;
} idt;

// in start.S
extern void idt_load();

void idt_set(u8 index, void (*base)(struct Registers*), u16 selector, u8 flags) {
    idt.entries[index] = (struct IDTEntry) {
        .offset_low = ((uintptr_t) base) & 0xFFFF,
        .offset_high = (((uintptr_t) base) >> 16) & 0xFFFF,
        .selector = selector,
        .type = flags | 0x60,
        .__ignored = 0
    };
}

void idt_init() {
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t) &idt.entries[0];
    for(size_t i = 0; i < sizeof(idt.entries); i++)
    {
    	((uint8_t*)idt.entries)[i] = 0;
    }
    idt_load((uintptr_t) &idt.pointer);
}
