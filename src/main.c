#include <stdio.h>
#include "banker.h"
#include "util.h"

int main(void)
{
    int i;

    /* Teste da inicializacao dos recursos */
    banker_init_resources(3);

    if (NUMBER_OF_RESOURCES != 3 || available == NULL) {
        printf("Erro na inicializacao dos recursos\n");
        return 1;
    }

    for (i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (available[i] != 0) {
            printf("Erro: recurso nao inicializado com zero\n");
            return 1;
        }
    }

    printf("banker_init_resources: OK\n");

    /* Simula a alocacao das matrizes allocation e need */
    NUMBER_OF_CUSTOMERS = 2;

    allocation = xcalloc(NUMBER_OF_CUSTOMERS, sizeof *allocation);
    need = xcalloc(NUMBER_OF_CUSTOMERS, sizeof *need);

    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        allocation[i] = xcalloc(NUMBER_OF_RESOURCES, sizeof **allocation);
        need[i] = xcalloc(NUMBER_OF_RESOURCES, sizeof **need);
    }

    printf("Matrizes allocation e need alocadas: OK\n");

    /* Teste da liberacao da memoria */
    banker_free();

    if (available != NULL ||
        maximum != NULL ||
        allocation != NULL ||
        need != NULL) {
        printf("Erro na liberacao da memoria\n");
        return 1;
    }

    printf("banker_free: OK\n");

    /* Teste de uma segunda chamada */
    banker_free();

    printf("Segunda chamada de banker_free: OK\n");

    return 0;
}