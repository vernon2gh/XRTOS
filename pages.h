#ifndef __PAGES_H__
#define __PAGES_H__

#include <stddef.h>
#include "types.h"

#define PAGE_SIZE   4096
#define PAGE_ORDER  12

#define PAGE_TAKEN  (1<<0)
#define PAGE_LAST   (1<<1)

struct page {
    /*
     * bit 0: flag if this page is taken(allocated)
     * bit 1: flag if this page is the last page of the memory block allocated
     */
    uint8_t flags;
};

void page_init(void);
void *page_alloc(uint32_t npages);
void page_free(void *p);

#endif
