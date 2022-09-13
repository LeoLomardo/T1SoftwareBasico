#include "converteutf.h"
#include <stdio.h>

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


//fgetc sempre le char por char, independente do tamanho da variavel q ta armazenando
//numero de caracteres no arquivo( == numero de bytes)
int numChar(FILE *arquivo_entrada){
    char c;
    int count = 0;
    for(c = fgetc(arquivo_entrada); c!=EOF; c = fgetc(arquivo_entrada)){
        count++;
    }
    return count ;
}
int main(void){

    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_saida = fopen("utf32Saida.bin", "wb");
    arquivo_entrada = fopen("utf32.bin", "rb");
    unsigned int aux;


    int size = numChar(arquivo_entrada);
    printf("%d", size);


    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}


/*
  //ler primeiros 4 bytes =  ler 4 char 

  //essa funcao ta +- certa, ela ta lendo a cada 4 bytes, precisamos arrumar ela pra ela ler byte por byte e ver se é char 1, 2,3 ou 4 bytes, dai traduzir 
    unsigned int  auxLeitura = 0;   //caracter que le byte por byte 
    char bom [4] = {0xFF, 0xFE, 0x00, 0x00};
    fwrite(bom, sizeof(bom),1, arquivo_saida);
    
    for(auxLeitura = fgetc(arquivo_entrada); auxLeitura !=EOF; auxLeitura = fgetc(arquivo_entrada)){
            char byteSaida[4] = {0x00, 0x00, 0x00, 0x00};

        if(auxLeitura <= 0x7F){
            byteSaida[0] = auxLeitura;
            fwrite(byteSaida,sizeof(char),1,arquivo_saida);
            //16 bits char
        }else if(auxLeitura >= 0x80 && auxLeitura <= 0x7FF){
            byteSaida[0] = 0xC0 | (auxLeitura >> 6); //110xxxxx || auxLeitura 6 bits pra direita para armazenar os dados do 1 byte
            byteSaida[1] = 0x80 | (auxLeitura & 0x3F); //10xxxxxx || auxLeitura & 0x3F            
            //24 bits char
            fwrite(byteSaida,sizeof(char),1,arquivo_saida);

        }else if(auxLeitura >=0x800 && auxLeitura <= 0xFFFF){
            byteSaida[0] = 0xE0 | (auxLeitura >> 12); //1110xxxx || 1 byte auxLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((auxLeitura >> 6) & 0x3F); //10xxxxxx || auxLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | (auxLeitura & 0X3F); //10xxxxxx || auxLeitura 2 bits pra direita
                        fwrite(byteSaida,sizeof(char),1,arquivo_saida);

            //32 bits
        }else if(auxLeitura >=0x10000 && auxLeitura <= 0x10FFFF){
            byteSaida[0] = 0xF0 | (auxLeitura >> 18); //11110xxx ||  1 byte auxLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((auxLeitura >> 12) & 0X3F); //10xxxxxx || auxLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | ((auxLeitura >> 6) & 0X3F); //10xxxxxx || auxLeitura 2 bits pra direita
            byteSaida[3] = 0x80 | (auxLeitura & 0x3F); //10xxxxxx || auxLeitura 2 bits pra direita
            fwrite(byteSaida, sizeof(byteSaida), 1, arquivo_saida);
        }else{
            printf("Caracter invalido");
        }

*/