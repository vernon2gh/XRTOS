#include "pages.h"

extern uint32_t heap_start;
extern uint32_t heap_end;

static uint32_t _page_start = 0;
static uint32_t _page_end   = 0;
static uint32_t _num_pages  = 0;

static inline void _clear_flag(struct page *page)
{
    page->flags = 0;
}

static inline void _set_flag(struct page *page, uint8_t flags)
{
    page->flags |= flags;
}

static inline int _is_free(struct page *page)
{
    return !(page->flags & PAGE_TAKEN);
}

static inline int _is_last(struct page *page)
{
    return (page->flags & PAGE_LAST);
}

static inline uint32_t _align_page(uint32_t addr)
{
    uint32_t tmp = (1 << PAGE_ORDER) - 1;
    return (addr + tmp) & (~tmp);
}

/*
 * Reserved 8x4096B to hold the page structures.
 * Because qemu risc-v virt machine is most 128MB memory.
 */
void page_init(void)
{
    struct page *page_bitmaps;
    int i;

    _page_start = _align_page(heap_start + 8 * PAGE_SIZE);
    _page_end = heap_end;
    _num_pages = (_page_end - _page_start) / PAGE_SIZE - 8;

    page_bitmaps = (struct page *)heap_start;

    for(i = 0; i < _num_pages; i++) {
        _clear_flag(page_bitmaps);
        page_bitmaps++;
    }
}

/*
 * Allocate pages from continuoes physical memory block
 *
 * npages: the number of pages for want to allocate
 */
void *page_alloc(uint32_t npages)
{
    struct page *page_bitmaps, *tmp;
    int i, j, allocable;

    page_bitmaps = (struct page *)heap_start;
    allocable = 0;

    for(i = 0; i <= (_num_pages - npages); i++) {
        if(_is_free(page_bitmaps)) {
            allocable = 1;
            tmp = page_bitmaps;

            for(j = i; j < (i + npages); j++) {
                if(!_is_free(tmp++)) {
                    allocable = 0;
                    break;
                }
            }

            if(allocable) {
                tmp = page_bitmaps;

                for(j = i; j < (i + npages); j++)
                    _set_flag(tmp++, PAGE_TAKEN);

                _set_flag(--tmp, PAGE_LAST);

                return (void *)(_page_start + i * PAGE_SIZE);
            }
        }

        page_bitmaps++;
    }
    return NULL;
}

/*
 * Free pages from continuoes physical memory block
 *
 * p: at-first address of pages for want to free
 */
void page_free(void *p)
{
    struct page *page_bitmaps;

    page_bitmaps = (struct page *)heap_start;
    page_bitmaps += ((uint32_t)p - _page_start) / PAGE_SIZE;

    while(!_is_free(page_bitmaps)) {
        if(_is_last(page_bitmaps)) {
            _clear_flag(page_bitmaps);
            break;
        }

        _clear_flag(page_bitmaps++);
    }
}

