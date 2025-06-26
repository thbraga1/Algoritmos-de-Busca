#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "algoritmo_pd.h"
#include "algoritmo_shiftand.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_texto> <arquivo_padroes>\n", argv[0]);
        return 1;
    }

    char *arquivo_texto = argv[1];
    char *arquivo_padroes = argv[2];

    char *texto = ler_arquivo(arquivo_texto);
    if (!texto) {
        printf("Erro ao ler arquivo de texto.\n");
        return 1;
    }

    int num_padroes = 0;
    char **padroes = ler_padroes(arquivo_padroes, &num_padroes);
    if (!padroes) {
        printf("Erro ao ler arquivo de padrões.\n");
        free(texto);
        return 1;
    }

    int k;
    printf("Digite o número de erros permitidos (k): ");
    if (scanf("%d", &k) != 1) {
        printf("Erro na leitura de k.\n");
        return 1;
    }

    for (int p = 0; p < num_padroes; p++) {
        char *padrao = padroes[p];
        printf("\n==============================\n");
        printf("Padrão: %s\n", padrao);

        // -------- Programação Dinâmica --------
        struct timeval inicio_pd, fim_pd;
        gettimeofday(&inicio_pd, NULL);

        int ocorrencias_pd[1000];
        printf("[Programação Dinâmica] Ocorrências: ");
        int count_pd = busca_pd(texto, padrao, k, ocorrencias_pd, 1000);
        for (int i = 0; i < count_pd; i++) {
            printf("%d ", ocorrencias_pd[i] + 1);
        }

        gettimeofday(&fim_pd, NULL);
        double tempo_pd = (fim_pd.tv_sec - inicio_pd.tv_sec) * 1000.0;
        tempo_pd += (fim_pd.tv_usec - inicio_pd.tv_usec) / 1000.0;
        printf("\nTempo PD: %.3lf ms\n", tempo_pd);

        // -------- Shift-And --------
        struct timeval inicio_sa, fim_sa;
        gettimeofday(&inicio_sa, NULL);

        int ocorrencias_sa[1000];
        printf("[Shift-And] Ocorrências: ");
        int count_sa = busca_shiftand(texto, padrao, k, ocorrencias_sa, 1000);
        for (int i = 0; i < count_sa; i++) {
            printf("%d ", ocorrencias_sa[i] + 1);
        }

        gettimeofday(&fim_sa, NULL);
        double tempo_sa = (fim_sa.tv_sec - inicio_sa.tv_sec) * 1000.0;
        tempo_sa += (fim_sa.tv_usec - inicio_sa.tv_usec) / 1000.0;
        printf("\nTempo Shift-And: %.3lf ms\n", tempo_sa);

        printf("==============================\n");
    }

    free(texto);
    liberar_padroes(padroes, num_padroes);
    return 0;
}
