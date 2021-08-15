#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "riscv.h"
#include "print.h"
#include "clint.h"
#include "bytes.h"

#define STACK_SIZE  1024

#define TASK_FIRST      0x01
#define TASK_READY      0x02
#define TASK_RUNNING    0x04
#define TASK_DELETED    0x08
#define TASK_SLEEPING   0x10

struct context {
	reg_t epc;

	// reg_t zero; // it always is zero
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
};

struct task {
	struct context ctx;
	uint8_t stack[STACK_SIZE];
	uint8_t flag;
	uint8_t priority;
	uint32_t timeslice;
	uint32_t timeout;

	struct task *prev;
	struct task *next;
};

void task_init(void);
int task_create(void (*func)(void *), void *param, uint8_t priority, uint32_t timeslice);
void task_exit(void);
int schedule(void);
int task_delay(uint32_t time);
void task_yield(void);
void task_check_timeout(void);

#endif
