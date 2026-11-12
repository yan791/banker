#include <stdio.h>
#include <stddef.h>
#include "banker.h"

int main(void)
{
    if (NUMBER_OF_CUSTOMERS != 0 ||
        NUMBER_OF_RESOURCES != 0 ||
        available != NULL ||
        maximum != NULL ||
        allocation != NULL ||
        need != NULL) {
        printf("Erro na inicializacao do estado do Banker\n");
        return 1;
    }

    printf("Estado inicial do Banker: OK\n");

    return 0;
}