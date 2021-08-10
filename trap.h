#ifndef __TRAP_H__
#define __TRAP_H__

#include "types.h"
#include "riscv.h"
#include "print.h"
#include "plic.h"
#include "sched.h"
#include "clint.h"
#include "syscall.h"

#define MACHINE_SOFTWARE_INTERRUPT  3
#define MACHINE_TIMER_INTERRUPT     7
#define MACHINE_EXTERNAL_INTERRUPT  11

#define LOAD_ACCESS_FAULT           5
#define STORE_AMO_ACCESS_FAULT      7
#define SYSTEM_CALL_FROM_USER_LEVEL 8

void trap_init(void);

#endif
