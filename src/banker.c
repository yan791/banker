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

void initialize_state(void){
    int i, j;

    allocation = xcalloc((size_t) NUMBER_OF_CUSTOMERS, sizeof *allocation);
    need = xcalloc((size_t) NUMBER_OF_CUSTOMERS, sizeof *need);

    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        allocation[i] = xcalloc((size_t) NUMBER_OF_RESOURCES,
                                sizeof **allocation);
        need[i] = xcalloc((size_t) NUMBER_OF_RESOURCES, sizeof **need);

        for (j = 0; j < NUMBER_OF_RESOURCES; j++)
            need[i][j] = maximum[i][j];
    }

    work = xcalloc((size_t) NUMBER_OF_RESOURCES, sizeof *work);
    finish = xcalloc((size_t) NUMBER_OF_CUSTOMERS, sizeof *finish);
}

static int less_or_equal(const int a[], const int b[]){
    int j;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        if (a[j] > b[j])
            return 0;
    return 1;
}

static int is_safe(void){
    int i, j;
    int finished = 0;
    int progress = 1;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        work[j] = available[j];
    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        finish[i] = 0;

    while (progress) {
        progress = 0;
        for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (finish[i] || !less_or_equal(need[i], work))
                continue;
            for (j = 0; j < NUMBER_OF_RESOURCES; j++)
                work[j] += allocation[i][j];
            finish[i] = 1;
            finished++;
            progress = 1;
        }
    }

    return finished == NUMBER_OF_CUSTOMERS;
}

int request_resources(int customer_num, const int request[]){
    int j;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        if (request[j] > need[customer_num][j])
            return REQUEST_EXCEEDS_NEED;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        if (request[j] > available[j])
            return REQUEST_NOT_AVAILABLE;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
        available[j] -= request[j];
        allocation[customer_num][j] += request[j];
        need[customer_num][j] -= request[j];
    }

    if (!is_safe()) {
        for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
            available[j] += request[j];
            allocation[customer_num][j] -= request[j];
            need[customer_num][j] += request[j];
        }
        return REQUEST_UNSAFE;
    }

    return REQUEST_GRANTED;
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
