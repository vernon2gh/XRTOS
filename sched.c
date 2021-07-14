#include "sched.h"

extern void switch_to(struct context *next);

static uint8_t task_stack[MAX_TASKS][STACK_SIZE];
static struct context task_context[MAX_TASKS];

static int active_tasks = 0;
static int current_task = -1;

void task_init(void)
{
    /* enable machine-mode software interrupts */
    mie_write(mie_read() | MIE_MSIE);
}

/*
 * create a task
 */
int task_create(void (*task)(void))
{
    if(active_tasks < MAX_TASKS)
    {
        task_context[active_tasks].sp = (reg_t)&task_stack[active_tasks][STACK_SIZE - 1];
        task_context[active_tasks].epc = (reg_t)task;
        active_tasks++;

        return 0;
    }
    else
    {
        return -1;
    }
}

/*
 * running next task
 *
 * FIFO scheduler
 */
int schedule(void)
{
    struct context *next;

    if(active_tasks <= 0) {
        printf("There is no task to perform!\n");
        return -1;
    }

    current_task = (current_task + 1) % active_tasks;
    next = &task_context[current_task];
    switch_to(next);

    return 0;
}

void task_yield(void)
{
    int hartid;

    hartid = mhartid_read();

    /* trigger a machine-level software interrupt */
    clint_write(CLINT_MSIP(hartid), 1);
}