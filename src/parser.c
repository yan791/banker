#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "util.h"

int parse_non_negative(const char *text, int *value)
{
    int result = 0;

    if (*text == '\0')
        return 0;

    for (; *text != '\0'; text++) {
        int digit;

        if (!isdigit((unsigned char) *text))
            return 0;

        digit = *text - '0';

        if (result > (INT_MAX - digit) / 10)
            return 0;

        result = result * 10 + digit;
    }

    *value = result;
    return 1;
}