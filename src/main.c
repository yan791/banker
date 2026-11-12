#include <stdio.h>
#include "banker.h"
#include "input.h"

int main(int argc, char *argv[])
{
    int i;

    parse_arguments(argc, argv);

    printf("Quantidade de recursos: %d\n", NUMBER_OF_RESOURCES);
    printf("Available: ");

    for (i = 0; i < NUMBER_OF_RESOURCES; i++)
        printf("%d ", available[i]);

    printf("\n");

    banker_free();

    return 0;
}