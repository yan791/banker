#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "banker.h"
#include "input.h"
#include "parser.h"
#include "util.h"

void parse_arguments(int argc, char *argv[]){
    int j;

    if (argc < 2)
        fatal("missing arguments: the number of instances of each resource "
              "type must be given on the command line\n"
              "Usage: ./banker <instances of resource 0> "
              "<instances of resource 1> ...\n"
              "Example: ./banker 10 5 7");

    banker_init_resources(argc - 1);

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        if (!parse_non_negative(argv[j + 1], &available[j]))
            fatal("invalid argument '%s' (argument %d): the number of "
                  "instances of a resource must be an integer between 0 and %d",
                  argv[j + 1], j + 1, INT_MAX);
}