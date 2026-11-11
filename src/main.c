#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(void)
{
    int value;

    if (parse_non_negative("123", &value) && value == 123)
        printf("Teste 1: numero valido - OK\n");
    else
        return EXIT_FAILURE;

    if (parse_non_negative("0", &value) && value == 0)
        printf("Teste 2: zero - OK\n");
    else
        return EXIT_FAILURE;

    if (!parse_non_negative("-5", &value))
        printf("Teste 3: numero negativo rejeitado - OK\n");
    else
        return EXIT_FAILURE;

    if (!parse_non_negative("12a", &value))
        printf("Teste 4: texto invalido rejeitado - OK\n");
    else
        return EXIT_FAILURE;

    if (!parse_non_negative("", &value))
        printf("Teste 5: texto vazio rejeitado - OK\n");
    else
        return EXIT_FAILURE;

    if (!parse_non_negative("999999999999999999999999", &value))
        printf("Teste 6: numero muito grande rejeitado - OK\n");
    else
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}