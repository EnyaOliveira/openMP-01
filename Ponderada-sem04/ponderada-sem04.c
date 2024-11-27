#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000 // Define o tamanho do vetor como 100 milhões de elementos

// Função para inicializar o vetor com valores aleatórios entre 1 e 100
void inicializa_vetor(int *vetor, int tamanho) {
    srand(time(NULL)); // Define uma semente baseada no tempo atual para gerar números aleatórios
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 100 + 1; // Gera valores entre 1 e 100
    }
}

// Versão serial do cálculo do somatório
long long soma_serial(int *vetor, int tamanho) {
    long long soma = 0;
    // Percorre todos os elementos do vetor e acumula o somatório
    for (int i = 0; i < tamanho; i++) {
        soma += vetor[i];
    }
    return soma; // Retorna o somatório calculado
}

// Versão paralela do cálculo do somatório utilizando OpenMP
long long soma_paralela(int *vetor, int tamanho, int num_threads) {
    long long soma = 0;

    // Define o número de threads a serem usadas na região paralela
    omp_set_num_threads(num_threads);

    // Diretiva para paralelizar o loop
    #pragma omp parallel for reduction(+:soma)
    for (int i = 0; i < tamanho; i++) {
        soma += vetor[i]; // Cada thread soma sua parte do vetor em uma variável local
    }

    return soma; // Retorna o somatório combinado das threads
}

int main() {
    // Aloca memória para o vetor de tamanho N
    int *vetor = (int *)malloc(N * sizeof(int));
    if (vetor == NULL) {
        // Verifica se a alocação falhou
        printf("Erro ao alocar memória!\n");
        return -1;
    }

    inicializa_vetor(vetor, N); // Inicializa o vetor com números aleatórios

    double inicio_serial = omp_get_wtime(); // Captura o tempo inicial
    long long soma_serial_resultado = soma_serial(vetor, N); // Calcula a soma de forma serial
    double tempo_serial = omp_get_wtime() - inicio_serial; // Calcula o tempo total gasto

    // Imprime o resultado da soma serial e o tempo de execução
    printf("Versão Serial: Soma = %lld, Tempo = %f segundos\n", soma_serial_resultado, tempo_serial);

    // *** Cálculo paralelo com diferentes números de threads ***
    for (int threads = 1; threads <= 8; threads *= 2) { // Testa com 1, 2, 4 e 8 threads
        double inicio_paralelo = omp_get_wtime(); // Captura o tempo inicial
        long long soma_paralela_resultado = soma_paralela(vetor, N, threads); // Calcula a soma de forma paralela
        double tempo_paralelo = omp_get_wtime() - inicio_paralelo; // Calcula o tempo total gasto

        // Imprime o resultado da soma paralela, número de threads e tempo de execução
        printf("Versão Paralela (%d threads): Soma = %lld, Tempo = %f segundos\n", 
               threads, soma_paralela_resultado, tempo_paralelo);
    }

    free(vetor); // Libera a memória alocada para o vetor

    return 0; // Finaliza o programa
}
