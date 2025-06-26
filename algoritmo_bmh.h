#ifndef ALGORITMO_BMH_H
#define ALGORITMO_BMH_H

#include "utils_simbolos.h"

int bmh(const char *texto, const char *padrao, int *ocorrencias);
int bmh_simbolos(const SequenciaSimbolos *texto, const SequenciaSimbolos *padrao, int *ocorrencias);

#endif
