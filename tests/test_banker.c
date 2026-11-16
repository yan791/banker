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

/* Estado do exemplo do enunciado: recursos 10 5 7 e os 5 clientes do livro. */
static void load_example(void)
{
    static const int demand[5][3] = {
        {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}
    };
    int i, j;

    banker_init_resources(3);
    available[0] = 10;
    available[1] = 5;
    available[2] = 7;
    for (i = 0; i < 5; i++) {
        int *row = banker_add_customer();

        for (j = 0; j < 3; j++)
            row[j] = demand[i][j];
    }
    initialize_state();
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

static void test_initialize_state(void)
{
    load_example();
    CHECK(NUMBER_OF_CUSTOMERS == 5);
    CHECK(allocation[0][0] == 0 && allocation[2][1] == 0 && allocation[4][2] == 0);
    CHECK(need[0][0] == 7 && need[0][1] == 5 && need[0][2] == 3);
    CHECK(need[2][0] == 9 && need[2][1] == 0 && need[2][2] == 2);
    reset_state();
}

static void test_request(void)
{
    load_example();

    CHECK(request_resources(0, (const int[]) {0, 2, 1}) == REQUEST_GRANTED);
    CHECK(request_resources(2, (const int[]) {3, 0, 2}) == REQUEST_GRANTED);
    CHECK(request_resources(1, (const int[]) {2, 0, 0}) == REQUEST_GRANTED);
    CHECK(request_resources(4, (const int[]) {0, 0, 2}) == REQUEST_GRANTED);
    CHECK(available[0] == 5 && available[1] == 3 && available[2] == 2);
    CHECK(allocation[0][1] == 2 && need[0][1] == 3);

    /* pedido maior que need, maior que available e pedido inseguro */
    CHECK(request_resources(2, (const int[]) {0, 1, 0}) == REQUEST_EXCEEDS_NEED);
    CHECK(request_resources(0, (const int[]) {6, 0, 0}) == REQUEST_NOT_AVAILABLE);
    CHECK(request_resources(0, (const int[]) {0, 3, 0}) == REQUEST_UNSAFE);

    /* pedidos recusados nao alteram o estado */
    CHECK(available[0] == 5 && available[1] == 3 && available[2] == 2);
    CHECK(allocation[0][1] == 2 && need[0][1] == 3);

    CHECK(request_resources(3, (const int[]) {2, 1, 1}) == REQUEST_GRANTED);
    reset_state();
}

static void test_release(void)
{
    load_example();
    CHECK(request_resources(0, (const int[]) {0, 2, 1}) == REQUEST_GRANTED);

    CHECK(release_resources(0, (const int[]) {0, 1, 1}) == RELEASE_DONE);
    CHECK(allocation[0][0] == 0 && allocation[0][1] == 1 && allocation[0][2] == 0);
    CHECK(need[0][1] == 4 && need[0][2] == 3);
    CHECK(available[0] == 10 && available[1] == 4 && available[2] == 7);

    /* liberar mais do que possui e recusado e nao altera o estado */
    CHECK(release_resources(0, (const int[]) {0, 2, 0}) == RELEASE_EXCEEDS_ALLOCATION);
    CHECK(release_resources(3, (const int[]) {0, 0, 1}) == RELEASE_EXCEEDS_ALLOCATION);
    CHECK(allocation[0][1] == 1 && available[1] == 4);
    reset_state();
}

int main(int argc, char *argv[])
{
    if (run_group(argc, argv, "init_resources"))
        test_init_resources();
    if (run_group(argc, argv, "add_customer"))
        test_add_customer();
    if (run_group(argc, argv, "initialize_state"))
        test_initialize_state();
    if (run_group(argc, argv, "request"))
        test_request();
    if (run_group(argc, argv, "release"))
        test_release();

    return check_done();
}
