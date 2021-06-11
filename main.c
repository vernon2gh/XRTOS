#include "uart.h"
#include "pages.h"
#include "bytes.h"

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

    byte_init();
    tmp = byte_alloc(4);
    *tmp = 0xaabbccdd;
    byte_free(tmp, 4);

    while(1);
}
