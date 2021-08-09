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
    uint32_t __timeout;

    swtimer = byte_alloc(sizeof(struct swtimer));
    __timeout = timeout + system_ticks;

    if(swtimer_head == NULL) {
        swtimer->next = swtimer;
        swtimer->prev = swtimer;
        swtimer_head = swtimer;
    } else {
        tmp = swtimer_head;

        do {
            if(swtimer_head->timeout >= __timeout) {
                swtimer_head = swtimer;
                break;
            }
            if(swtimer_head->prev->timeout <= __timeout) {
                break;
            }

            tmp = tmp->next;
        } while(!((tmp->prev->timeout <= __timeout)&&(__timeout <= tmp->timeout)));

        swtimer_add(swtimer, tmp->prev, tmp);
    }

    swtimer->func = func;
    swtimer->arg = arg;
    swtimer->timeout = __timeout;

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
 * or other timeout time is the same as the first software timer
 */
int swtimer_check(void)
{
    struct swtimer *next, *tmp;

    next = swtimer_head;
    if(next == NULL)
        return -1;

    do {
        tmp = next;
        if(system_ticks >= tmp->timeout) {
            tmp->func(tmp->arg);
            next = tmp->next;
            swtimer_delete(tmp);
        }

        if((swtimer_head == NULL) || (tmp == next))
            break;
    } while(tmp->timeout == next->timeout);

    return 0;
}