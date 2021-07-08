#include "trap.h"

extern void trap_vector(void);

void trap_init(void)
{
    mtvec_write((reg_t)trap_vector); // set the trap-vector base address for machine mode
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t pc, cause_code, interrupt_exception;

    pc = epc;
    cause_code = cause & 0x7FFFFFFF;
    interrupt_exception = cause & 0x80000000;

    if(interrupt_exception) {
        printf("Asynchronous trap: interrupt: code = %d\n", cause_code);

        switch(cause_code) {
            case 3:
                printf("Machine software interrupt\n");
                break;
            case 7:
                printf("Machine timer interrupt\n");
                break;
            case 11:
                printf("Machine external interrupt\n");
                plic_machine_external_interrupt();
                break;
            default:
                printf("unkown async trap!\n");
                break;
        }
    } else {
        printf("Synchronous trap: exception: code = %d\n", cause_code);

        switch(cause_code) {
            case 5:
                printf("Load access fault\n");
                break;
            case 7:
                printf("Store/AMO access fault\n");
                break;
            default:
                printf("unkown sync trap!\n");
                break;
        }

        //pc += 4;
        while(1);
    }

    return pc;
}
