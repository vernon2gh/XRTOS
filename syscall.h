#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "sched.h"

/*
 * system call number
 */
#define SYS_GETHARTID   1

void do_syscall(struct context *context);

#endif
