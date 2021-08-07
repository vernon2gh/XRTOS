#ifndef __SWTIMER_H__
#define __SWTIMER_H__

#include "clint.h"

/*
 * software timer
 */
struct swtimer {
    void (*func)(void *);
    void *arg;
    uint32_t timeout;

    struct swtimer *prev;
    struct swtimer *next;
};

#define second_to_ticks(second) (second * SYSTEM_TICKS)
#define ticks_to_second(ticks)  (ticks / SYSTEM_TICKS)

void swtimer_init(void);
struct swtimer *swtimer_create(void (*func)(void *), void *arg, uint32_t timeout);
int swtimer_delete(struct swtimer *swtimer);
int swtimer_check(void);

#endif