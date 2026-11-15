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

static command *commands = NULL;
static int number_of_commands = 0;
static int commands_capacity = 0;

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

    for (count = 0; count < NUMBER_OF_RESOURCES; count++)
        if (row[count] > available[count])
            fatal("%s, line %d: customer %d maximum demand for resource %d "
                  "(%d) exceeds the total number of instances (%d)",
                  CUSTOMER_FILE, line_number, customer, count,
                  row[count], available[count]);
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
static void parse_request_release(command *cmd, const char *name,char *cursor, int line_number){
    char *token;
    int count = 0;

    token = next_token(&cursor);
    if (token == NULL)
        fatal("%s, line %d: missing customer number in command %s "
              "(expected: %s <customer> <%d resource value(s)>)",
              COMMANDS_FILE, line_number, name, name, NUMBER_OF_RESOURCES);
    if (!parse_non_negative(token, &cmd->customer))
        fatal("%s, line %d: invalid customer number '%s' in command %s "
              "(expected a non-negative integer)",
              COMMANDS_FILE, line_number, token, name);
    if (cmd->customer >= NUMBER_OF_CUSTOMERS)
        fatal("%s, line %d: customer %d does not exist (%s defines "
              "customers 0 to %d)", COMMANDS_FILE, line_number,
              cmd->customer, CUSTOMER_FILE, NUMBER_OF_CUSTOMERS - 1);

    cmd->resources = xcalloc((size_t) NUMBER_OF_RESOURCES,
                             sizeof *cmd->resources);

    while ((token = next_token(&cursor)) != NULL) {
        int number;

        if (!parse_non_negative(token, &number))
            fatal("%s, line %d: invalid resource value '%s' in command %s "
                  "(expected non-negative integers separated by spaces)",
                  COMMANDS_FILE, line_number, token, name);
        if (count < NUMBER_OF_RESOURCES)
            cmd->resources[count] = number;
        count++;
    }

    if (count != NUMBER_OF_RESOURCES)
        fatal("the number of resources does not match: %s, line %d "
              "(command %s) has %d resource value(s), but %d resource "
              "type(s) were given on the command line",
              COMMANDS_FILE, line_number, name, count, NUMBER_OF_RESOURCES);
}
void load_commands(void){
    int line_number = 0;
    int first_blank_line = 0;
    char *buffer;

    input_file = fopen(COMMANDS_FILE, "r");
    if (input_file == NULL)
        fatal("could not open %s: %s", COMMANDS_FILE, strerror(errno));

    while (read_line(input_file, COMMANDS_FILE, line_number + 1, &buffer)) {
        char *line;
        char *cursor;
        char *token;
        command *cmd;

        line_number++;
        line = trim(line_number == 1 ? skip_bom(buffer) : buffer);

        if (*line == '\0') {
            if (first_blank_line == 0)
                first_blank_line = line_number;
            continue;
        }
        if (first_blank_line != 0)
            fatal("%s, line %d: empty line (each line must contain one "
                  "command: RQ, RL or *)", COMMANDS_FILE, first_blank_line);

        if (number_of_commands == commands_capacity) {
            int new_capacity = commands_capacity > 0 ? commands_capacity * 2 : 16;

            commands = xrealloc(commands, (size_t) new_capacity * sizeof *commands);
            commands_capacity = new_capacity;
        }
        cmd = &commands[number_of_commands++];
        cmd->customer = -1;
        cmd->resources = NULL;

        cursor = line;
        token = next_token(&cursor);

        if (strcmp(token, "*") == 0) {
            cmd->type = CMD_PRINT;
            if (next_token(&cursor) != NULL)
                fatal("%s, line %d: the command * does not take arguments",
                      COMMANDS_FILE, line_number);
        } else if (strcmp(token, "RQ") == 0) {
            cmd->type = CMD_REQUEST;
            parse_request_release(cmd, "RQ", cursor, line_number);
        } else if (strcmp(token, "RL") == 0) {
            cmd->type = CMD_RELEASE;
            parse_request_release(cmd, "RL", cursor, line_number);
        } else {
            fatal("%s, line %d: unknown command '%s' (valid commands: "
                  "RQ, RL and *)", COMMANDS_FILE, line_number, token);
        }
    }

    fclose(input_file);
    input_file = NULL;

    if (number_of_commands == 0)
        fatal("%s is empty: it must contain at least one command "
              "(RQ, RL or *)", COMMANDS_FILE);
}

const command *get_commands(int *count){
    *count = number_of_commands;
    return commands;
}

void input_free(void){
    int i;

    for (i = 0; i < number_of_commands; i++)
        free(commands[i].resources);
    free(commands);
    commands = NULL;
    number_of_commands = 0;
    commands_capacity = 0;

    if (input_file != NULL) {
        fclose(input_file);
        input_file = NULL;
    }
}