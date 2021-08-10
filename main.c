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

#include "user_syscall.h"

void delay(volatile int count)
{
    count *= 50000;
    while (count--);
}

void swtimer_func(void *arg)
{
    printf("swtimer%d running...\n", (int)arg);
}

void kernel_task0(void *param)
{
    int tmp;

    int_lock();
    for(tmp=0; tmp<5; tmp++) {
        printf("kernel_task0 lock: %d\n", tmp);
        delay(4000);
    }
    int_unlock();

    printf("kernel_task0 yield...\n");
    task_yield();
    printf("kernel_task0 back...\n");

    swtimer_create(swtimer_func, (void *)3, second_to_ticks(1));

    while (1) {
        printf("kernel_task0 running...\n");

        //*(int *)0x00000000 = 0xFF; // trap
        //tmp = *(int *)0x00000000;  //

        delay(2000);
    }
}

void kernel_task1(void *param)
{
    int tmp;

    int_lock();
    for(tmp=0; tmp<5; tmp++) {
        printf("kernel_task1 lock: %d\n", tmp);
        delay(4000);
    }
    int_unlock();

    while (1) {
        printf("kernel_task1 running...\n");
        delay(2000);
    }
}

void user_task0(void *param)
{
    int tmp;
    int hartid;

    printf("%s: param = %p\n", __func__, param);

    /*
     * Can't read mhardid from User-mode level,
     * If want to read mhardid, by syscall only.
     */
    // hartid = mhartid_read();
    // printf("hart id is %d\n", hartid);

    tmp = get_hartid(&hartid);
    if(!tmp) {
        printf("hard id = %d\n", hartid);
    }
    else {
        printf("get_hardid failed! Error Value = %d\n", tmp);
    }

    while (1) {
        printf("%s: running...\n", __func__);
        task_delay(second_to_ticks(1));
        task_exit();
    }
}

void user_task1(void *param)
{
    printf("%s: param = %p\n", __func__, param);

    while (1) {
        printf("%s: running...\n", __func__);
        task_delay(second_to_ticks(1));
    }
}

void start_kernel(void)
{
    uint32_t *tmp;
    struct swtimer *swtimer;

    uart_init();
    uart_puts("hello world, XOS.\n");
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
    swtimer = swtimer_create(swtimer_func, (void *)1, second_to_ticks(1));
    if(swtimer == NULL)
        printf("software timer overout...\n");
    swtimer = swtimer_create(swtimer_func, (void *)2, second_to_ticks(5));
    if(swtimer == NULL)
        printf("software timer overout...\n");

    task_init();
#ifdef CONFIG_USER_MODE
    task_create(user_task0, (void *)0x00, 255, second_to_ticks(1));
    task_create(user_task1, (void *)0x01, 0, second_to_ticks(2));
#else
    task_create(kernel_task0, NULL, 255, second_to_ticks(1));
    task_create(kernel_task1, NULL, 255, second_to_ticks(1));
#endif
    schedule();

    while(1);
}
