#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

typedef struct payload {
    double* a;
    double* b;
    double* c;
    int start;
    int end;
} scalar_payload;

void* scalar_product(void* arg) {
    scalar_payload op = *((scalar_payload*) arg);
    
    for (size_t i = op.start; i < op.end; ++i) {
        *(op.c) += op.a[i] * op.b[i];
    }
    op.c++;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    if (n_threads > a_size) {
        n_threads = a_size;
    }
    
    scalar_payload payloads[n_threads];
    pthread_t threads[n_threads];
    int sums_per_thread = a_size / n_threads;
    
    double* c = calloc(n_threads, n_threads * sizeof(double));
    double result = 0;

    for (int i = 0; i < n_threads; ++i) {
        int start = i * sums_per_thread;
        int end = start + sums_per_thread;

        if (i == n_threads - 1) {
            end = a_size;
        }

        payloads[i].a = a;
        payloads[i].b = b;
        payloads[i].c = c;
        payloads[i].start = start;
        payloads[i].end = end;
        pthread_create(&threads[i], NULL, scalar_product, (void*) &payloads[i]);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (size_t i = 0; i < n_threads; i++) {
        result += c[i];
    }
    
    avaliar(a, b, a_size, result);

    free(a);
    free(b);
    free(c);

    return 0;
}
