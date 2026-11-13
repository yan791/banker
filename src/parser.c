#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "util.h"

static char *line_buffer = NULL;
static size_t line_capacity = 0;

int read_line(FILE *file, const char *filename, int line_number, char **line){
    size_t length = 0;
    int c;

    if (line_buffer == NULL) {
        line_capacity = 128;
        line_buffer = xmalloc(line_capacity);
    }

    while ((c = fgetc(file)) != EOF && c != '\n') {
        if (c == '\0')
            fatal("%s, line %d: invalid character in file "
                  "(the file must be plain text)", filename, line_number);

        if (length + 1 >= line_capacity) {
            line_buffer = xrealloc(line_buffer, line_capacity * 2);
            line_capacity *= 2;
        }

        line_buffer[length++] = (char) c;
    }

    if (ferror(file))
        fatal("could not read %s: %s", filename, strerror(errno));

    line_buffer[length] = '\0';
    *line = line_buffer;

    return !(c == EOF && length == 0);
}

char *trim(char *text){
    char *end;

    while (isspace((unsigned char) *text))
        text++;

    end = text + strlen(text);

    while (end > text && isspace((unsigned char) end[-1]))
        end--;

    *end = '\0';

    return text;
}
char *skip_bom(char *text){
    if (strncmp(text, "\xEF\xBB\xBF", 3) == 0)
        return text + 3;
    return text;
}

char *next_token(char **cursor){
    char *start = *cursor;
    char *end;

    while (isspace((unsigned char) *start))
        start++;

    if (*start == '\0') {
        *cursor = start;
        return NULL;
    }

    end = start;

    while (*end != '\0' && !isspace((unsigned char) *end))
        end++;

    if (*end != '\0')
        *end++ = '\0';

    *cursor = end;

    return start;
}

int parse_non_negative(const char *text, int *value){
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

void parser_free(void){
    free(line_buffer);

    line_buffer = NULL;
    line_capacity = 0;
}