#include <stdlib.h>
#include <string.h>
#include "utils_simbolos.h"
#include "huffman.h"

// Função original (se você ainda usar)
SequenciaSimbolos converter_para_simbolos(const unsigned char *bits, int tamanho_bits) {
    SequenciaSimbolos seq;
    seq.dados = (char *)malloc(tamanho_bits);
    seq.tamanho = 0;

    int i = 0;
    unsigned char atual = 0;
    int bits_lidos = 0;

    while (i < tamanho_bits) {
        if (bits[i] == 1) {
            seq.dados[seq.tamanho++] = atual;
            atual = 0;
            bits_lidos = 0;
        } else {
            atual = (atual << 1) | bits[i];
            bits_lidos++;
        }
        i++;
    }

    return seq;
}

// Função removida - está implementada em huffman.c

void liberar_sequencia(SequenciaSimbolos *seq) {
    if (seq->dados) {
        free(seq->dados);
        seq->dados = NULL;
    }
    seq->tamanho = 0;
}