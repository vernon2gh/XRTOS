#ifndef __BYTES_H__
#define __BYTES_H__

#include <stddef.h>
#include "types.h"
#include "pages.h"

void byte_init(void);
void *byte_alloc(uint32_t nbytes);
void byte_free(void *p, uint32_t nbytes);

#endif
