#include <stdio.h>
#include <string.h>
#include "parser.h"

int main(void)
{
    char text_with_bom[] = "\xEF\xBB\xBF" "7,5,3";
    char text_without_bom[] = "7,5,3";

    char *result;

    result = skip_bom(text_with_bom);

    if (result != text_with_bom + 3 ||
        strcmp(result, "7,5,3") != 0) {
        printf("Teste 1: FALHOU\n");
        return 1;
    }

    printf("Teste 1: remocao do BOM UTF-8 - OK\n");

    result = skip_bom(text_without_bom);

    if (result != text_without_bom ||
        strcmp(result, "7,5,3") != 0) {
        printf("Teste 2: FALHOU\n");
        return 1;
    }

    printf("Teste 2: texto sem BOM preservado - OK\n");

    return 0;
}