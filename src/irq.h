#ifndef IRQ_H
#define IRQ_H

#include "util/types.h"
#include "util/IO.h"
#include "util/mem.h"
#include "isr.h"

void irq_install(size_t i, void (*handler)(struct Registers*));
void irq_init();

#endif
