    .equ STACK_SIZE, 1024

    .global _start
    .global switch_to

    # save all General-Purpose(GP) registers to task_context[]
    .macro reg_save base
    sw ra, 0(\base)
    sw sp, 4(\base)
    sw gp, 8(\base)
    sw tp, 12(\base)
    sw t0, 16(\base)
    sw t1, 20(\base)
    sw t2, 24(\base)
    sw s0, 28(\base)
    sw s1, 32(\base)
    sw a0, 36(\base)
    sw a1, 40(\base)
    sw a2, 44(\base)
    sw a3, 48(\base)
    sw a4, 52(\base)
    sw a5, 56(\base)
    sw a6, 60(\base)
    sw a7, 64(\base)
    sw s2, 68(\base)
    sw s3, 72(\base)
    sw s4, 76(\base)
    sw s5, 80(\base)
    sw s6, 84(\base)
    sw s7, 88(\base)
    sw s8, 92(\base)
    sw s9, 96(\base)
    sw s10, 100(\base)
    sw s11, 104(\base)
    sw t3, 108(\base)
    sw t4, 112(\base)
    sw t5, 116(\base)
    sw t6, 120(\base)
    .endm

    # restore all General-Purpose(GP) registers from task_context[]
    .macro reg_restore base
    lw ra, 0(\base)
    lw sp, 4(\base)
    lw gp, 8(\base)
    lw tp, 12(\base)
    lw t0, 16(\base)
    lw t1, 20(\base)
    lw t2, 24(\base)
    lw s0, 28(\base)
    lw s1, 32(\base)
    lw a0, 36(\base)
    lw a1, 40(\base)
    lw a2, 44(\base)
    lw a3, 48(\base)
    lw a4, 52(\base)
    lw a5, 56(\base)
    lw a6, 60(\base)
    lw a7, 64(\base)
    lw s2, 68(\base)
    lw s3, 72(\base)
    lw s4, 76(\base)
    lw s5, 80(\base)
    lw s6, 84(\base)
    lw s7, 88(\base)
    lw s8, 92(\base)
    lw s9, 96(\base)
    lw s10, 100(\base)
    lw s11, 104(\base)
    lw t3, 108(\base)
    lw t4, 112(\base)
    lw t5, 116(\base)
    lw t6, 120(\base)
    .endm

    .text
_start:
    la  sp, stacks + STACK_SIZE # setup stack pointer
    csrw mscratch, 0            # initialize mscratch to 0
    j   start_kernel            # entry C programme environment

switch_to:
    csrrw t6, mscratch, t6  # save the previous task context pointer to t6
    beqz t6, restore    	# if t6 == 0, now this is first task, without previous task,
                        	# so no need to save previous task context, jump to label restore
    reg_save t6         	# save the previous task context

restore:
    csrw mscratch, a0   	# restore mscratch from the next task context pointer
    mv t6, a0
    reg_restore t6      	# restore the next task context
    ret                 	# running next task

    # Read-only area
    # Store start and end address of each section.
    .section .rodata
    .global heap_start
    .global heap_end

    heap_start: .word _heap_start
    heap_end: .word _heap_end

stacks:                         # stack size is 1024B
    .skip STACK_SIZE

    .end
