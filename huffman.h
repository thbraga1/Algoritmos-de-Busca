#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "utils_simbolos.h"  // Adicionar esta linha

typedef struct {
    char *data;
    int size_bits;
} HuffmanResult;

typedef struct HuffmanNode {
    unsigned char c;
    int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// Funções originais
HuffmanNode* construir_arvore_huffman(const char *input);
void liberar_arvore_huffman(HuffmanNode *node);
HuffmanResult huffman_comprimir(const char *input, HuffmanNode *tree);
char *huffman_descomprimir(const HuffmanResult *compressed, HuffmanNode *tree);
void liberar_huffman(HuffmanResult *compressed);

// Nova função adicionada
SequenciaSimbolos converter_huffman_para_simbolos(const char *input, HuffmanNode *tree);

#endif