#include "sched.h"

extern void switch_to(struct context *next);

static struct task *current_task; // alway point current task pointer

void task_init(void)
{
    /* enable machine-mode software interrupts */
    mie_write(mie_read() | MIE_MSIE);

    /* init current task */
    current_task = NULL;
}

/*
 * create a task
 *
 * Parameter:
 * func     -> Pointer of task function
 *
 * Return:
 * If the task is created successfully, 0 is returned;
 * otherwise, -1 is returned
 */
int task_create(void (*func)(void))
{
    struct task *task;
    struct context *ctx;

    task = byte_alloc(sizeof(struct task));

    ctx = &task->ctx;
    ctx->sp = (reg_t)(task->stack + (STACK_SIZE - 1));
    ctx->epc = (reg_t)func;

    if(current_task == NULL) {
        task->next = task;
        task->prev = task;
        task->flag = TASK_FIRST;
        current_task = task;
    }
    else {
        task->next = current_task;
        task->prev = current_task->prev;
        current_task->prev->next = task;
        current_task->prev = task;
    }

    return 0;
}

/*
 * delete a task
 */
void task_exit(void)
{
    current_task->flag = TASK_DELETED;
    task_yield();
}

/*
 * running next task
 *
 * FIFO scheduler
 */
int schedule(void)
{
    struct task *task;
    struct context *context;

    switch(current_task->flag) {
        case TASK_FIRST:
            current_task->flag &= ~TASK_FIRST;
            break;

        case TASK_DELETED:
            task = current_task;
            current_task = task->next;

            task->prev->next = current_task;
            current_task->prev = task->prev;
            byte_free(task, sizeof(struct task));
            break;

        case TASK_RUNNING:
        default:
            task = current_task;
            current_task = task->next;
    }

    context = &current_task->ctx;
    current_task->flag |= TASK_RUNNING;
    switch_to(context);

    return 0;
}

void task_yield(void)
{
    int hartid;

#ifdef CONFIG_USER_MODE
    #include "user_syscall.h"
    get_hartid(&hartid);
#else
    hartid = mhartid_read();
#endif

    /* trigger a machine-level software interrupt */
    clint_write(CLINT_MSIP(hartid), 1);
}