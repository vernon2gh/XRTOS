#include "sched.h"
#include "swtimer.h"

extern void switch_to(struct context *next);

static struct task *current_task; // alway point current task

void task_init(void)
{
    /* enable machine-mode software interrupts */
    mie_write(mie_read() | MIE_MSIE);

    /* init current task */
    current_task = NULL;
}

static void task_add(struct task *new, struct task *prev, struct task *next)
{
    new->prev = prev;
    new->next = next;
    prev->next = new;
    next->prev = new;
}

static void task_delete(struct task *entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

/*
 * create a task
 *
 * Parameter:
 * func     -> Pointer of task function
 * param    -> Parameter of task function
 * priority -> Priority of task function,
 *             rang is [0, 255], 0 is highest priority level
 * timeslice-> task maximum running time, unit: tick
 *
 * Return:
 * If the task is created successfully, 0 is returned;
 * otherwise, -1 is returned
 */
int task_create(void (*func)(void *), void *param, uint8_t priority, uint32_t timeslice)
{
    struct task *task, *tmp;
    struct context *ctx;

    task = byte_alloc(sizeof(struct task));
    task->flag = TASK_READY;
    task->priority = priority;
    task->timeslice = timeslice;

    ctx = &task->ctx;
    ctx->sp = (reg_t)(task->stack + (STACK_SIZE - 1));
    ctx->epc = (reg_t)func;
    ctx->a0 = (reg_t)param;

    if(current_task == NULL) {
        task->next = task;
        task->prev = task;
        task->flag = TASK_FIRST;
        current_task = task;
    } else {
        tmp = current_task;

        do {
            if(current_task->priority >= priority) {
                current_task->flag = TASK_READY;
                task->flag = TASK_FIRST;
                current_task = task;
                break;
            }
            if(current_task->prev->priority <= priority) {
                break;
            }

            tmp = tmp->next;
        } while(!((tmp->prev->priority <= priority)&&(priority <= tmp->priority)));

        task_add(task, tmp->prev, tmp);
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
    struct task *task, *tmp;
    struct context *context;

    task = current_task;

    if(task->flag != TASK_FIRST) {
        tmp = task->next;
        while(tmp->flag == TASK_SLEEPING)
            tmp = tmp->next;

        current_task = tmp;
    }

    switch(task->flag) {
        case TASK_DELETED:
            task_delete(task);
            byte_free(task, sizeof(struct task));
            break;

        case TASK_SLEEPING:
            break;

        case TASK_FIRST:
        case TASK_READY:
        case TASK_RUNNING:
        default:
            task->flag = TASK_READY;
    }

    context = &current_task->ctx;
    current_task->flag = TASK_RUNNING;
    current_task->timeout = system_ticks + current_task->timeslice;
    switch_to(context);

    return 0;
}

static void task_wakeup(void *arg)
{
    struct task *task;

    task = (struct task*)arg;
    task->flag = TASK_READY;
}

/*
 * make task to sleep
 *
 * Parameter:
 * time -> task sleep time, unit: tick
 *
 * Return:
 * If successfully, 0 is returned;
 * otherwise, -1 is returned
 */
int task_delay(uint32_t time)
{
    struct task *task;

    task = current_task;
    task->flag = TASK_SLEEPING;

    swtimer_create(task_wakeup, (void *)task, time);
    task_yield();

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

/*
 * check task running time whether timeout
 */
void task_check_timeout(void)
{
    if(system_ticks >= current_task->timeout)
        schedule();
}