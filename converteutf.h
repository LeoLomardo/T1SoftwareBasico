#include <stdio.h>

int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida);
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida);
int utf8_size(unsigned char aux);
int verificaBom(unsigned int bom);