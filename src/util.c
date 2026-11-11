#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void fatal(const char *format, ...)
{
    va_list args;

    fflush(stdout);
    fputs("Error: ", stderr);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}