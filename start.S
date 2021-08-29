#include "instruction.h"

    .equ STACK_SIZE, 1024

    .global _start
    .global switch_to
    .global trap_vector

    # save all General-Purpose(GP) registers to task_context[]
    .macro reg_save base
    MSTORE ra, CTX(1)(\base)
    MSTORE sp, CTX(2)(\base)
    MSTORE gp, CTX(3)(\base)
    MSTORE tp, CTX(4)(\base)
    MSTORE t0, CTX(5)(\base)
    MSTORE t1, CTX(6)(\base)
    MSTORE t2, CTX(7)(\base)
    MSTORE s0, CTX(8)(\base)
    MSTORE s1, CTX(9)(\base)
    MSTORE a0, CTX(10)(\base)
    MSTORE a1, CTX(11)(\base)
    MSTORE a2, CTX(12)(\base)
    MSTORE a3, CTX(13)(\base)
    MSTORE a4, CTX(14)(\base)
    MSTORE a5, CTX(15)(\base)
    MSTORE a6, CTX(16)(\base)
    MSTORE a7, CTX(17)(\base)
    MSTORE s2, CTX(18)(\base)
    MSTORE s3, CTX(19)(\base)
    MSTORE s4, CTX(20)(\base)
    MSTORE s5, CTX(21)(\base)
    MSTORE s6, CTX(22)(\base)
    MSTORE s7, CTX(23)(\base)
    MSTORE s8, CTX(24)(\base)
    MSTORE s9, CTX(25)(\base)
    MSTORE s10, CTX(26)(\base)
    MSTORE s11, CTX(27)(\base)
    MSTORE t3, CTX(28)(\base)
    MSTORE t4, CTX(29)(\base)
    MSTORE t5, CTX(30)(\base)
    MSTORE t6, CTX(31)(\base)
    .endm

    # restore all General-Purpose(GP) registers from task_context[]
    .macro reg_restore base
    MLOAD ra, CTX(1)(\base)
    MLOAD sp, CTX(2)(\base)
    MLOAD gp, CTX(3)(\base)
    MLOAD tp, CTX(4)(\base)
    MLOAD t0, CTX(5)(\base)
    MLOAD t1, CTX(6)(\base)
    MLOAD t2, CTX(7)(\base)
    MLOAD s0, CTX(8)(\base)
    MLOAD s1, CTX(9)(\base)
    MLOAD a0, CTX(10)(\base)
    MLOAD a1, CTX(11)(\base)
    MLOAD a2, CTX(12)(\base)
    MLOAD a3, CTX(13)(\base)
    MLOAD a4, CTX(14)(\base)
    MLOAD a5, CTX(15)(\base)
    MLOAD a6, CTX(16)(\base)
    MLOAD a7, CTX(17)(\base)
    MLOAD s2, CTX(18)(\base)
    MLOAD s3, CTX(19)(\base)
    MLOAD s4, CTX(20)(\base)
    MLOAD s5, CTX(21)(\base)
    MLOAD s6, CTX(22)(\base)
    MLOAD s7, CTX(23)(\base)
    MLOAD s8, CTX(24)(\base)
    MLOAD s9, CTX(25)(\base)
    MLOAD s10, CTX(26)(\base)
    MLOAD s11, CTX(27)(\base)
    MLOAD t3, CTX(28)(\base)
    MLOAD t4, CTX(29)(\base)
    MLOAD t5, CTX(30)(\base)
    MLOAD t6, CTX(31)(\base)
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

    MLOAD t6, CTX(0)(a0)    # restore mepc from the context of the next task
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
    MSTORE a0, CTX(0)(t6)   #

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
