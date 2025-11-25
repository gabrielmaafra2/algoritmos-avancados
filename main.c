#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Struct Sala
 * Representa um comodo da mansao.
 * Cada sala possui:
 *  - nome: string identificando o comodo
 *  - esquerda: ponteiro para a sala a esquerda
 *  - direita: ponteiro para a sala a direita
 */
typedef struct Sala {
    char nome[40];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/*
 * Funcao: criarSala
 * Cria dinamicamente uma sala usando malloc
 * Copia o nome passado como parametro para o campo "nome"
 * Inicializa esquerda e direita como NULL (folha)
 */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

/*
 * Funcao: explorarSalas
 * Permite ao jogador navegar pela mansao a partir do Hall.
 * A navegacao e feita entrando com 'e' (esquerda), 'd' (direita) ou 's' (sair).
 * A funcao termina ao chegar em uma sala sem filhos ou se o jogador decidir sair.
 */
void explorarSalas(Sala* atual) {
    char escolha;

    printf("\n=== EXPLORACAO DA MANSAO ===\n");

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Caso seja uma folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala nao possui caminhos. Exploracao encerrada.\n");
            return;
        }

        printf("Escolha um caminho:\n");
        printf("  e - Ir para a esquerda\n");
        printf("  d - Ir para a direita\n");
        printf("  s - Sair da exploracao\n");
        printf("Opcao: ");

        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao ha caminho a esquerda!\n");
            }
        }
        else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao ha caminho a direita!\n");
            }
        }
        else if (escolha == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            return;
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

/*
 * Funcao principal
 * Monta manualmente o mapa da mansao usando uma arvore binaria.
 * O Hall e a raiz da arvore.
 * Depois, inicia a exploracao.
 */
int main() {

    /* Montagem manual da arvore (fixa) */

    Sala* hall         = criarSala("Hall de entrada");
    Sala* salaEstar    = criarSala("Sala de estar");
    Sala* cozinha      = criarSala("Cozinha");
    Sala* biblioteca   = criarSala("Biblioteca");
    Sala* jardim       = criarSala("Jardim");
    Sala* despensa     = criarSala("Despensa");
    Sala* porao        = criarSala("Porao");

    // Ligacoes da arvore (esquerda / direita)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita = porao;

    // Iniciar exploracao
    explorarSalas(hall);
    
    return 0;
}
