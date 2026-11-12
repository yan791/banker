#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int main(void)
{
    FILE *file;
    char *line;
    int i;

    file = tmpfile();

    if (file == NULL) {
        printf("Erro ao criar arquivo temporario\n");
        return EXIT_FAILURE;
    }

    fputs("  primeira linha  \n\n\tsegunda linha\t\n", file);

    /* Cria uma linha maior que o buffer inicial de 128 bytes. */
    for (i = 0; i < 200; i++)
        fputc('A', file);

    fputc('\n', file);

    fputs("ultima linha sem quebra", file);

    rewind(file);

    /* Teste 1: leitura e remocao de espacos. */
    if (!read_line(file, "teste.txt", 1, &line) ||
        strcmp(trim(line), "primeira linha") != 0) {
        printf("Teste 1: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 1: leitura e trim - OK\n");

    /* Teste 2: linha vazia. */
    if (!read_line(file, "teste.txt", 2, &line) ||
        strcmp(trim(line), "") != 0) {
        printf("Teste 2: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 2: leitura de linha vazia - OK\n");

    /* Teste 3: remocao de tabulacoes. */
    if (!read_line(file, "teste.txt", 3, &line) ||
        strcmp(trim(line), "segunda linha") != 0) {
        printf("Teste 3: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 3: remocao de tabulacoes - OK\n");

    /* Teste 4: leitura de linha maior que 128 caracteres. */
    if (!read_line(file, "teste.txt", 4, &line) ||
        strlen(line) != 200) {
        printf("Teste 4: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 4: redimensionamento do buffer - OK\n");

    /* Teste 5: ultima linha sem quebra de linha. */
    if (!read_line(file, "teste.txt", 5, &line) ||
        strcmp(line, "ultima linha sem quebra") != 0) {
        printf("Teste 5: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 5: ultima linha sem quebra - OK\n");

    /* Teste 6: verificacao do final do arquivo. */
    if (read_line(file, "teste.txt", 6, &line)) {
        printf("Teste 6: FALHOU\n");
        return EXIT_FAILURE;
    }

    printf("Teste 6: identificacao do final do arquivo - OK\n");

    parser_free();
    parser_free();

    printf("Teste 7: liberacao do buffer - OK\n");

    fclose(file);

    return EXIT_SUCCESS;
}