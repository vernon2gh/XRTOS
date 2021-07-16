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
        tmp->timeout_ticks = 0;

        tmp++;
    }
}

struct swtimer *swtimer_create(void (*func)(uint32_t), uint32_t arg, uint32_t timeout_ticks)
{
    struct swtimer *tmp;
    int i;

    tmp = swtimer_list;
    for(i=0; i<MAX_SWTIMER; i++) {
        if(tmp->func == NULL) {
            tmp->func = func;
            tmp->arg = arg;
            tmp->timeout_ticks = system_ticks + timeout_ticks;
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
            tmp->timeout_ticks = 0;
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
        if((system_ticks >= tmp->timeout_ticks)&&(tmp->func)) {
            tmp->func(tmp->arg);

            tmp->func = NULL;
            tmp->arg = 0;
            tmp->timeout_ticks = 0;

            break;
        }

        tmp++;
    }
}