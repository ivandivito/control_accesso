//
// Created by ivan-ubuntu on 09/06/17.
//

#include "Heap.h"

void *pvPortRealloc(void *mem, size_t newsize)
{
    if (newsize == 0) {
        vPortFree(mem);
        return NULL;
    }

    void *p;
    p = pvPortMalloc(newsize);
    if (p) {
        /* zero the memory */
        if (mem != NULL) {
            memcpy(p, mem, newsize);
            vPortFree(mem);
        }
    }
    return p;
}