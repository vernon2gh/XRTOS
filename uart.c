#include "uart.h"

void uart_init()
{
    uint8_t tmp = 0;

    /* disable interrupts */
    uart_write(IER, 0x00);

    /*
     * divisor latch enable
     * baud rate 115200 when 1.8432MHz clock
     */
    tmp = uart_read(LCR);
    tmp |= (1<<7);
    uart_write(LCR, tmp);
    uart_write(DLL, 0x01);
    uart_write(DLM, 0x00);

    /*
     * word length bits : 8
     * stop bits        : 1
     * no parity
     * no break control
     * divisor latch disable
     */
    tmp = 0x03;
    uart_write(LCR, tmp);

    /* Enable interrupts */
    tmp = uart_read(IER);
    uart_write(IER, tmp | IER_RX_INT);
}

static void uart_putc(char ch)
{
    while(!(uart_read(LSR) & LSR_TX_IDLE)); // Whether the polling char `ch` is sent
    uart_write(THR, ch);                    // Transmit char `ch`
}

void uart_puts(char *s)
{
    while(*s) {
        uart_putc(*s++);
    }
}

static char uart_getc(void)
{
    char tmp;

    if(uart_read(LSR) & LSR_RX_READY)
        tmp = uart_read(RHR);   // Receive char
    else
        tmp = -1;

    return tmp;
}

void uart_isr(void)
{
    char ch;

    ch = uart_getc();

    uart_putc(ch);
    uart_putc('\n');
}