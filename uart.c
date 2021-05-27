#include "uart.h"

#define UART0 0x10000000 // UART0 base address

#define RHR 0 // Receive Holding Register (read mode)
#define THR 0 // Transmit Holding Register (write mode)
#define IER 1 // Interrupt Enable Register (write mode)

#define DLL 0 // LSB of Divisor Latch (write mode)
#define DLM 1 // MSB of Divisor Latch (write mode)

#define FCR 2 // FIFO Control Register (write mode)
#define ISR 2 // Interrupt Status Register (read mode)

#define LCR 3 // Line Control Register
#define MCR 4 // Modem Control Register
#define LSR 5 // Line Status Register
#define MSR 6 // Modem Status Register
#define SPR 7 // ScratchPad Register

#define uart_read(reg)          (*((volatile uint8_t *)(UART0 + reg)))
#define uart_write(reg, val)    (*((volatile uint8_t *)(UART0 + reg)) = (val))

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
}

void uart_putc(char ch)
{
    while(!(uart_read(LSR)&(0x01<<5))); // Whether the polling char `ch` is sent
    uart_write(THR, ch);                // send char `ch`
}

void uart_puts(char *s)
{
    while(*s) {
        uart_putc(*s++);
    }
}