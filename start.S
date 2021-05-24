    .equ STACK_SIZE, 1024

    .global _start

    .text
_start:
    la  sp, stacks + STACK_SIZE # setup stack pointer
    j   start_kernel            # entry C programme environment

stacks:                         # stack size is 1024B
    .skip STACK_SIZE

    .end