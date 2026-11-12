#include <stdio.h>

#include "banker.h"
#include "input.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    int i;
    int j;

    parse_arguments(argc, argv);
    load_customers();

    printf("Quantidade de clientes: %d\n", NUMBER_OF_CUSTOMERS);
    printf("Quantidade de recursos: %d\n", NUMBER_OF_RESOURCES);

    printf("\nMAXIMUM:\n");

    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", maximum[i][j]);

        printf("\n");
    }

    parser_free();
    banker_free();

    return 0;
}