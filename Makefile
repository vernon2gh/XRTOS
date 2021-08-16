archbits = 32

CROSS_COMPILE = riscv$(archbits)-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -mcmodel=medany -g -Wall -D CONFIG_ARCH$(archbits) -D CONFIG_USER_MODE

QEMU = qemu-system-riscv$(archbits)
QFLAGS = -nographic -smp 1 -machine virt -bios none

CC = ${CROSS_COMPILE}gcc
GDB = ${CROSS_COMPILE}gdb
OBJDUMP = ${CROSS_COMPILE}objdump

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

IMAGE = xos
OBJS = start.o \
		main.o \
		uart.o \
		print.o \
		pages.o \
		bytes.o \
		sched.o \
		trap.o \
		plic.o \
		clint.o \
		lock.o \
		swtimer.o \
		syscall.o \
		user_syscall.o

${IMAGE} : ${OBJS}
	${CC} $(CFLAGS) -T ${IMAGE}.ld -o $@ $^

run : ${IMAGE}
	@${QEMU} ${QFLAGS} -kernel $<

debug : ${IMAGE}
	@${QEMU} ${QFLAGS} -kernel $< -s -S &
	@${GDB} $< -q -x ./gdbinit

code : ${IMAGE}
	@${OBJDUMP} -S $< | less

clean :
	rm -rf ${IMAGE} *.o
