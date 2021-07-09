#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"
#include "riscv.h"
#include "print.h"

#define CLINT_BASE              0x2000000L
#define CLINT_MTIMECMP(hartid)  (0x4000 + 8 * hartid)
#define CLINT_MTIME             0xBFF8

#define CLINT_TIMEBASE_FREQ     10000000                            // timer frequency
#define SYSTEM_TICKS            1                                   // tick number per-second
#define TIMER_INTERVAL          (CLINT_TIMEBASE_FREQ/SYSTEM_TICKS)  // interval time per-tick

#define timer_read(reg)         (*(volatile uint64_t *)(CLINT_BASE + reg))
#define timer_write(reg, val)   (*(volatile uint64_t *)(CLINT_BASE + reg) = val)

void timer_init(void);
void timer_handler(void);

#endif
