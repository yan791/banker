#include <stdio.h>
#include <string.h>
#include "parser.h"

int main(void)
{
    char command[] = "  RQ   0  2\t1 ";
    char *cursor = command;
    char *token;

    token = next_token(&cursor);

    printf("Token encontrado: '%s'\n", token);

    if (strcmp(token, "RQ") != 0) {
        printf("Erro: token encontrado incorretamente!\n");
        return 1;
    }

    return 0;
}