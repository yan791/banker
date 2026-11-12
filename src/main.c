#include <stdio.h>
#include "banker.h"

int main(void)
{
    int *customer;

    banker_init_resources(3);

    customer = banker_add_customer();

    if (customer != maximum[0]) {
        printf("Erro: ponteiro retornado incorretamente\n");
        banker_free();
        return 1;
    }

    printf("banker_add_customer: OK\n");

    banker_free();

    return 0;
}