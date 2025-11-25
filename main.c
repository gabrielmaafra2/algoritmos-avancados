#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Struct Sala
 * Representa cada comodo da mansao.
 * - nome: nome da sala
 * - pista: texto da pista encontrada (ou string vazia se nao houver pista)
 * - esquerda / direita: filhos da arvore binaria
 */
typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/*
 * Struct PistaNode
 * Representa um no da arvore BST de pistas coletadas.
 * - pista: conteudo da pista
 * - esquerda / direita: filhos na BST
 */
typedef struct PistaNode {
    char pista[80];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

/* ================================================
   FUNCOES PARA MANIPULAR AS SALAS (ARVORE BINARIA)
   ================================================ */

/*
 * criarSala:
 * Cria dinamicamente uma sala da mansao com seu nome e pista.
 * A pista pode ser string vazia se o comodo nao tiver pista.
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

/* ====================================================
   FUNCOES PARA MANIPULAR A ARVORE DE PISTAS (BST)
   ==================================================== */

/*
 * inserirPista:
 * Insere uma nova pista na BST de forma ordenada (a < z).
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (!novo) {
            printf("Erro ao alocar memoria.\n");
            exit(1);
        }
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

/*
 * exibirPistas:
 * Percorre a BST em ordem alfabetica (in-order) e imprime as pistas coletadas.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* =======================================================
   FUNCAO PRINCIPAL DE EXPLORACAO DA MANSÃO COM COLETA DE PISTAS
   ======================================================= */

/*
 * explorarSalasComPistas:
 * Navega pela mansao seguindo as escolhas do jogador.
 * - e: esquerda
 * - d: direita
 * - s: sair
 *
 * Sempre que o jogador entra em uma sala que possui pista,
 * essa pista e automaticamente inserida na BST.
 */
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;

    printf("\n=== INICIO DA EXPLORACAO ===\n");

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Se a sala tiver pista, guarda na BST
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada! -> \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste comodo.\n");
        }

        printf("Escolha o caminho:\n");
        printf("  e - Ir para a esquerda\n");
        printf("  d - Ir para a direita\n");
        printf("  s - Sair da exploracao\n");
        printf("Opcao: ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Nao ha sala a esquerda!\n");
        }
        else if (escolha == 'd') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Nao ha sala a direita!\n");
        }
        else if (escolha == 's') {
            printf("\nExploracao encerrada.\n");
            return;
        }
        else {
            printf("Opcao invalida.\n");
        }
    }
}

/* ============================================================
   FUNCAO MAIN
   Monta a arvore da mansao + pista, inicia exploracao,
   exibe pistas coletadas ao final.
   ============================================================ */

int main() {
    
    /* Mapa da mansao (arvore binaria fixa) */

    Sala* hall         = criarSala("Hall de Entrada", "Pegadas recentes na entrada");
    Sala* salaEstar    = criarSala("Sala de Estar",  "Um livro aberto na mesa");
    Sala* cozinha      = criarSala("Cozinha",        "Cheiro de algo queimado");
    Sala* biblioteca   = criarSala("Biblioteca",     "Um mapa antigo faltando uma parte");
    Sala* jardim       = criarSala("Jardim",         "");
    Sala* despensa     = criarSala("Despensa",       "Uma lata derrubada no chao");
    Sala* porao        = criarSala("Porao",          "Um objeto brilhante escondido");

    // Ligacoes da arvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita = porao;

    // Arvore de pistas (BST)
    PistaNode* arvorePistas = NULL;

    // Explorar a mansao e coletar pistas
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibir pistas coletadas
    printf("\n=== PISTAS COLETADAS (ORDEM ALFABETICA) ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    printf("===========================================\n");

    return 0;
}
