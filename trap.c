#include "trap.h"

extern void trap_vector(void);

void trap_init(void)
{
    mtvec_write((reg_t)trap_vector); // set the trap-vector base address for machine mode
}

reg_t trap_handler(reg_t epc, reg_t cause, struct context *context)
{
    reg_t pc, cause_code, interrupt_exception;
    int hartid;

    pc = epc;
    cause_code = cause & 0x7FFFFFFF;
    interrupt_exception = cause & 0x80000000;

    if(interrupt_exception) {
        // printf("Asynchronous trap: interrupt: code = %d\n", cause_code);

        switch(cause_code) {
            case MACHINE_SOFTWARE_INTERRUPT:
                hartid = mhartid_read();

                /* acknowledge a machine-level software interrupt */
                clint_write(CLINT_MSIP(hartid), 0);
                schedule();
                break;

            case MACHINE_TIMER_INTERRUPT:
                timer_handler();
                break;

            case MACHINE_EXTERNAL_INTERRUPT:
                plic_machine_external_interrupt();
                break;

            default:
                printf("unkown async trap!\n");
                break;
        }
    } else {
        // printf("Synchronous trap: exception: code = %d\n", cause_code);

        switch(cause_code) {
            case LOAD_ACCESS_FAULT:
                printf("Load access fault\n");
                while(1);
                break;
            case STORE_AMO_ACCESS_FAULT:
                printf("Store/AMO access fault\n");
                while(1);
                break;
            case SYSTEM_CALL_FROM_USER_LEVEL:
                do_syscall(context);
                pc += 4;
                break;
            default:
                printf("unkown sync trap!\n");
                while(1);
                break;
        }
    }

    return pc;
}
