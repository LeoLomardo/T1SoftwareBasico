#include "converteutf.h"
#include <stdio.h>


int main(void){
    FILE *arquivo_entrada;


    arquivo_entrada = fopen("utf32.bin", "rb");
       //ler primeiros 4 bytes =  ler 4 char 
     int bom  = 0xFFFE0000;
     int check = 0;

    fread(&check, sizeof(int),1, arquivo_entrada);
    printf("%d", check);
    

    fclose(arquivo_entrada);
    return 0;
}