#include <stdio.h>

#include "banker.h"
#include "input.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    int i, j;

    parse_arguments(argc, argv);
    load_customers();
    initialize_state();

    printf("Matriz NEED:\n");

    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", need[i][j]);

        printf("\n");
    }

    parser_free();
    banker_free();

    return 0;
}