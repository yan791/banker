/*
 * test_parser.c - Testes de parser.c (leitura e conversao de texto)
 *
 * Compilar: cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_parser tests/test_parser.c src/parser.c src/util.c
 * Rodar:    ./tests/test_parser [grupo]   (sem grupo, roda todos)
 */

#include <stdio.h>
#include <string.h>

#include "check.h"
#include "parser.h"

/* Cria um arquivo temporario com o conteudo dado, pronto para leitura. */
static FILE *temp_file(const char *content)
{
    FILE *file = tmpfile();

    if (file != NULL) {
        fputs(content, file);
        rewind(file);
    }
    return file;
}

static void test_parse_non_negative(void)
{
    int value = -1;

    CHECK(parse_non_negative("0", &value) && value == 0);
    CHECK(parse_non_negative("10", &value) && value == 10);
    CHECK(parse_non_negative("007", &value) && value == 7);
    CHECK(parse_non_negative("2147483647", &value) && value == 2147483647);
    CHECK(!parse_non_negative("2147483648", &value));
    CHECK(!parse_non_negative("", &value));
    CHECK(!parse_non_negative("-5", &value));
    CHECK(!parse_non_negative("+5", &value));
    CHECK(!parse_non_negative("5a", &value));
    CHECK(!parse_non_negative("3.5", &value));
    CHECK(!parse_non_negative(" 5", &value));
}

static void test_read_line(void)
{
    char long_line[302];
    char *line;
    FILE *file;

    file = temp_file("7,5,3\n\n3,2,2\r\n9,0,2");
    CHECK(file != NULL);
    if (file == NULL)
        return;
    CHECK(read_line(file, "teste", 1, &line) == 1 && strcmp(line, "7,5,3") == 0);
    CHECK(read_line(file, "teste", 2, &line) == 1 && strcmp(line, "") == 0);
    CHECK(read_line(file, "teste", 3, &line) == 1 && strcmp(line, "3,2,2\r") == 0);
    CHECK(read_line(file, "teste", 4, &line) == 1 && strcmp(line, "9,0,2") == 0);
    CHECK(read_line(file, "teste", 5, &line) == 0);
    fclose(file);

    /* linha maior que o buffer inicial de 128 bytes */
    memset(long_line, 'a', 300);
    long_line[300] = '\n';
    long_line[301] = '\0';
    file = temp_file(long_line);
    CHECK(file != NULL);
    if (file == NULL)
        return;
    CHECK(read_line(file, "teste", 1, &line) == 1 && strlen(line) == 300);
    fclose(file);

    parser_free();
}

static void test_trim(void)
{
    char spaces[] = "  RQ 0 1  \t";
    char windows[] = "7,5,3\r";
    char blank[] = "   ";
    char empty[] = "";

    CHECK(strcmp(trim(spaces), "RQ 0 1") == 0);
    CHECK(strcmp(trim(windows), "7,5,3") == 0);
    CHECK(strcmp(trim(blank), "") == 0);
    CHECK(strcmp(trim(empty), "") == 0);
}

static void test_skip_bom(void)
{
    char with_bom[] = "\xEF\xBB\xBF" "7,5,3";
    char without_bom[] = "7,5,3";
    char short_text[] = "\xEF";

    CHECK(strcmp(skip_bom(with_bom), "7,5,3") == 0);
    CHECK(skip_bom(without_bom) == without_bom);
    CHECK(skip_bom(short_text) == short_text);
}

static void test_next_token(void)
{
    char line[] = "RQ  0\t1 2 ";
    char blank[] = "   ";
    char *cursor = line;
    char *blank_cursor = blank;

    CHECK(strcmp(next_token(&cursor), "RQ") == 0);
    CHECK(strcmp(next_token(&cursor), "0") == 0);
    CHECK(strcmp(next_token(&cursor), "1") == 0);
    CHECK(strcmp(next_token(&cursor), "2") == 0);
    CHECK(next_token(&cursor) == NULL);
    CHECK(next_token(&blank_cursor) == NULL);
}

int main(int argc, char *argv[])
{
    if (run_group(argc, argv, "parse_non_negative"))
        test_parse_non_negative();
    if (run_group(argc, argv, "read_line"))
        test_read_line();
    if (run_group(argc, argv, "trim"))
        test_trim();
    if (run_group(argc, argv, "skip_bom"))
        test_skip_bom();
    if (run_group(argc, argv, "next_token"))
        test_next_token();

    return check_done();
}
