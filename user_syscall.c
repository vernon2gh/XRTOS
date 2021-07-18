#include "user_syscall.h"

/*
 * read hart id
 *
 * parameter:
 * a0 -> hart id pointer
 * a7 -> system call number
 *
 * return:
 * a0 -> Successful return 0 or Fail return Error-Value
 */
int get_hartid(int *hartid)
{
    int tmp;

    asm volatile("li a7, %0" : : "i"(SYS_GETHARTID));
    asm volatile("mv a0, %0" : : "r"(hartid) );
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(tmp) );

    return tmp;
}