#ifndef INPUT_H
#define INPUT_H
#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"

typedef enum { CMD_REQUEST, CMD_RELEASE, CMD_PRINT } command_type;

typedef struct {
    command_type type;
    int customer;    
    int *resources;  
} command;

void parse_arguments(int argc, char *argv[]);
void load_customers(void);
void load_commands(void);
const command *get_commands(int *count);
void input_free(void);
#endif 
