/*
 * check.h - Macros e funcoes dos testes manuais
 *
 * CHECK(condicao) imprime "ok" ou "FALHOU" seguido do codigo testado.
 * Ao final, check_done() mostra RESULTADO: CORRETO ou RESULTADO: INCORRETO.
 */

#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <string.h>

static int checks_total = 0;
static int checks_failed = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        checks_total++;                                                 \
        if (cond) {                                                     \
            printf("  ok      %s\n", #cond);                            \
        } else {                                                        \
            checks_failed++;                                            \
            printf("  FALHOU  %s  (%s, linha %d)\n", #cond,             \
                   __FILE__, __LINE__);                                 \
        }                                                               \
    } while (0)

/*
 * Diz se o grupo de testes deve rodar: quando ele foi pedido na linha de
 * comando ou quando nenhum grupo foi pedido (roda todos).
 */
static int run_group(int argc, char *argv[], const char *name)
{
    if (argc > 1 && strcmp(argv[1], name) != 0)
        return 0;
    printf("== %s\n", name);
    return 1;
}

/* Mostra o resumo e devolve o codigo de saida (0 = tudo correto). */
static int check_done(void)
{
    printf("%d verificacao(oes), %d falha(s)\n", checks_total, checks_failed);
    if (checks_total > 0 && checks_failed == 0) {
        printf("RESULTADO: CORRETO\n");
        return 0;
    }
    printf("RESULTADO: INCORRETO\n");
    return 1;
}

#endif /* CHECK_H */
