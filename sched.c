#include "sched.h"

extern void switch_to(struct context *next);

static uint8_t task_stack[MAX_TASKS][STACK_SIZE];
static struct context task_context[MAX_TASKS];

static int active_tasks = 0;
static int current_task = -1;

void task_init(void)
{
    mscratch_write(0); // initialize mscratch to 0
}

/*
 * create a task
 */
int task_create(void (*task)(void))
{
    if(active_tasks < MAX_TASKS)
    {
        task_context[active_tasks].sp = (reg_t)&task_stack[active_tasks][STACK_SIZE - 1];
        task_context[active_tasks].ra = (reg_t)task;
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
    schedule();
}