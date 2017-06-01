#ifndef PRINT_H
#define PRINT_H

#include "../../control/inc/sapi_uart.h"

int sprintf(char *out, const char *format, ...);
int debug_uart(const char *format, ...);

#endif
