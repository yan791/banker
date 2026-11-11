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
void *xmalloc(size_t size)
{
    void *pointer = malloc(size > 0 ? size : 1);

    if (pointer == NULL)
        fatal("out of memory");
    return pointer;
}

void *xcalloc(size_t count, size_t size)
{
    void *pointer = calloc(count > 0 ? count : 1, size > 0 ? size : 1);

    if (pointer == NULL)
        fatal("out of memory");
    return pointer;
}

void *xrealloc(void *pointer, size_t size)
{
    void *new_pointer = realloc(pointer, size > 0 ? size : 1);

    if (new_pointer == NULL)
        fatal("out of memory");
    return new_pointer;
}