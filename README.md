This is XOS project, base qemu RISC-V virt32/virt64 machine, include function, as follow:

* Allocated pages managed by array
* Allocated bytes managed by list
* running task by FIFO scheduler, has priority, timeslice function.
    * Cooperative Multitasking from Machine software interrupt
    * Preemptive Multitasking from Machine timer/software interrupt
* idle task
* running trap
* external interrupt uart0 by PLIC device
* hardware timer
* software timer
* interrupt lock
* syscall from User-mode level