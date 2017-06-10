//
// Created by ivan-ubuntu on 09/06/17.
//

#ifndef CONTROL_HEAP_H
#define CONTROL_HEAP_H

#include "FreeRTOS.h"
#include "stddef.h"
#include "string.h"

void *pvPortRealloc(void *mem, size_t newsize) PRIVILEGED_FUNCTION;

#endif //CONTROL_HEAP_H
