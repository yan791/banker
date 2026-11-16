# Algoritmo do Banqueiro

Implementação em C do algoritmo do Banqueiro (*Banker's Algorithm*), com quantidade dinâmica de clientes e tipos de recursos.

## Estrutura do projeto

```text
banker/
├── Makefile           compilacao (make) e limpeza (make clean)
├── README.md
├── evidencias.log     evidencias de compilacao, execucao e testes
├── include/           cabecalhos (.h)
│   ├── banker.h
│   ├── input.h
│   ├── output.h
│   ├── parser.h
│   └── util.h
├── src/               codigo-fonte (.c)
│   ├── main.c         funcao principal
│   ├── banker.c       algoritmo do banqueiro: seguranca, requisicao e liberacao
│   ├── input.c        leitura e validacao dos argumentos, customer.txt e commands.txt
│   ├── output.c       execucao dos comandos e geracao do result.txt
│   ├── parser.c       funcoes auxiliares de leitura de texto
│   └── util.c         mensagens de erro e alocacao de memoria
└── tests/             testes manuais
    ├── check.h        macros dos testes (ok / FALHOU / RESULTADO)
    ├── test_util.c    testes de util.c
    ├── test_parser.c  testes de parser.c
    └── test_banker.c  testes de banker.c
```

## Requisitos

- Compilador C compatível com C99 (por exemplo, `cc` ou `gcc`)
- `make`

## Compilação

Na pasta do projeto, execute:

```bash
make
```

Isso gera o executável `banker`. Para remover o executável e os arquivos-objeto:

```bash
make clean
```

## Execução

Passe na linha de comando a quantidade total de instâncias de cada tipo de recurso. Cada argumento representa um tipo de recurso:

```bash
./banker 10 5 7
```

O exemplo inicia o programa com três tipos de recursos, contendo respectivamente 10, 5 e 7 instâncias.

## Arquivos de entrada

Os arquivos `customer.txt` e `commands.txt` devem estar no mesmo diretório em que o programa será executado.

### `customer.txt`

Cada linha representa um cliente. Os números separados por vírgula indicam a demanda máxima daquele cliente para cada tipo de recurso. A quantidade de valores em cada linha deve ser igual à quantidade de argumentos passados na execução. A demanda máxima de um cliente para um tipo de recurso não pode superar o total de instâncias desse recurso informado na linha de comando.

Exemplo para três tipos de recursos:

```text
7,5,3
3,2,2
9,0,2
2,2,2
4,3,3
```

### `commands.txt`

Contém um comando por linha:

- `RQ cliente r0 r1 ...`: solicita recursos para o cliente indicado.
- `RL cliente r0 r1 ...`: libera recursos do cliente indicado.
- `*`: imprime o estado atual das matrizes e dos recursos disponíveis.

A quantidade de valores de recursos em cada `RQ` ou `RL` deve ser igual à quantidade de tipos de recursos informada na execução. Os índices dos clientes começam em zero.

Exemplo:

```text
RQ 0 0 2 1
RQ 2 3 0 2
RL 0 0 1 1
*
```

## Saída

As operações e os resultados são gravados em `result.txt`, criado ou sobrescrito a cada execução bem-sucedida. Solicitações que ultrapassem o limite máximo do cliente, que não tenham recursos disponíveis ou que levem a um estado inseguro são recusadas.

Mensagens gravadas no `result.txt`:

```text
Allocate to customer 0 the resources 0 2 1
Release from customer 0 the resources 0 1 1
The resources 3 3 2 are not enough to customer 4 request 4 0 0
The customer 0 request 0 2 0 was denied because result in an unsafe state
The customer 2 request 0 1 0 was denied because exceed its maximum allocation
The customer 0 release 2 0 0 was denied because exceed its allocation
```

O comando `*` grava a tabela `MAXIMUM | ALLOCATION | NEED` e a linha `AVAILABLE`.

## Tratamento de erros

Antes de executar qualquer comando, o programa valida os argumentos e os dois arquivos por completo. Se encontrar um erro, mostra uma mensagem indicando o arquivo, a linha e o problema, encerra com código de saída 1 e não gera o `result.txt`. São tratados:

- quantidade de argumentos diferente da quantidade de recursos de uma linha do `customer.txt` ou de um comando `RQ`/`RL` do `commands.txt`;
- `customer.txt` ou `commands.txt` inexistente ou que não pode ser lido;
- formatação diferente da esperada: arquivo vazio, valor não numérico, negativo ou ausente, separador errado, linha vazia no meio do arquivo, comando desconhecido, cliente inexistente ou `*` com argumentos;
- argumentos inválidos: nenhum argumento, valor não numérico, negativo ou grande demais;
- demanda máxima de um cliente maior que o total de instâncias do recurso.

Exemplo:

```text
$ ./banker 10 5
Error: the number of resources does not match: customer.txt, line 1 (customer 0) has 3 resource value(s), but 2 resource type(s) were given on the command line
```

## Testes

Os testes são manuais. Cada arquivo de `tests/` testa as funções de um módulo e, no final, mostra `RESULTADO: CORRETO` ou `RESULTADO: INCORRETO`. Para compilar:

```bash
cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_util tests/test_util.c src/util.c
cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_parser tests/test_parser.c src/parser.c src/util.c
cc -std=c99 -Wall -Wextra -pedantic -Iinclude -o tests/test_banker tests/test_banker.c src/banker.c src/util.c
```

Para rodar todos os testes de um módulo, ou só um grupo:

```bash
./tests/test_parser
./tests/test_parser trim
```

A leitura dos arquivos de entrada e a geração do `result.txt` são testadas executando o `./banker` com entradas válidas e inválidas. As saídas de todos os testes estão registradas no `evidencias.log`.
