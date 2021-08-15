#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdbool.h>

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

/*
 * Architecture-dependent
 *
 * if use 64bits architecture, then uint_arch_t is 8byte
 * if use 32bits architecture, then uint_arch_t is 4byte
 */
typedef unsigned long       uint_arch_t;
typedef signed long         int_arch_t;

#if (defined CONFIG_ARCH64) || (defined CONFIG_ARCH32)
typedef uint_arch_t         reg_t;
typedef uint_arch_t         pointer_t;
#define ARCH_BITS           (sizeof(uint_arch_t) * 8)
#endif

#endif
