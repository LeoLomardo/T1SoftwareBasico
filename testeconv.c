#include <stdio.h>
#include <stdlib.h>
#include "converteutf.h"

int main(void){

    FILE *arquivo_entrada8;
    FILE *arquivo_entrada32;
    FILE *arquivo_saida8;
    FILE *arquivo_saida32;

    arquivo_entrada8 = fopen("utf8.bin", "rb");
    arquivo_entrada32 = fopen("utf32.bin", "rb");

    arquivo_saida8 = fopen("utf8Saida.bin", "wb");
    arquivo_saida32 = fopen("utf32Saida.bin", "wb");

    converteUtf8Para32(arquivo_entrada8,arquivo_saida32);
    converteUtf32Para8(arquivo_entrada32, arquivo_saida8);

    fclose(arquivo_entrada32);
    fclose(arquivo_entrada8);
    fclose(arquivo_saida8);
    fclose(arquivo_saida32);
    return 0;
}
