#include "instruction.h"

    .equ STACK_SIZE, 1024

    .global _start
    .global switch_to
    .global trap_vector

    # save all General-Purpose(GP) registers to task_context[]
    .macro reg_save base
    MS ra, CTX(1)(\base)
    MS sp, CTX(2)(\base)
    MS gp, CTX(3)(\base)
    MS tp, CTX(4)(\base)
    MS t0, CTX(5)(\base)
    MS t1, CTX(6)(\base)
    MS t2, CTX(7)(\base)
    MS s0, CTX(8)(\base)
    MS s1, CTX(9)(\base)
    MS a0, CTX(10)(\base)
    MS a1, CTX(11)(\base)
    MS a2, CTX(12)(\base)
    MS a3, CTX(13)(\base)
    MS a4, CTX(14)(\base)
    MS a5, CTX(15)(\base)
    MS a6, CTX(16)(\base)
    MS a7, CTX(17)(\base)
    MS s2, CTX(18)(\base)
    MS s3, CTX(19)(\base)
    MS s4, CTX(20)(\base)
    MS s5, CTX(21)(\base)
    MS s6, CTX(22)(\base)
    MS s7, CTX(23)(\base)
    MS s8, CTX(24)(\base)
    MS s9, CTX(25)(\base)
    MS s10, CTX(26)(\base)
    MS s11, CTX(27)(\base)
    MS t3, CTX(28)(\base)
    MS t4, CTX(29)(\base)
    MS t5, CTX(30)(\base)
    MS t6, CTX(31)(\base)
    .endm

    # restore all General-Purpose(GP) registers from task_context[]
    .macro reg_restore base
    ML ra, CTX(1)(\base)
    ML sp, CTX(2)(\base)
    ML gp, CTX(3)(\base)
    ML tp, CTX(4)(\base)
    ML t0, CTX(5)(\base)
    ML t1, CTX(6)(\base)
    ML t2, CTX(7)(\base)
    ML s0, CTX(8)(\base)
    ML s1, CTX(9)(\base)
    ML a0, CTX(10)(\base)
    ML a1, CTX(11)(\base)
    ML a2, CTX(12)(\base)
    ML a3, CTX(13)(\base)
    ML a4, CTX(14)(\base)
    ML a5, CTX(15)(\base)
    ML a6, CTX(16)(\base)
    ML a7, CTX(17)(\base)
    ML s2, CTX(18)(\base)
    ML s3, CTX(19)(\base)
    ML s4, CTX(20)(\base)
    ML s5, CTX(21)(\base)
    ML s6, CTX(22)(\base)
    ML s7, CTX(23)(\base)
    ML s8, CTX(24)(\base)
    ML s9, CTX(25)(\base)
    ML s10, CTX(26)(\base)
    ML s11, CTX(27)(\base)
    ML t3, CTX(28)(\base)
    ML t4, CTX(29)(\base)
    ML t5, CTX(30)(\base)
    ML t6, CTX(31)(\base)
    .endm

    .text
_start:
    la  sp, stacks + STACK_SIZE # setup stack pointer

    # to parepare the mstatus here for the first task
    # default mstatus is 0
#ifdef CONFIG_USER_MODE
    # mstatus.MPP = 0, run in User-mode after first MRET operating
    # mstatus.MPIE = 1, enable global interrupt after first MRET operating
    li t0, 1 << 7
#else
    # mstatus.MPP = 3, run in Machine-mode after first MRET operating
    # mstatus.MPIE = 1, enable global interrupt after first MRET operating
    li t0, 3 << 11 | 1 << 7
#endif
    csrr t1, mstatus
    or t0, t0, t1
    csrw mstatus, t0

    j   start_kernel        # entry C programme environment

switch_to:
    csrw mscratch, a0       # restore mscratch from the next task context pointer

    ML t6, CTX(0)(a0)       # restore mepc from the context of the next task
    csrw mepc, t6           #
    mv t6, a0
    reg_restore t6          # restore the next task context

    mret                    # running next task in mstatus.MPP level
                            # and restore mstatus.MPIE to mstatus.MIE

    .align 4                # the trap-vector base address must always be aligned on 4 byte boundary
trap_vector:
    csrr t6, mscratch       # save context
    reg_save t6             #

    csrr a0, mepc           # save mepc to the context of the current task
    MS a0, CTX(0)(t6)       #

    csrr a0, mepc           # args 1, a0 = mepc
    csrr a1, mcause         # args 2, a1 = mcause
    csrr a2, mscratch       # args 3, a2 = mscratch
    call trap_handler       # enter the trap handler function on .c soure code
    csrw mepc, a0           # return value a0, and than a0 copy to mepc

    csrr t6, mscratch       # restore context
    reg_restore t6          #

    mret                    # return to whatever we were doing before trap in mstatus.MPP level
                            # and restore mstatus.MPIE to mstatus.MIE

    # Read-only area
    # Store start and end address of each section.
    .section .rodata
    .global heap_start
    .global heap_end

    heap_start: BYTES _heap_start
    heap_end: BYTES _heap_end

stacks:                         # stack size is 1024B
    .skip STACK_SIZE

    .end
