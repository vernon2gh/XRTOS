#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

static inline void mscratch_write(reg_t x)
{
    asm volatile("csrw mscratch, %0" : : "r"(x));
}

static inline void mtvec_write(reg_t x)
{
    asm volatile("csrw mtvec, %0" : : "r"(x));
}

#endif