#include "timer.h"

static void timer_interval(uint64_t interval)
{
    int hartid;
    uint64_t tmp;

    hartid = mhartid_read();

    tmp = timer_read(CLINT_MTIME);
    timer_write(CLINT_MTIMECMP(hartid), (tmp + interval));
}

void timer_init(void)
{
    /* set interval for next timer interrupt */
    timer_interval(TIMER_INTERVAL);

    /* enable machine-mode timer interrupts */
    mie_write(mie_read() | MIE_MTIE);

    /* enable machine-mode global interrupts */
    mstatus_write(mstatus_read() | MSTATUS_MIE);
}

void timer_handler(void)
{
    static uint32_t ticks = 0;

    printf("ticks: %d\n", ++ticks);

    /* set interval for next timer interrupt */
    timer_interval(TIMER_INTERVAL);
}