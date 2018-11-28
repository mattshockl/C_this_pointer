#ifndef THIS_H
#define THIS_H

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "macros.h"

char __this__trampoline[] = "\x48\xb8" "\xbe\xba\xfe\xca\xef\xbe\xad\xde" /* mov rax, &ret_addr */
                            "\x50"                                        /* push rax */
                            "\x48\xb8" "\xbe\xba\xfe\xca\xef\xbe\xad\xde" /* mov rax, &this */
                            "\xc3";                                       /* ret */

typedef struct __this_page {
    void *xw_mem;
    unsigned int inuse;
    struct __this_page *next;
} __this_page;

__this_page *page_list = NULL;

__this_page *__this_get_open_page(void)
{
    __this_page *ret = NULL;
    __this_page **curr = &page_list;

    while (*curr != NULL)
    {
        __this_page *page = *curr;
        unsigned int memleft = getpagesize() - (page->inuse * sizeof(__this__trampoline));

        if (memleft > sizeof(__this__trampoline))
        {
            ret = page;
            goto done;
        }

        curr = &page->next;
    }

    void *xw_mem = mmap(NULL, getpagesize(), PROT_EXEC | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == xw_mem)
    {
        goto done;
    }

    __this_page *page = calloc(1, sizeof(__this_page));
    if (NULL == page)
    {
        munmap(xw_mem, 1);
        goto done;
    }
    page->xw_mem = xw_mem;
    page->inuse = 0;
    page->next = NULL;

    *curr = page;
    ret = page;

done:
    return ret;
}

void *__this_get_xw_mem(void)
{
    void *ret = NULL;

    __this_page *page = __this_get_open_page();
    if (NULL == page)
    {
        goto done;
    }

    unsigned int i;
    for (i = 0; i < getpagesize(); i += sizeof(__this__trampoline))
    {
        int *currmem = (int*)(page->xw_mem + i);
        if (*currmem == 0)
        {
            break;
        }
    }

    ret = page->xw_mem + i;
    page->inuse++;

done:
    return ret;
}

void *__this_create_trampoline(void *this, void *ret)
{
    void *xw_mem = __this_get_xw_mem();
    if (NULL == xw_mem)
    {
        return NULL;
    }
    
    memcpy(xw_mem, __this__trampoline, sizeof(__this__trampoline));
    memcpy(xw_mem + 2, &ret, sizeof(void *));
    memcpy(xw_mem + 13, &this, sizeof(void *));

    return xw_mem;
}

void __this_destroy_trampoline(void *mem)
{
    __this_page **curr = &page_list;

    while (*curr != NULL)
    {
        __this_page *page = *curr;
        if (page->xw_mem <= mem && (page->xw_mem + getpagesize()) > mem)
        {
            page->inuse--;
            memset(mem, 0, sizeof(__this__trampoline));

            if (page->inuse == 0)
            {
                *curr = page->next;
                munmap(page->xw_mem, 1);
                free(page);
            }
            return;
        }

        curr = &page->next;
    } 
}

#endif