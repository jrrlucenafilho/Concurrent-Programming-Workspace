#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estrutura para manter o estoque
typedef struct {
    int quantidade;
    pthread_mutex_t lock;
} Estoque;

Estoque estoque; // Instância do estoque compartilhado
const int NUM_THREADS = 5; // Número de threads que tentarão atualizar o estoque

// Função para inicializar o estoque
void inicializarEstoque(Estoque *estoque) {
    estoque->quantidade = 0;
    pthread_mutex_init(&estoque->lock, NULL);
}

// Função para atualizar o estoque
void atualizarEstoque(Estoque *estoque, int quantidade) {
    pthread_mutex_lock(&estoque->lock); // Bloqueia a trava
    estoque->quantidade += quantidade;
    printf("Estoque atualizado por thread %ld: Quantidade = %d\n", pthread_self(), estoque->quantidade);
    pthread_mutex_unlock(&estoque->lock); // Libera a trava
}

// Função que simula uma operação de atualização do estoque por um thread
void *atualizarEstoqueThread(void *arg) {
    for (int i = 0; i < 3; i++) {
        atualizarEstoque(&estoque, 10);
        usleep(100); // Simula um pequeno atraso
    }
    return NULL;
}

int main() {
    inicializarEstoque(&estoque);

    pthread_t threads[NUM_THREADS];

    // Cria várias threads para atualizar o estoque
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, atualizarEstoqueThread, NULL) != 0) {
            perror("Erro ao criar thread");
            exit(EXIT_FAILURE);
        }
    }

    // Espera que todas as threads terminem
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erro ao aguardar a thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&estoque.lock);

    return 0;
}