#include "syscall.h"

static int sys_gethartid(int *hartid)
{
    if(hartid == NULL) {
        return -1;
    }
    else {
        *hartid = mhartid_read();
        return 0;
    }
}

void do_syscall(struct context *context)
{
    uint32_t syscall_number = context->a7;

    switch(syscall_number) {
        case SYS_GETHARTID:
            context->a0 = sys_gethartid((int *)(context->a0));
            break;
        default:
            printf("Unknown syscall number: %d\n", syscall_number);
            context->a0 = -1;
    }
}