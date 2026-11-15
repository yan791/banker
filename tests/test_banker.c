/*
 * test_banker.c - Testes de banker.c (estado e Algoritmo do Banqueiro)
 *
 * Compilar: cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_banker tests/test_banker.c src/banker.c src/util.c
 * Rodar:    ./tests/test_banker [grupo]   (sem grupo, roda todos)
 */

#include "banker.h"
#include "check.h"

/* Libera o estado e zera os contadores, para o proximo grupo comecar do zero. */
static void reset_state(void)
{
    banker_free();
    NUMBER_OF_CUSTOMERS = 0;
    NUMBER_OF_RESOURCES = 0;
}

static void test_init_resources(void)
{
    banker_init_resources(3);
    CHECK(NUMBER_OF_RESOURCES == 3);
    CHECK(available != NULL);
    CHECK(available[0] == 0 && available[1] == 0 && available[2] == 0);
    CHECK(NUMBER_OF_CUSTOMERS == 0);
    reset_state();
    CHECK(available == NULL);
}

static void test_add_customer(void)
{
    int *first, *last = NULL;
    int i;

    banker_init_resources(2);
    first = banker_add_customer();
    first[0] = 4;
    first[1] = 1;
    /* 20 clientes: passa da capacidade inicial de 8 linhas */
    for (i = 1; i < 20; i++)
        last = banker_add_customer();
    CHECK(NUMBER_OF_CUSTOMERS == 20);
    CHECK(maximum[0] == first && maximum[0][0] == 4 && maximum[0][1] == 1);
    CHECK(maximum[19] == last && last[0] == 0 && last[1] == 0);
    reset_state();
}

int main(int argc, char *argv[])
{
    if (run_group(argc, argv, "init_resources"))
        test_init_resources();
    if (run_group(argc, argv, "add_customer"))
        test_add_customer();

    return check_done();
}
