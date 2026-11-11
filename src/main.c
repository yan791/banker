#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main(void)
{
    int *numbers = xmalloc(2 * sizeof(int));

    numbers[0] = 10;
    numbers[1] = 20;

    printf("xmalloc: %d e %d\n", numbers[0], numbers[1]);

    int *zeroed = xcalloc(3, sizeof(int));

    if (zeroed[0] != 0 || zeroed[1] != 0 || zeroed[2] != 0)
        return EXIT_FAILURE;

    printf("xcalloc: memoria inicializada com zero\n");

    numbers = xrealloc(numbers, 4 * sizeof(int));

    if (numbers[0] != 10 || numbers[1] != 20)
        return EXIT_FAILURE;

    numbers[2] = 30;
    numbers[3] = 40;

    printf("xrealloc: %d, %d, %d e %d\n",
           numbers[0], numbers[1], numbers[2], numbers[3]);

    free(numbers);
    free(zeroed);

    return EXIT_SUCCESS;
}