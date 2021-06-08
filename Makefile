CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

CC = ${CROSS_COMPILE}gcc
GDB = ${CROSS_COMPILE}gdb
OBJDUMP = ${CROSS_COMPILE}objdump

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

IMAGE = xrtos
OBJS = start.o main.o uart.o pages.o

${IMAGE} : ${OBJS}
	${CC} $(CFLAGS) -T xrtos.ld -o $@ $^

run : ${IMAGE}
	@${QEMU} ${QFLAGS} -kernel $<

debug : ${IMAGE}
	@${QEMU} ${QFLAGS} -kernel $< -s -S &
	@${GDB} $< -q -x ./gdbinit

code : ${IMAGE}
	@${OBJDUMP} -S $< | less

clean :
	rm -rf ${IMAGE} *.o
