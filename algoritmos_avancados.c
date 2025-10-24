#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
    char pista[50]; // Pista encontrada na sala (se houver)
} Sala;

// Estrutura da árvore de busca de pistas
typedef struct Pista {
    char nome[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// Função para criar uma sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para criar uma pista
Pista* inserirPista(Pista* raiz, const char* nome) {
    if (raiz == NULL) {
        Pista* nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->nome, nome);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }
    if (strcmp(nome, raiz->nome) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->direita = inserirPista(raiz->direita, nome);
    return raiz;
}

// Exibe pistas em ordem alfabética
void emOrdem(Pista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("- %s\n", raiz->nome);
        emOrdem(raiz->direita);
    }
}

// Exploração das salas
void explorarSalas(Sala* atual, Pista** pistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("🔍 Pista encontrada: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        }

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
    // Criando salas com pistas
    Sala* hall = criarSala("Hall de Entrada", "");
    Sala* salaEstar = criarSala("Sala de Estar", "Pegada de lama");
    Sala* biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala* cozinha = criarSala("Cozinha", "Faca suja");
    Sala* jardim = criarSala("Jardim", "");

    // Montando a árvore binária de salas
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    // Árvore de pistas
    Pista* pistas = NULL;

    // Exploração
    explorarSalas(hall, &pistas);

    // Exibindo pistas encontradas
    printf("\n🧠 Pistas coletadas (em ordem alfabética):\n");
    emOrdem(pistas);

    // Liberando memória (simplificado)
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    // Liberação da BST de pistas omitida por simplicidade
    return 0;
}