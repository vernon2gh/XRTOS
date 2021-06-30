#include "uart.h"
#include "print.h"
#include "pages.h"
#include "bytes.h"

void start_kernel(void)
{
    uint32_t *tmp;

    uart_init();
    uart_puts("hello world, XRTOS.\n");
    printf("hello word, printf\n");

    page_init();
    tmp = page_alloc(1);
    *tmp = 0x12345678;
    *(tmp+1) = 0xaa;
    printf("page: %p = 0x%x\n", tmp, *tmp);
    printf("page: %p = 0x%x\n", tmp+1, *(tmp+1));
    page_free(tmp);

    byte_init();
    tmp = byte_alloc(4);
    *tmp = 0xaabbccdd;
    printf("byte: %p = 0x%x\n", tmp, *tmp);
    byte_free(tmp, 4);

    while(1);
}
