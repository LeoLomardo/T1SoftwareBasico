/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena -  - 3WB  */

#include "converteutf.h"

/*
int confereTipo(arquivo_entrada){
    unsigned int byteConfere;
    fread(byteConfere,sizeof(int),1, arquivo_entrada);

    if((byteConfere&0xFEFF) == 0xFEFF || (byteConfere&0xFEFF) == 0xFFFE0000){
        converteUtf32Para8(arquivo_entrada, arquivo_saida);
    }else
        converteUtf32Para8(arquivo_entrada, arquivo_saida);


}*/


    //ele pega o 1 byte = 1 char, verifica se o bit mais significativo é 0 11 111 1110, dividindo em 4 casos possíveil
int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida){
    unsigned int auxLeitura;   //caracter que le byte por byte 
    char byteSaida[4] = {0x00, 0x00, 0x00, 0x00};

    for(auxLeitura = fgetc(arquivo_entrada); auxLeitura !=EOF; auxLeitura = fgetc(arquivo_entrada)){
        //realizei a leitura utilizando unsigned int, ja que o caracter de maior tamanho e 4 bytes
            //8 bists char 
        if(auxLeitura <= 0x7F){
            byteSaida[0] = auxLeitura;
            //16 bits char
            fwrite(byteSaida, sizeof(char), 1, arquivo_saida);
        }else if(auxLeitura <= 0x7FF){
            byteSaida[0] = 0xC0 | (auxLeitura >> 6); //110xxxxx || auxLeitura 6 bits pra direita para armazenar os dados do 1 byte
            byteSaida[1] = 0x80 | (auxLeitura & 0x3F); //10xxxxxx || auxLeitura & 0x3F
            fwrite(byteSaida, sizeof(char), 2, arquivo_saida);
            //24 bits char
        }else if(auxLeitura <=0xFFFF){
            byteSaida[0] = 0xE0 | (auxLeitura >> 12); //1110xxxx || 1 byte auxLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((auxLeitura >> 6) & 0x3F); //10xxxxxx || auxLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | (auxLeitura & 0X3F); //10xxxxxx || auxLeitura 2 bits pra direita
            fwrite(byteSaida, sizeof(char), 3, arquivo_saida);

            //32 bits
        }else if(auxLeitura <= 0x10FFFF){
            byteSaida[0] = 0xF0 | (auxLeitura >> 18); //11110xxx ||  1 byte auxLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((auxLeitura >> 12) & 0X3F); //10xxxxxx || auxLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | ((auxLeitura >> 6) & 0X3F); //10xxxxxx || auxLeitura 2 bits pra direita
            byteSaida[2] = 0x80 | (auxLeitura & 0x3F); //10xxxxxx || auxLeitura 2 bits pra direita
            fwrite(byteSaida, sizeof(char), 4, arquivo_saida);

        }else{
            printf("Caracter fora do alcance.c\n");
        }
    }
    //como esta escrevendo em utf32 bits, falta implementar o BOM
    //PRECISA VERIRICAR SE ESTÁ ESCREVENDO NA ORDENACAO LITTLE ENDIAN, talvez precise fazer uma funcao aux void Imprime( char *byteSaida, int numBytesSaida)
}
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){

}