#include <stdlib.h>

#include "banker.h"
#include "util.h"

int NUMBER_OF_CUSTOMERS = 0;
int NUMBER_OF_RESOURCES = 0;

int *available = NULL;
int **maximum = NULL;
int **allocation = NULL;
int **need = NULL;

static int maximum_capacity = 0;   
static int *work = NULL;
static int *finish = NULL;

void banker_init_resources(int number_of_resources){
    NUMBER_OF_RESOURCES = number_of_resources;
    available = xcalloc((size_t) NUMBER_OF_RESOURCES, sizeof *available);
}

int *banker_add_customer(void){
    if (NUMBER_OF_CUSTOMERS == maximum_capacity) {
        int new_capacity = maximum_capacity > 0 ? maximum_capacity * 2 : 8;
        int i;

        maximum = xrealloc(maximum, (size_t) new_capacity * sizeof *maximum);

        for (i = maximum_capacity; i < new_capacity; i++)
            maximum[i] = NULL;

        maximum_capacity = new_capacity;
    }

    maximum[NUMBER_OF_CUSTOMERS] =
        xcalloc((size_t) NUMBER_OF_RESOURCES, sizeof **maximum);

    return maximum[NUMBER_OF_CUSTOMERS++];
}

void banker_free(void){
    free_matrix(maximum, maximum_capacity);
    free_matrix(allocation, NUMBER_OF_CUSTOMERS);
    free_matrix(need, NUMBER_OF_CUSTOMERS);
    free(available);
    free(work);
    free(finish);
    maximum = allocation = need = NULL;
    available = work = finish = NULL;
    maximum_capacity = 0;
}
