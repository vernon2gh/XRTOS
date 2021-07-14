#include "lock.h"

void int_lock(void)
{
    /* disable machine-mode global interrupts */
    mstatus_write(mstatus_read() & (~MSTATUS_MIE));
}

void int_unlock(void)
{
    /* enable machine-mode global interrupts */
    mstatus_write(mstatus_read() | MSTATUS_MIE);
}