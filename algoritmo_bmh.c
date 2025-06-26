#include <string.h>
#include <stdio.h>
#include "algoritmo_bmh.h"

// Busca BMH no texto original
int bmh(const char *texto, const char *padrao, int *ocorrencias) {
    int n = strlen(texto);
    int m = strlen(padrao);
    int count = 0;

    if (m == 0 || n == 0 || m > n) return 0;

    int tabela[256];
    for (int i = 0; i < 256; i++) tabela[i] = m;
    for (int i = 0; i < m - 1; i++) tabela[(unsigned char)padrao[i]] = m - i - 1;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && padrao[j] == texto[i + j]) {
            j--;
        }
        if (j < 0) {
            ocorrencias[count++] = i;
            i += tabela[(unsigned char)texto[i + m - 1]];
        } else {
            i += tabela[(unsigned char)texto[i + m - 1]];
        }
    }

    return count;
}

// Busca BMH nos símbolos do texto comprimido
int bmh_simbolos(const SequenciaSimbolos *texto, const SequenciaSimbolos *padrao, int *ocorrencias) {
    int n = texto->tamanho;
    int m = padrao->tamanho;
    int count = 0;

    if (m == 0 || n == 0 || m > n) return 0;

    int tabela[256];
    for (int i = 0; i < 256; i++) tabela[i] = m;
    for (int i = 0; i < m - 1; i++) {
        tabela[(unsigned char)padrao->dados[i]] = m - i - 1;
    }

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && padrao->dados[j] == texto->dados[i + j]) {
            j--;
        }
        if (j < 0) {
            ocorrencias[count++] = i;
            i += tabela[(unsigned char)texto->dados[i + m - 1]];
        } else {
            i += tabela[(unsigned char)texto->dados[i + m - 1]];
        }
    }

    return count;
}
