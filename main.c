#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

/* ============================================
   STRUCTS PRINCIPAIS
   ============================================ */

/*
 * Struct Sala:
 * Representa cada comodo da mansao.
 * Campos:
 * - nome: identificador da sala
 * - pista: texto da pista encontrada
 * - esquerda/direita: filhos da arvore binaria
 */
typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/*
 * Struct PistaNode:
 * No da BST de pistas coletadas.
 */
typedef struct PistaNode {
    char pista[80];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

/*
 * Struct HashItem:
 * Associacao pista -> suspeito.
 */
typedef struct HashItem {
    char pista[80];
    char suspeito[40];
    struct HashItem* prox;
} HashItem;

/* ============================================
   FUNCOES DA ARVORE BINARIA (MAPA DA MANSAO)
   ============================================ */

/* criaSala:
 * Cria um comodo da mansao dinamicamente.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ============================================
   FUNCOES DA BST (ARVORE DE PISTAS COLETADAS)
   ============================================ */

/* inserirPista:
 * Insere uma pista na BST em ordem alfabetica.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/* exibirPistas:
 * Exibe as pistas coletadas em ordem alfabetica.
 */
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* ============================================
   FUNCOES DA TABELA HASH
   ============================================ */

/* hashFunction:
 * Retorna o indice para a pista.
 */
int hashFunction(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

/* inserirNaHash:
 * Registra pista -> suspeito.
 */
void inserirNaHash(HashItem* tabela[], const char* pista, const char* suspeito) {
    int indice = hashFunction(pista);

    HashItem* novo = (HashItem*)malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

/* encontrarSuspeito:
 * Dada uma pista, retorna o suspeito associado.
 * Retorna NULL se nao encontrado.
 */
char* encontrarSuspeito(HashItem* tabela[], const char* pista) {
    int indice = hashFunction(pista);
    HashItem* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/* ============================================
   FUNCAO DE JULGAMENTO FINAL
   ============================================ */

/* contarSuspeito:
 * Percorre a BST e conta quantas pistas apontam para o acusado.
 */
int contarSuspeito(PistaNode* raiz, HashItem* tabela[], const char* acusado) {
    if (!raiz) return 0;

    int cont = 0;

    char* suspeito = encontrarSuspeito(tabela, raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, acusado) == 0)
        cont++;

    return cont +
           contarSuspeito(raiz->esquerda, tabela, acusado) +
           contarSuspeito(raiz->direita, tabela, acusado);
}

/* verificarSuspeitoFinal:
 * Decide se ha evidencias suficientes (2 pistas)
 * para condenar o suspeito escolhido.
 */
void verificarSuspeitoFinal(PistaNode* arvorePistas, HashItem* tabela[]) {
    char acusado[40];
    printf("\n=== FASE FINAL ===\n");
    printf("Digite o nome do suspeito que voce deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int total = contarSuspeito(arvorePistas, tabela, acusado);

    printf("\nPistas que apontam para %s: %d\n", acusado, total);

    if (total >= 2)
        printf("Decisao: O suspeito %s e o culpado! Caso resolvido.\n", acusado);
    else
        printf("Decisao: Ha evidencias insuficientes. Voce acusou a pessoa errada!\n");
}

/* ============================================
   EXPLORACAO DA MANSAO
   ============================================ */

/* explorarSalas:
 * Permite ao jogador navegar pela mansao.
 * Coleta pistas e as insere na BST.
 */
void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste comodo.\n");
        }

        printf("Escolha o caminho: (e) esquerda, (d) direita, (s) sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Nao ha caminho a esquerda!\n");
        }
        else if (opcao == 'd') {
            if (atual->direita) atual = atual->direita;
            else printf("Nao ha caminho a direita!\n");
        }
        else if (opcao == 's') {
            printf("\nExploracao encerrada.\n");
            break;
        }
        else {
            printf("Opcao invalida.\n");
        }
    }
}

/* ============================================
   MAIN – MONTAGEM DO MAPA + HASH + EXPLORACAO
   ============================================ */

int main() {
    /* Mapa da mansao (fixo) */
    Sala* hall        = criarSala("Hall de Entrada", "Pegadas de botas");
    Sala* salaEstar   = criarSala("Sala de Estar",   "Livro manchado");
    Sala* cozinha     = criarSala("Cozinha",         "Faca faltando");
    Sala* biblioteca  = criarSala("Biblioteca",      "Pagina rasgada");
    Sala* jardim      = criarSala("Jardim",          "");
    Sala* despensa    = criarSala("Despensa",        "Embalagem aberta");
    Sala* porao       = criarSala("Porao",           "Chave enferrujada");

    hall->esquerda = salaEstar;
    hall->direita  = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita  = porao;

    /* Arvore BST de pistas coletadas */
    PistaNode* arvorePistas = NULL;

    /* Tabela hash de pistas → suspeitos */
    HashItem* tabela[TAM_HASH] = {0};

    /* Inserindo associacoes pista → suspeito */
    inserirNaHash(tabela, "Pegadas de botas",      "Carlos");
    inserirNaHash(tabela, "Livro manchado",        "Ana");
    inserirNaHash(tabela, "Faca faltando",         "Carlos");
    inserirNaHash(tabela, "Pagina rasgada",        "Beatriz");
    inserirNaHash(tabela, "Embalagem aberta",      "Ana");
    inserirNaHash(tabela, "Chave enferrujada",     "Carlos");

    /* Exploracao da mansao */
    explorarSalas(hall, &arvorePistas);

    /* Exibir pistas coletadas */
    printf("\n=== PISTAS COLETADAS ===\n");
    exibirPistas(arvorePistas);

    /* Fase final: julgamento */
    verificarSuspeitoFinal(arvorePistas, tabela);

    return 0;
}
