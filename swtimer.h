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
};

#define MAX_SWTIMER 10

#define second_to_ticks(second) (second * SYSTEM_TICKS)
#define ticks_to_second(ticks)  (ticks / SYSTEM_TICKS)

void swtimer_init(void);
struct swtimer *swtimer_create(void (*func)(void *), void *arg, uint32_t timeout);
void swtimer_delete(struct swtimer *swtimer);
void swtimer_check(void);

#endif