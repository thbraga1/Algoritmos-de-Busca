#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char *ler_arquivo(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *buffer = (char *)malloc(tamanho + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, tamanho, f);
    buffer[tamanho] = '\0';
    fclose(f);
    return buffer;
}

char **ler_padroes(const char *nome_arquivo, int *num_padroes) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return NULL;

    char linha[1024];
    char **padroes = NULL;
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        padroes = realloc(padroes, (count + 1) * sizeof(char *));
        padroes[count] = strdup(linha);
        count++;
    }

    fclose(f);
    *num_padroes = count;
    return padroes;
}

void liberar_padroes(char **padroes, int num_padroes) {
    for (int i = 0; i < num_padroes; i++) {
        free(padroes[i]);
    }
    free(padroes);
}
