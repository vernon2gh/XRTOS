#include "uart.h"

void start_kernel(void)
{
    uart_init();
    uart_puts("hello world, XRTOS.\n");

    while(1);
}