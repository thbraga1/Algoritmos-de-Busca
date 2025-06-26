#ifndef UTILS_SIMBOLOS_H
#define UTILS_SIMBOLOS_H

typedef struct {
    char *dados;
    int tamanho;
} SequenciaSimbolos;

// Função original (se ainda precisar)
SequenciaSimbolos converter_para_simbolos(const unsigned char *bits, int tamanho_bits);

// Função converter_huffman_para_simbolos está declarada em huffman.h

void liberar_sequencia(SequenciaSimbolos *seq);

#endif