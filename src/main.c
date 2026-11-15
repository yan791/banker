#include <stdio.h>

#include "banker.h"
#include "input.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    const command *list;
    int command_count;
    int i, j;

    parse_arguments(argc, argv);
    load_customers();
    load_commands();

    printf("Clientes carregados: %d\n", NUMBER_OF_CUSTOMERS);
    printf("Recursos: %d\n", NUMBER_OF_RESOURCES);

    printf("\nComandos carregados:\n");

    list = get_commands(&command_count);

    for (i = 0; i < command_count; i++) {
        if (list[i].type == CMD_PRINT) {
            printf("*\n");
        } else {
            printf("%s %d",
                   list[i].type == CMD_REQUEST ? "RQ" : "RL",
                   list[i].customer);

            for (j = 0; j < NUMBER_OF_RESOURCES; j++)
                printf(" %d", list[i].resources[j]);

            printf("\n");
        }
    }

    printf("\nTotal de comandos: %d\n", command_count);

    input_free();
    parser_free();
    banker_free();

    return 0;
}   