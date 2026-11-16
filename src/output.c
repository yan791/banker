#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "banker.h"
#include "input.h"
#include "output.h"
#include "util.h"

static FILE *result_file = NULL;

static void write_vector(const int vector[]){
    int j;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
        if (j > 0)
            fputc(' ', result_file);
        fprintf(result_file, "%d", vector[j]);
    }
}

static void write_vector_spaced(const int vector[]){
    int j;

    for (j = 0; j < NUMBER_OF_RESOURCES; j++)
        fprintf(result_file, "%d ", vector[j]);
}

static void execute_request(int customer, const int request[]){
    switch (request_resources(customer, request)) {
    case REQUEST_GRANTED:
        fprintf(result_file, "Allocate to customer %d the resources ", customer);
        write_vector(request);
        fputc('\n', result_file);
        break;
    case REQUEST_NOT_AVAILABLE:
        fputs("The resources ", result_file);
        write_vector_spaced(available);
        fprintf(result_file, "are not enough to customer %d request ", customer);
        write_vector_spaced(request);
        fputc('\n', result_file);
        break;
    case REQUEST_UNSAFE:
        fprintf(result_file, "The customer %d request ", customer);
        write_vector(request);
        fputs(" was denied because result in an unsafe state\n", result_file);
        break;
    case REQUEST_EXCEEDS_NEED:
    default:
        fprintf(result_file, "The customer %d request ", customer);
        write_vector(request);
        fputs(" was denied because exceed its maximum allocation\n",
              result_file);
        break;
    }
}

static void execute_release(int customer, const int release[]){
    if (release_resources(customer, release) == RELEASE_DONE) {
        fprintf(result_file, "Release from customer %d the resources ", customer);
        write_vector(release);
        fputc('\n', result_file);
    } else {
        fprintf(result_file, "The customer %d release ", customer);
        write_vector(release);
        fputs(" was denied because exceed its allocation\n", result_file);
    }
}

static void print_state(void){
    int i;

    fputs("MAXIMUM   |   ALLOCATION  |   NEED\n", result_file);
    for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        write_vector_spaced(maximum[i]);
        fputs("    |   ", result_file);
        write_vector_spaced(allocation[i]);
        fputs("      |   ", result_file);
        write_vector_spaced(need[i]);
        fputc('\n', result_file);
    }
    fputs("AVAILABLE ", result_file);
    write_vector_spaced(available);
    fputc('\n', result_file);
}

void execute_commands(void){
    const command *commands;
    int number_of_commands;
    int k;
    int write_error;

    result_file = fopen(RESULT_FILE, "w");
    if (result_file == NULL)
        fatal("could not create %s: %s", RESULT_FILE, strerror(errno));

    commands = get_commands(&number_of_commands);
    for (k = 0; k < number_of_commands; k++) {
        const command *cmd = &commands[k];

        switch (cmd->type) {
        case CMD_REQUEST:
            execute_request(cmd->customer, cmd->resources);
            break;
        case CMD_RELEASE:
            execute_release(cmd->customer, cmd->resources);
            break;
        case CMD_PRINT:
        default:
            print_state();
            break;
        }
    }

    write_error = ferror(result_file);
    if (fclose(result_file) != 0)
        write_error = 1;
    result_file = NULL;
    if (write_error)
        fatal("could not write %s", RESULT_FILE);
}

void output_free(void){
    if (result_file != NULL) {
        fclose(result_file);
        result_file = NULL;
    }
}
