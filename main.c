#include "uart.h"
#include "print.h"
#include "pages.h"
#include "bytes.h"
#include "sched.h"
#include "trap.h"
#include "plic.h"
#include "clint.h"
#include "lock.h"
#include "swtimer.h"

void delay(volatile int count)
{
    count *= 50000;
    while (count--);
}

void swtimer_func(uint32_t arg)
{
    printf("swtimer%d running...\n", arg);
}

void task0(void)
{
    int tmp;

    int_lock();
    for(tmp=0; tmp<5; tmp++) {
        printf("task0 lock: %d\n", tmp);
        delay(4000);
    }
    int_unlock();

    printf("task0 yield...\n");
    task_yield();
    printf("task0 back...\n");

    swtimer_create(swtimer_func, 3, second_to_ticks(1));

    while (1) {
        printf("task0 running...\n");

        //*(int *)0x00000000 = 0xFF; // trap
        //tmp = *(int *)0x00000000;  //

        delay(2000);
    }
}

void task1(void)
{
    int tmp;

    int_lock();
    for(tmp=0; tmp<5; tmp++) {
        printf("task1 lock: %d\n", tmp);
        delay(4000);
    }
    int_unlock();

    while (1) {
        printf("task1 running...\n");
        delay(2000);
    }
}

void start_kernel(void)
{
    uint32_t *tmp;
    struct swtimer *swtimer;

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

    trap_init();
    plic_init();
    timer_init();

    swtimer_init();
    swtimer = swtimer_create(swtimer_func, 1, second_to_ticks(1));
    if(swtimer == NULL)
        printf("software timer overout...\n");
    swtimer = swtimer_create(swtimer_func, 2, second_to_ticks(5));
    if(swtimer == NULL)
        printf("software timer overout...\n");

    task_init();
    task_create(task0);
    task_create(task1);
    schedule();

    while(1);
}
