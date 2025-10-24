#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Estrutura da sala
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
    char pista[50];
    char suspeito[50];
} Sala;

// Estrutura da BST de pistas
typedef struct Pista {
    char nome[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// Estrutura da tabela hash (lista encadeada para colisões)
typedef struct EntradaHash {
    char pista[50];
    char suspeito[50];
    struct EntradaHash* prox;
} EntradaHash;

// Função de espalhamento simples
int hash(const char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % TAM_HASH;
}

// Inserção na tabela hash
void inserirNaHash(EntradaHash* tabela[], const char* pista, const char* suspeito) {
    int indice = hash(pista);
    EntradaHash* nova = (EntradaHash*)malloc(sizeof(EntradaHash));
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->prox = tabela[indice];
    tabela[indice] = nova;
}

// Exibe todas as associações pista → suspeito
void exibirHash(EntradaHash* tabela[]) {
    printf("\n🔗 Associações pista → suspeito:\n");
    for (int i = 0; i < TAM_HASH; i++) {
        EntradaHash* atual = tabela[i];
        while (atual != NULL) {
            printf("- %s → %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

// Conta ocorrências de suspeitos
void contarSuspeitos(EntradaHash* tabela[], char* suspeitoMaisCitado) {
    int max = 0;
    char suspeitos[50][50];
    int contadores[50] = {0};
    int total = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        EntradaHash* atual = tabela[i];
        while (atual != NULL) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contadores[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(suspeitos[total], atual->suspeito);
                contadores[total++] = 1;
            }
            atual = atual->prox;
        }
    }

    for (int i = 0; i < total; i++) {
        if (contadores[i] > max) {
            max = contadores[i];
            strcpy(suspeitoMaisCitado, suspeitos[i]);
        }
    }
}

// Funções da BST
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

void emOrdem(Pista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("- %s\n", raiz->nome);
        emOrdem(raiz->direita);
    }
}

// Exploração das salas
void explorarSalas(Sala* atual, Pista** pistas, EntradaHash* tabela[]) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("🔍 Pista encontrada: %s\n", atual->pista);
            printf("👤 Suspeito associado: %s\n", atual->suspeito);
            *pistas = inserirPista(*pistas, atual->pista);
            inserirNaHash(tabela, atual->pista, atual->suspeito);
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho.\n");
            break;
        }

        printf("Escolha uma direção:\n");
        if (atual->esquerda) printf("e → Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita) printf("d → Ir para a direita (%s)\n", atual->direita->nome);
        printf("s → Sair\n");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

int main() {
    // Criando salas com pistas e suspeitos
    Sala* hall = criarSala("Hall de Entrada", "", "");
    Sala* salaEstar = criarSala("Sala de Estar", "Pegada de lama", "Sr. Green");
    Sala* biblioteca = criarSala("Biblioteca", "Livro rasgado", "Sra. White");
    Sala* cozinha = criarSala("Cozinha", "Faca suja", "Srta. Scarlet");
    Sala* jardim = criarSala("Jardim", "Luvas molhadas", "Sr. Green");

    // Montando a árvore binária
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    // Inicializando estruturas
    Pista* pistas = NULL;
    EntradaHash* tabela[TAM_HASH] = {0};

    // Exploração
    explorarSalas(hall, &pistas, tabela);

    // Exibir pistas
    printf("\n🧠 Pistas coletadas:\n");
    emOrdem(pistas);

    // Exibir associações
    exibirHash(tabela);

    // Determinar suspeito mais citado
    char suspeitoMaisCitado[50];
    contarSuspeitos(tabela, suspeitoMaisCitado);
    printf("\n🎯 Suspeito mais citado: %s\n", suspeitoMaisCitado);

    return 0;
}