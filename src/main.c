#include <stdlib.h>
#include "banker.h"
#include "input.h"
#include "parser.h"

static void cleanup(void)
{
    input_free();
    parser_free();
    banker_free();
}

int main(int argc, char *argv[])
{
    atexit(cleanup);

    parse_arguments(argc, argv); 
    load_customers();             
    load_commands();              
    initialize_state();           

    return EXIT_SUCCESS;
}
