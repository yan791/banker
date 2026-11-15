/*
 * test_util.c - Testes de util.c (erros e alocacao de memoria)
 *
 * Compilar: cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_util tests/test_util.c src/util.c
 * Rodar:    ./tests/test_util [grupo]   (sem grupo, roda todos)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "check.h"
#include "util.h"

/* xmalloc, xcalloc, xrealloc e free_matrix */
static void test_memory(void)
{
    char *text;
    int *values;
    int **matrix;

    text = xmalloc(6);
    strcpy(text, "teste");
    CHECK(strcmp(text, "teste") == 0);
    free(text);

    values = xcalloc(4, sizeof *values);
    CHECK(values[0] == 0 && values[1] == 0 && values[2] == 0 && values[3] == 0);

    values[0] = 7;
    values[3] = 9;
    values = xrealloc(values, 100 * sizeof *values);
    CHECK(values[0] == 7 && values[3] == 9);
    free(values);

    matrix = xcalloc(3, sizeof *matrix);
    matrix[0] = xcalloc(2, sizeof **matrix);
    matrix[2] = xcalloc(2, sizeof **matrix);
    CHECK(matrix[1] == NULL);
    free_matrix(matrix, 3);
    free_matrix(NULL, 3);
}

/*
 * fatal() deve escrever "Error: ..." em stderr e encerrar com codigo 1.
 * Ela e chamada num processo filho, para o teste continuar rodando.
 */
static void test_fatal(void)
{
    char message[64];
    size_t length = 0;
    ssize_t n;
    int status = 0;
    int fd[2];
    pid_t pid;

    if (pipe(fd) != 0) {
        CHECK(!"pipe() falhou");
        return;
    }
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDERR_FILENO);
        fatal("teste de erro %d", 42);
    }
    close(fd[1]);
    while (length < sizeof message - 1 &&
           (n = read(fd[0], message + length, sizeof message - 1 - length)) > 0)
        length += (size_t) n;
    message[length] = '\0';
    close(fd[0]);
    waitpid(pid, &status, 0);

    CHECK(pid > 0);
    CHECK(WIFEXITED(status) && WEXITSTATUS(status) == 1);
    CHECK(strcmp(message, "Error: teste de erro 42\n") == 0);
}

int main(int argc, char *argv[])
{
    if (run_group(argc, argv, "memory"))
        test_memory();
    if (run_group(argc, argv, "fatal"))
        test_fatal();

    return check_done();
}
