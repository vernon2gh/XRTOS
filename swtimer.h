#ifndef __SWTIMER_H__
#define __SWTIMER_H__

#include "clint.h"

/*
 * software timer
 */
struct swtimer {
    void (*func)(uint32_t);
    uint32_t arg;
    uint32_t timeout_ticks;
};

#define MAX_SWTIMER 10

#define second_to_ticks(second) (second * SYSTEM_TICKS)
#define ticks_to_second(ticks)  (ticks / SYSTEM_TICKS)

void swtimer_init(void);
struct swtimer *swtimer_create(void (*func)(uint32_t), uint32_t arg, uint32_t timeout_ticks);
void swtimer_delete(struct swtimer *swtimer);
void swtimer_check(void);

#endif