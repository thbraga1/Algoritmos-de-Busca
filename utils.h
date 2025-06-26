#ifndef UTILS_H
#define UTILS_H

char *ler_arquivo(const char *nome_arquivo);
char **ler_padroes(const char *nome_arquivo, int *num_padroes);
void liberar_padroes(char **padroes, int num_padroes);

#endif
