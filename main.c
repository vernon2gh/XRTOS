#include "uart.h"
#include "pages.h"

void start_kernel(void)
{
    uint32_t *tmp;

    uart_init();
    uart_puts("hello world, XRTOS.\n");

    page_init();
    tmp = page_alloc(1);
    *tmp = 0x12345678;
    *(tmp+1) = 0xaa;
    page_free(tmp);

    while(1);
}
