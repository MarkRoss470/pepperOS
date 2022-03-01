#ifndef TIMER_H
#define TIMER_H

#include <util/IO.h>
#include <util/types.h>

// number chosen to be integer divisor of PIC frequency
#define TIMER_TPS 57

u64 timer_get();
void timer_init();

#endif
