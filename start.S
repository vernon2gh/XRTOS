    .equ STACK_SIZE, 1024

    .global _start

    .text
_start:
    la  sp, stacks + STACK_SIZE # setup stack pointer
    j   start_kernel            # entry C programme environment

    /*
     * Read-only area
     * Store start and end address of each section.
     */
    .section .rodata
    .global heap_start
    .global heap_end

    heap_start: .word _heap_start
    heap_end: .word _heap_end

stacks:                         # stack size is 1024B
    .skip STACK_SIZE

    .end
