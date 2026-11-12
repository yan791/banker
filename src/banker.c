#include <stdlib.h>

#include "banker.h"
#include "util.h"

int NUMBER_OF_CUSTOMERS = 0;
int NUMBER_OF_RESOURCES = 0;

int *available = NULL;
int **maximum = NULL;
int **allocation = NULL;
int **need = NULL;