#ifndef __TRAP_H__
#define __TRAP_H__

#include "types.h"
#include "riscv.h"
#include "print.h"
#include "plic.h"
#include "sched.h"
#include "clint.h"
#include "syscall.h"

void trap_init(void);

#endif
