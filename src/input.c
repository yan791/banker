#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CUSTOMER_FILE "customer.txt"

static FILE *input_file = NULL;
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
static void parse_customer_line(char *line, int line_number, int customer,int *row)
{
    char *field = line;
    int count = 0;

    for (;;) {
        char *comma = strchr(field, ',');
        char *value;
        int number;

        if (comma != NULL)
            *comma = '\0';
        value = trim(field);

        if (*value == '\0')
            fatal("%s, line %d: missing value (expected non-negative "
                  "integers separated by commas, e.g. 7,5,3)",
                  CUSTOMER_FILE, line_number);
        if (!parse_non_negative(value, &number))
            fatal("%s, line %d: invalid value '%s' (expected non-negative "
                  "integers separated by commas, e.g. 7,5,3)",
                  CUSTOMER_FILE, line_number, value);

        if (count < NUMBER_OF_RESOURCES)
            row[count] = number;
        count++;

        if (comma == NULL)
            break;
        field = comma + 1;
    }

    if (count != NUMBER_OF_RESOURCES)
        fatal("the number of resources does not match: %s, line %d "
              "(customer %d) has %d resource value(s), but %d resource "
              "type(s) were given on the command line",
              CUSTOMER_FILE, line_number, customer, count, NUMBER_OF_RESOURCES);
}

void load_customers(void)
{
    int line_number = 0;
    int first_blank_line = 0;
    char *buffer;

    input_file = fopen(CUSTOMER_FILE, "r");
    if (input_file == NULL)
        fatal("could not open %s: %s", CUSTOMER_FILE, strerror(errno));

    while (read_line(input_file, CUSTOMER_FILE, line_number + 1, &buffer)) {
        char *line;
        int *row;

        line_number++;
        line = trim(line_number == 1 ? skip_bom(buffer) : buffer);

        if (*line == '\0') {
            if (first_blank_line == 0)
                first_blank_line = line_number;
            continue;
        }
        if (first_blank_line != 0)
            fatal("%s, line %d: empty line (each line must contain the "
                  "maximum demand of one customer, e.g. 7,5,3)",
                  CUSTOMER_FILE, first_blank_line);

        row = banker_add_customer();
        parse_customer_line(line, line_number, NUMBER_OF_CUSTOMERS - 1, row);
    }

    fclose(input_file);
    input_file = NULL;

    if (NUMBER_OF_CUSTOMERS == 0)
        fatal("%s is empty: it must contain one line per customer with its "
              "maximum demand (e.g. 7,5,3)", CUSTOMER_FILE);
}
