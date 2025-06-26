#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define MAX_TREE_NODES 512
#define MAX_CODE_LEN 256

typedef struct {
    unsigned char bits[MAX_CODE_LEN];
    int length;
} HuffmanCode;

static HuffmanCode codes[256];

// Estrutura para armazenar dados comprimidos como bits
typedef struct {
    unsigned char *bytes;
    int num_bits;
    int capacity_bytes;
} BitStream;

BitStream* criar_bitstream() {
    BitStream *bs = malloc(sizeof(BitStream));
    bs->capacity_bytes = 1024;
    bs->bytes = malloc(bs->capacity_bytes);
    bs->num_bits = 0;
    return bs;
}

void adicionar_bit(BitStream *bs, int bit) {
    int byte_pos = bs->num_bits / 8;
    int bit_pos = bs->num_bits % 8;
    
    // Expandir se necessário
    if (byte_pos >= bs->capacity_bytes) {
        bs->capacity_bytes *= 2;
        bs->bytes = realloc(bs->bytes, bs->capacity_bytes);
    }
    
    // Inicializar byte se for a primeira vez
    if (bit_pos == 0) {
        bs->bytes[byte_pos] = 0;
    }
    
    // Adicionar bit
    if (bit) {
        bs->bytes[byte_pos] |= (1 << (7 - bit_pos));
    }
    
    bs->num_bits++;
}

void adicionar_bits(BitStream *bs, unsigned char *bits, int length) {
    for (int i = 0; i < length; i++) {
        adicionar_bit(bs, bits[i]);
    }
}

int obter_bit(BitStream *bs, int pos) {
    if (pos >= bs->num_bits) return -1;
    int byte_pos = pos / 8;
    int bit_pos = pos % 8;
    return (bs->bytes[byte_pos] >> (7 - bit_pos)) & 1;
}

void liberar_bitstream(BitStream *bs) {
    free(bs->bytes);
    free(bs);
}

// Cria um novo nó
HuffmanNode *new_node(unsigned char c, int freq, HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode *n = malloc(sizeof(HuffmanNode));
    n->c = c;
    n->freq = freq;
    n->left = left;
    n->right = right;
    return n;
}

// Comparador para qsort
int compare(const void *a, const void *b) {
    HuffmanNode *n1 = *(HuffmanNode **)a;
    HuffmanNode *n2 = *(HuffmanNode **)b;
    return n1->freq - n2->freq;
}

// Constrói árvore de Huffman
HuffmanNode *construir_arvore_huffman(const char *input) {
    int freq[256] = {0};
    int len = strlen(input);
    
    // Conta frequência dos caracteres
    for (int i = 0; i < len; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    // Cria nós folha
    HuffmanNode *nodes[256];
    int node_count = 0;
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[node_count++] = new_node(i, freq[i], NULL, NULL);
        }
    }
    
    // Caso especial: apenas um caractere
    if (node_count == 1) {
        return new_node(0, nodes[0]->freq, nodes[0], NULL);
    }
    
    // Constrói árvore
    while (node_count > 1) {
        qsort(nodes, node_count, sizeof(HuffmanNode *), compare);
        
        HuffmanNode *left = nodes[0];
        HuffmanNode *right = nodes[1];
        HuffmanNode *parent = new_node(0, left->freq + right->freq, left, right);
        
        nodes[0] = parent;
        for (int i = 1; i < node_count - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        node_count--;
    }
    
    return nodes[0];
}

// Gera códigos recursivamente
void gerar_codigos_rec(HuffmanNode *node, unsigned char *code, int depth) {
    if (!node) return;
    
    // Nó folha
    if (!node->left && !node->right) {
        codes[node->c].length = depth;
        memcpy(codes[node->c].bits, code, depth);
        return;
    }
    
    // Percorre filhos
    if (node->left) {
        code[depth] = 0;
        gerar_codigos_rec(node->left, code, depth + 1);
    }
    if (node->right) {
        code[depth] = 1;
        gerar_codigos_rec(node->right, code, depth + 1);
    }
}

void gerar_codigos(HuffmanNode *tree) {
    memset(codes, 0, sizeof(codes));
    unsigned char temp_code[MAX_CODE_LEN];
    
    // Caso especial: árvore com um só nó
    if (!tree->left && !tree->right) {
        codes[tree->c].length = 1;
        codes[tree->c].bits[0] = 0;
        return;
    }
    
    gerar_codigos_rec(tree, temp_code, 0);
}

// Compressão com marcação
HuffmanResult huffman_comprimir(const char *input, HuffmanNode *tree) {
    gerar_codigos(tree);
    
    BitStream *bs = criar_bitstream();
    int len = strlen(input);
    
    for (int i = 0; i < len; i++) {
        unsigned char c = input[i];
        HuffmanCode *code = &codes[c];
        
        // Adiciona código do caractere
        adicionar_bits(bs, code->bits, code->length);
        
        // Adiciona bit de marcação (1)
        adicionar_bit(bs, 1);
    }
    
    HuffmanResult result;
    result.data = (char*)bs->bytes;
    result.size_bits = bs->num_bits;
    
    // Não liberar bs->bytes pois será usado no resultado
    free(bs);
    
    return result;
}

// Descompressão
char *huffman_descomprimir(const HuffmanResult *compressed, HuffmanNode *tree) {
    BitStream bs;
    bs.bytes = (unsigned char*)compressed->data;
    bs.num_bits = compressed->size_bits;
    
    char *output = malloc(compressed->size_bits); // Estimativa
    int output_pos = 0;
    int bit_pos = 0;
    
    while (bit_pos < bs.num_bits) {
        HuffmanNode *current = tree;
        
        // Navegar na árvore até encontrar folha
        while (current->left || current->right) {
            if (bit_pos >= bs.num_bits) break;
            
            int bit = obter_bit(&bs, bit_pos++);
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            
            if (!current) break;
        }
        
        if (current && (!current->left && !current->right)) {
            output[output_pos++] = current->c;
            
            // Pular bit de marcação
            if (bit_pos < bs.num_bits) {
                bit_pos++; // Pula o bit de marcação
            }
        } else {
            break;
        }
    }
    
    output[output_pos] = '\0';
    return output;
}

// Converte resultado comprimido para sequência de símbolos (caracteres decodificados)
SequenciaSimbolos converter_huffman_para_simbolos(const char *input, HuffmanNode *tree) {
    gerar_codigos(tree);
    
    SequenciaSimbolos seq;
    seq.dados = malloc(strlen(input));
    seq.tamanho = 0;
    
    // Para Huffman com marcação, cada caractere vira um símbolo
    for (int i = 0; input[i]; i++) {
        seq.dados[seq.tamanho++] = input[i];
    }
    
    return seq;
}

void liberar_arvore_huffman(HuffmanNode *node) {
    if (!node) return;
    liberar_arvore_huffman(node->left);
    liberar_arvore_huffman(node->right);
    free(node);
}

void liberar_huffman(HuffmanResult *compressed) {
    free(compressed->data);
    compressed->data = NULL;
    compressed->size_bits = 0;
}