#include "swtimer.h"
#include "bytes.h"

static struct swtimer *swtimer_head; // alway point first software timer

void swtimer_init(void)
{
    swtimer_head = NULL;
}

static void swtimer_add(struct swtimer *new, struct swtimer *prev, struct swtimer *next)
{
    new->prev = prev;
    new->next = next;
    prev->next = new;
    next->prev = new;
}


/*
 * create a software timer
 *
 * Parameter:
 * func     -> Pointer of software timer function
 * arg      -> Parameter of software timer function
 * timeout  -> timeout time, unit: tick
 *
 * Return:
 * If the software timer is created successfully, software timer pointer is returned;
 * otherwise, NULL is returned
 */
struct swtimer *swtimer_create(void (*func)(void *), void *arg, uint32_t timeout)
{
    struct swtimer *swtimer, *tmp;

    swtimer = byte_alloc(sizeof(struct swtimer));

    if(swtimer_head == NULL) {
        swtimer->next = swtimer;
        swtimer->prev = swtimer;
        swtimer_head = swtimer;
    } else {
        tmp = swtimer_head;

        do {
            if(swtimer_head->timeout > timeout) {
                swtimer_head = swtimer;
                break;
            }
            if(swtimer_head->prev->timeout < timeout) {
                break;
            }

            tmp = tmp->next;
        } while(!((tmp->prev->timeout < timeout)&&(timeout < tmp->timeout)));

        swtimer_add(swtimer, tmp->prev, tmp);
    }

    swtimer->func = func;
    swtimer->arg = arg;
    swtimer->timeout = system_ticks + timeout;

    return swtimer;
}

int swtimer_delete(struct swtimer *swtimer)
{
    struct swtimer *tmp;

    tmp = swtimer;
    if(tmp == NULL)
        return -1;

    if(tmp == swtimer_head)
        swtimer_head = tmp->next;

    if(tmp == tmp->next)
        swtimer_head = NULL;

    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    tmp->prev = NULL;
    tmp->next = NULL;
    byte_free(tmp, sizeof(struct swtimer));

    return 0;
}

/*
 * check software timer whether timeout
 *
 * Because the software timer is sorted by timeout from small to large,
 * system_ticks is also linearly increased,
 * so just check if the first software timer times out
 */
int swtimer_check(void)
{
    struct swtimer *tmp;

    tmp = swtimer_head;
    if(tmp == NULL)
        return -1;

    if(system_ticks >= tmp->timeout) {
        tmp->func(tmp->arg);
        swtimer_delete(tmp);
    }

    return 0;
}