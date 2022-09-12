/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena -  - 3WB  */

#include "converteutf.h"

int confere_endian(unsigned int bom){
    if(bom == 0x0000FEFF)
        return 'b';
    if(bom == 0xFFFE0000)  //big endian
        return 'l';
    
}
//le o byte e ve se precisa do byte seguinte para montar o caractere
int utf8_size(unsigned char aux){
    if((aux>>7) == 0x00){
        printf("Char de 1 byte");
        return 1;
    }else if((aux >> 5) == 0xC0){
        printf("Char de 2 byte");
        return 2;
    }else if((aux>>4) == 0xE0){
        printf("Char de 3 byte");
        return 3;
    }else if((aux>>5) == 0xF0){
        printf("Char de 4 byte");
        return 4;
    }else{
        printf("Fora do alcance");
        return -1;
    }
}
//numero de caracteres no arquivo( == numero de bytes)
int numChar(FILE *arquivo_entrada){
    char c;
    int count = -1;
    for(c = fgetc(arquivo_entrada); c!=EOF; c = fgetc(arquivo_entrada)){
        count+=1;
    }
    return count;
}

int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida){
    
    
}
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){
    
}