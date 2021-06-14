#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int value);    //< definida em helper.c
void consumir(int produto); //< definida em helper.c
void *produtor_func(void *arg);
void *consumidor_func(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer;
int* buffer;
sem_t cheio, vazio;

//Você deve fazer as alterações necessárias nesta função e na função
//consumidor_func para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.
void *produtor_func(void *arg) {
    //arg contem o número de itens a serem produzidos
    int max = *((int*)arg);
    for (int i = 0; i <= max; ++i) {
        sem_wait(&vazio);
        int produto;
        if (i == max) { 
            produto = -1;          //envia produto sinlizando FIM
        } else {
            produto = produzir(i); //produz um elemento normal
        }
        indice_produtor = (indice_produtor + 1) % tamanho_buffer; //calcula posição próximo elemento
        buffer[indice_produtor] = produto; //adiciona o elemento produzido à lista
        sem_post(&cheio);
    }
    return NULL;
}

void *consumidor_func(void *arg) {
    while (1) {
        sem_wait(&cheio);
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer; //Calcula o próximo item a consumir
        int produto = buffer[indice_consumidor]; //obtém o item da lista
        //Podemos receber um produto normal ou um produto especial
        if (produto >= 0) {
            consumir(produto); //Consome o item obtido.
        } else {
            break; //produto < 0 é um sinal de que o consumidor deve parar
        }
        sem_post(&vazio);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s tamanho_buffer itens_produzidos\n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    int n_itens = atoi(argv[2]);
    printf("n_itens: %d\n", n_itens);

    //Iniciando buffer
    indice_produtor = 0;
    indice_consumidor = 0;
    buffer = malloc(sizeof(int) * tamanho_buffer);

    // Crie threads e o que mais for necessário para que uma produtor crie 
    // n_itens produtos e o consumidor os consuma

    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, tamanho_buffer);
    
    pthread_t thread_produtor;
    pthread_t thread_consumidor;
    pthread_create(&thread_produtor, NULL, produtor_func, (void*)&n_itens);
    pthread_create(&thread_consumidor, NULL, consumidor_func, NULL);
    pthread_join(thread_produtor, NULL);
    pthread_join(thread_consumidor, NULL);
    
    sem_destroy(&cheio);
    sem_destroy(&vazio);

    //Libera memória do buffer
    free(buffer);

    return 0;
}

