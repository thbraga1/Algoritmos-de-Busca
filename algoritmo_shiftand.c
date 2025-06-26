#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "algoritmo_shiftand.h"

int busca_shiftand(const char *texto, const char *padrao, int k, int *ocorrencias, int max_ocorrencias) {
    int n = strlen(texto);
    int m = strlen(padrao);

    if (m > 63) {
        printf("Shift-And suporta padrões de até 63 caracteres.\n");
        return 0;
    }

    uint64_t B[256];
    uint64_t mask = 1ULL << (m - 1);
    memset(B, 0xFF, sizeof(B));

    for (int i = 0; i < m; i++) {
        B[(unsigned char)padrao[i]] &= ~(1ULL << i);
    }

    uint64_t *D = (uint64_t *)malloc((k + 1) * sizeof(uint64_t));
    for (int e = 0; e <= k; e++) {
        D[e] = ~0ULL;
    }

    int count = 0;

    for (int j = 0; j < n; j++) {
        uint64_t old_Dk_1 = D[0];

        D[0] = ((D[0] << 1) | B[(unsigned char)texto[j]]);

        for (int e = 1; e <= k; e++) {
            uint64_t temp = D[e];
            D[e] = ((D[e] << 1) | B[(unsigned char)texto[j]]) & 
                   ((old_Dk_1 << 1) | (old_Dk_1)) & 
                   (D[e - 1] << 1) & (D[e - 1]);
            old_Dk_1 = temp;
        }

        if (~D[k] & mask) {
            if (count < max_ocorrencias) {
                ocorrencias[count++] = j - m + 1; // Posição 1-indexada
            }
        }
    }

    free(D);
    return count;
}
