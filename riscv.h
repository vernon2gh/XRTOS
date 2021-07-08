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

static inline reg_t mhartid_read()
{
    reg_t x;

    asm volatile("csrr %0, mhartid" : "=r"(x) );

    return x;
}

/*
 * Machine interrupt-enable register (mie)
 */
#define MIE_MEIE (1 << 11) // external
#define MIE_MTIE (1 << 7)  // timer
#define MIE_MSIE (1 << 3)  // software

static inline reg_t mie_read()
{
    reg_t x;

    asm volatile("csrr %0, mie" : "=r"(x) );

    return x;
}

static inline void mie_write(reg_t x)
{
    asm volatile("csrw mie, %0" : : "r"(x));
}

/*
 * Machine-mode status register (mstatus)
 */
#define MSTATUS_MIE (1 << 3) // Machine-mode interrupt enable
#define MSTATUS_SIE (1 << 1) // Supervisor-mode interrupt enable
#define MSTATUS_UIE (1 << 0) // User-mode interrupt enable

static inline reg_t mstatus_read()
{
    reg_t x;

    asm volatile("csrr %0, mstatus" : "=r"(x) );

    return x;
}

static inline void mstatus_write(reg_t x)
{
    asm volatile("csrw mstatus, %0" : : "r"(x));
}

#endif