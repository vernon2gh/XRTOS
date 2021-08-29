#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#ifdef CONFIG_ARCH64
#define MSTORE          sd
#define MLOAD           ld
#define CTX(offset)     (8 * offset)
#define BYTES           .dword
#elif CONFIG_ARCH32
#define MSTORE          sw
#define MLOAD           lw
#define CTX(offset)     (4 * offset)
#define BYTES           .word
#endif

#endif
