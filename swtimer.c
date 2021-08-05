#include "swtimer.h"

static struct swtimer swtimer_list[MAX_SWTIMER];

void swtimer_init(void)
{
    struct swtimer *tmp;
    int i;

    tmp = swtimer_list;
    for(i=0; i<MAX_SWTIMER; i++) {
        tmp->func = NULL;
        tmp->arg = 0;
        tmp->timeout = 0;

        tmp++;
    }
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
    struct swtimer *tmp;
    int i;

    tmp = swtimer_list;
    for(i=0; i<MAX_SWTIMER; i++) {
        if(tmp->func == NULL) {
            tmp->func = func;
            tmp->arg = arg;
            tmp->timeout = system_ticks + timeout;
            return tmp;
        }

        tmp++;
    }

    return NULL;
}

void swtimer_delete(struct swtimer *swtimer)
{
    struct swtimer *tmp;
    int i;

    tmp = swtimer_list;
    for(i=0; i<MAX_SWTIMER; i++) {
        if(tmp == swtimer) {
            tmp->func = NULL;
            tmp->arg = 0;
            tmp->timeout = 0;
            break;
        }

        tmp++;
    }
}

void swtimer_check(void)
{
    struct swtimer *tmp;
    int i;

    tmp = swtimer_list;
    for(i=0; i<MAX_SWTIMER; i++) {
        if((system_ticks >= tmp->timeout)&&(tmp->func)) {
            tmp->func(tmp->arg);

            tmp->func = NULL;
            tmp->arg = 0;
            tmp->timeout = 0;

            break;
        }

        tmp++;
    }
}