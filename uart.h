#ifndef __UART_H__
#define __UART_H__

#include "types.h"

#define UART0 0x10000000 // UART0 base address
#define UART0_IRQ 10     // UART0 interrupt ID

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

#define IER_RX_INT  (1 << 0)
#define IER_TX_INT  (1 << 1)

#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

#define uart_read(reg)          (*((volatile uint8_t *)(UART0 + reg)))
#define uart_write(reg, val)    (*((volatile uint8_t *)(UART0 + reg)) = (val))

void uart_init();
void uart_puts(char *s);
void uart_isr(void);

#endif