#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "algoritmo_bmh.h"
#include "huffman.h" 
#include "utils.h"

// Função para contar comparações no BMH
int bmh_com_contador(const char *texto, const char *padrao, int *ocorrencias, int *comparacoes) {
    int n = strlen(texto);
    int m = strlen(padrao);
    int count = 0;
    *comparacoes = 0;

    if (m == 0 || n == 0 || m > n) return 0;

    int tabela[256];
    for (int i = 0; i < 256; i++) tabela[i] = m;
    for (int i = 0; i < m - 1; i++) tabela[(unsigned char)padrao[i]] = m - i - 1;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && ((*comparacoes)++, padrao[j] == texto[i + j])) {
            j--;
        }
        if (j < 0) {
            ocorrencias[count++] = i;
            i += (i + m - 1 < n) ? tabela[(unsigned char)texto[i + m - 1]] : 1;
        } else {
            i += (i + m - 1 < n) ? tabela[(unsigned char)texto[i + m - 1]] : 1;
        }
    }

    return count;
}

// Converte dados binários para string hexadecimal para busca
char* dados_para_hex(const char *dados, int size_bits) {
    int size_bytes = (size_bits + 7) / 8;
    char *hex = malloc(size_bytes * 2 + 1);
    
    for (int i = 0; i < size_bytes; i++) {
        sprintf(hex + i * 2, "%02x", (unsigned char)dados[i]);
    }
    hex[size_bytes * 2] = '\0';
    return hex;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_texto> <arquivo_padroes>\n", argv[0]);
        return 1;
    }

    char *arquivo_texto = argv[1];
    char *arquivo_padroes = argv[2];

    // Ler arquivos
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

    // Construir árvore de Huffman
    HuffmanNode *arvore_huffman = construir_arvore_huffman(texto);
    
    // Comprimir o texto
    HuffmanResult texto_comprimido = huffman_comprimir(texto, arvore_huffman);
    
    // Converter para representação hexadecimal para busca
    char *texto_hex = dados_para_hex(texto_comprimido.data, texto_comprimido.size_bits);
    
    printf("Tamanho original: %ld bytes\n", strlen(texto));
    printf("Tamanho comprimido: %d bits (%d bytes)\n", 
           texto_comprimido.size_bits, (texto_comprimido.size_bits + 7) / 8);
    printf("Taxa de compressão: %.2f%%\n\n", 
           (1.0 - (double)(texto_comprimido.size_bits / 8) / strlen(texto)) * 100);

    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    int total_comparacoes_original = 0;
    int total_comparacoes_comprimido = 0;

    for (int p = 0; p < num_padroes; p++) {
        char *padrao = padroes[p];
        printf("Padrão: %s\n", padrao);

        // ===== BUSCA NO TEXTO ORIGINAL =====
        struct timeval inicio_orig, fim_orig;
        gettimeofday(&inicio_orig, NULL);
        
        int ocorrencias_orig[1000];
        int comparacoes_orig = 0;
        int count_orig = bmh_com_contador(texto, padrao, ocorrencias_orig, &comparacoes_orig);
        
        gettimeofday(&fim_orig, NULL);
        double tempo_orig = (fim_orig.tv_sec - inicio_orig.tv_sec) * 1000.0;
        tempo_orig += (fim_orig.tv_usec - inicio_orig.tv_usec) / 1000.0;
        
        printf("Original: ");
        for (int i = 0; i < count_orig; i++) {
            printf("%d ", ocorrencias_orig[i] + 1);
        }
        if (count_orig == 0) printf("Nenhuma");
        printf(" (Tempo: %.3f ms, Comparações: %d)\n", tempo_orig, comparacoes_orig);
        
        total_comparacoes_original += comparacoes_orig;

        // ===== BUSCA NO TEXTO COMPRIMIDO =====
        struct timeval inicio_comp, fim_comp;
        gettimeofday(&inicio_comp, NULL);
        
        // Comprimir o padrão
        HuffmanResult padrao_comprimido = huffman_comprimir(padrao, arvore_huffman);
        char *padrao_hex = dados_para_hex(padrao_comprimido.data, padrao_comprimido.size_bits);
        
        int ocorrencias_comp[1000];
        int comparacoes_comp = 0;
        int count_comp = bmh_com_contador(texto_hex, padrao_hex, ocorrencias_comp, &comparacoes_comp);
        
        gettimeofday(&fim_comp, NULL);
        double tempo_comp = (fim_comp.tv_sec - inicio_comp.tv_sec) * 1000.0;
        tempo_comp += (fim_comp.tv_usec - inicio_comp.tv_usec) / 1000.0;
        
        printf("Comprimido: ");
        for (int i = 0; i < count_comp; i++) {
            printf("%d ", ocorrencias_comp[i] + 1);
        }
        if (count_comp == 0) printf("Nenhuma");
        printf(" (Tempo: %.3f ms, Comparações: %d)\n", tempo_comp, comparacoes_comp);
        
        total_comparacoes_comprimido += comparacoes_comp;

        // Limpeza
        liberar_huffman(&padrao_comprimido);
        free(padrao_hex);
        printf("\n");
    }

    gettimeofday(&fim, NULL);
    double tempo_total = (fim.tv_sec - inicio.tv_sec) * 1000.0;
    tempo_total += (fim.tv_usec - inicio.tv_usec) / 1000.0;
    
    printf("=== RESUMO DE DESEMPENHO ===\n");
    printf("Tempo total: %.3f ms\n", tempo_total);
    printf("Total de comparações (original): %d\n", total_comparacoes_original);
    printf("Total de comparações (comprimido): %d\n", total_comparacoes_comprimido);
    printf("Redução de comparações: %.2f%%\n", 
           (1.0 - (double)total_comparacoes_comprimido / total_comparacoes_original) * 100);

    // Limpeza final
    free(texto_hex);
    liberar_huffman(&texto_comprimido);
    liberar_arvore_huffman(arvore_huffman);
    free(texto);
    liberar_padroes(padroes, num_padroes);

    return 0;
}