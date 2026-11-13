#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

int read_line(FILE *file, const char *filename, int line_number, char **line);
char *trim(char *text);
char *skip_bom(char *text);
char *next_token(char **cursor);
int parse_non_negative(const char *text, int *value);
void parser_free(void);
#endif