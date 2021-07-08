#ifndef __PLIC_H__
#define __PLIC_H__

#include "types.h"
#include "riscv.h"
#include "print.h"

#define PLIC_BASE               0x0C000000L
#define PLIC_PRIORITY(irq)      (irq * 4)
#define PLIC_PENDING(irq)       (0x1000 + irq / 32)
#define PLIC_MENABLE(hart)      (0x2000 + hart * 0x80)
#define PLIC_MTHRESHOLD(hart)   (0x200000 + hart * 0x1000)
#define PLIC_MCLAIM(hart)       (0x200004 + hart * 0x1000)
#define PLIC_MCOMPLETE(hart)    (0x200004 + hart * 0x1000)

#define plic_read(reg)          (*(volatile uint32_t *)(PLIC_BASE + reg))
#define plic_write(reg, val)    (*(volatile uint32_t *)(PLIC_BASE + reg) = val)

void plic_init(void);
void plic_machine_external_interrupt(void);

#endif
