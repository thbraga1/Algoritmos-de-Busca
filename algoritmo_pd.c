#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo_pd.h"

int busca_pd(const char *texto, const char *padrao, int k, int *ocorrencias, int max_ocorrencias) {
    int n = strlen(texto);
    int m = strlen(padrao);
    int count = 0;

    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    // Inicialização
    for (int j = 0; j <= n; j++) dp[0][j] = 0;
    for (int i = 1; i <= m; i++) dp[i][0] = i;

    // Preenchimento da matriz
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int custo = (padrao[i - 1] == texto[j - 1]) ? 0 : 1;
            int insercao = dp[i][j - 1] + 1;
            int delecao = dp[i - 1][j] + 1;
            int substituicao = dp[i - 1][j - 1] + custo;
            dp[i][j] = (insercao < delecao ? 
                        (insercao < substituicao ? insercao : substituicao) : 
                        (delecao < substituicao ? delecao : substituicao));
        }
    }

    // Verificação das ocorrências
    for (int j = 1; j <= n; j++) {
        if (dp[m][j] <= k) {
            if (count < max_ocorrencias) {
                ocorrencias[count++] = j - m;
            }
        }
    }

    // Liberação de memória
    for (int i = 0; i <= m; i++) free(dp[i]);
    free(dp);

    return count;
}
