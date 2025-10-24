#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de um cômodo (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função para criar uma sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas
void explorarSalas(Sala* atual) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho. Sala sem saídas.\n");
            break;
        }
        printf("Escolha uma direção:\n");
        if (atual->esquerda) printf("e → Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita) printf("d → Ir para a direita (%s)\n", atual->direita->nome);
        printf("s → Sair da exploração\n");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

int main() {
    // Criando salas
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jardim = criarSala("Jardim");

    // Montando a árvore binária
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    // Iniciando exploração
    explorarSalas(hall);

    // Liberando memória
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);

    return 0;
}