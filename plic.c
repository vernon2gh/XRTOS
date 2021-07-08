#include "plic.h"

void plic_init(void)
{
    int hartid;

    hartid = mhartid_read();

    /* Set priority for UART0 */
    plic_write(PLIC_PRIORITY(UART0_IRQ), 1);

    /* Enable UART0 */
    plic_write(PLIC_MENABLE(hartid), (1 << UART0_IRQ));

    /* Set priority threshold for UART0 */
    plic_write(PLIC_MTHRESHOLD(hartid), 0);

    /* enable machine-mode external interrupts */
    mie_write(mie_read() | MIE_MEIE);

    /* enable machine-mode global interrupts */
    mstatus_write(mstatus_read() | MSTATUS_MIE);
}

/*
 * The PLIC can perform an interrupt claim by reading the claim/complete register,
 * which returns the ID of the highest priority pending interrupt
 * or zero if there is no pending interrupt.
 *
 * A successful claim will also atomically clear the corresponding pending bit
 * on the interrupt source
 */
static uint32_t plic_claim(void)
{
    int hartid;
    uint32_t irq;

    hartid = mhartid_read();
    irq = plic_read(PLIC_MCLAIM(hartid));

    return irq;
}

/*
 * The PLIC signals it has completed executing an interrupt handler
 * by writing the interrupt ID it received from the claim to the claim/complete register.
 *
 * The PLIC does not check whether the completion ID is the same as the last claim ID for that target.
 * If the completion ID does not match an interrupt source that is currently enabled for the target,
 * the completion is silently ignored
 */
static void plic_completion(uint32_t irq)
{
    int hartid;

    hartid = mhartid_read();
    plic_write(PLIC_MCOMPLETE(hartid), irq);
}

void plic_machine_external_interrupt(void)
{
    uint32_t irq;

    irq = plic_claim();

    if(irq == UART0_IRQ)
        uart_isr();
    else if(irq)
        printf("unexpected: irq = %d\n", irq);

    plic_completion(irq);
}
