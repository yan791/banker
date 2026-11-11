#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
#define ATTR_FATAL __attribute__((format(printf, 1, 2), noreturn))
#else
#define ATTR_FATAL
#endif

void fatal(const char *format, ...) ATTR_FATAL;

#endif 