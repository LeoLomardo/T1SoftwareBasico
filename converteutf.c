/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena - 2010796 - 3WB */

#include "converteutf.h"


int utf8_size(unsigned char aux){
    if((aux>>3) == 0x1E){
        printf("Char de 4 byte");
        return 4;
    }else if((aux >> 4) == 0xE){
        printf("Char de 3 byte");
        return 3;
    }else if((aux>>5) == 0x6){
        printf("Char de 2 byte");
        return 2;
    }
    printf("Char de 1 byte");
    return 1;
}


int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida){
    unsigned char percorreArq;
    unsigned int escreveByte;
    int numBytes = 1;
    unsigned char aux1 = 0;
    unsigned char aux2 = 0;
    unsigned char aux3 = 0;

    
    unsigned int bom = 0x0000feff;
    fwrite(&bom, 4,1,arquivo_saida);
    fread(&percorreArq,sizeof(char),1,arquivo_entrada);

    do{
         numBytes = utf8_size(percorreArq);

            if(numBytes > 4){
                printf("Estrapolou limite de 4 bytes por char.");
                return -1;

            }
            if(numBytes == 1){ 
                printf("1\n");
                escreveByte  = 0x00 | percorreArq;
            }
            if(numBytes == 2){
                printf("2\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                aux1 = (aux1<<1) |0x00;

                percorreArq = (percorreArq <<2) | 0x00;
                escreveByte = (escreveByte <<8) |aux1;
                escreveByte = escreveByte | percorreArq;
            }
            if(numBytes == 3){
                printf("3\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                fread(&aux2, sizeof(char),1,arquivo_entrada);
                
                aux1 = (aux1<<1) |0x00;
                aux2 = (aux2<<1) |0x00;

                percorreArq = (percorreArq <<3) | 0x00;
                escreveByte = (escreveByte <<16) |aux2;
                escreveByte = (escreveByte <<8) |aux1;
                escreveByte = escreveByte | percorreArq;
            }
            if(numBytes == 4){
                printf("4\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                fread(&aux2, sizeof(char),1,arquivo_entrada);
                fread(&aux3, sizeof(char),1,arquivo_entrada);

                aux1 = (aux1<<1) |0x00;
                aux2 = (aux2<<1) |0x00;
                aux3 = (aux3<<1) |0x00;

                percorreArq = (percorreArq <<4) | 0x00;
                escreveByte = (escreveByte <<24) |aux2;
                escreveByte = (escreveByte <<16) |aux2;
                escreveByte = (escreveByte <<8) |aux1;
                escreveByte = escreveByte | percorreArq;
            }
            if(fwrite(&escreveByte,4,1,arquivo_saida) !=1){
                printf("Erro ao escrever no arquivo");
                return -1;
            } 

        }while(fread(&percorreArq,sizeof(char),1,arquivo_entrada));
    
    return 0;
}
/*          REFAZER TUDO DAQUI PRA BAIXO*/
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){
    unsigned char resultado[4] = {0x00, 0x80, 0x80, 0x80}; //independente do tamanho, do segundo byte em diante ele vai comecar com 0x80 
    unsigned char bom;
    unsigned int leitura;
    int sizeBytes = numChar(arquivo_entrada);
    unsigned char lixo;
    fread(&leitura, sizeof(int), 1, arquivo_entrada);

    //utilizei a funcao que confere a endian do arquivo atraves do bom para verificar se o bom esta presente
    if(confere_endian(leitura) == 'e'){
        printf("BOM invalido\n");
        return -1;
    }else if(confere_endian(leitura) == 'b'){
        inverteEndian(leitura);
    }else if(confere_endian(leitura) == 'l'){
        for(lixo = fgetc(arquivo_entrada); lixo !=EOF; lixo = fgetc(arquivo_entrada)){
            read(&leitura, sizeof(int), 1, arquivo_entrada);
            if(leitura > 0x10FFFF){
                printf("Ultrapassou valor maximo (U+10FFFF)\n"); //no enunciado fala que nao precisa, mantive apenas para organizar pensamento
                return -1;
            }

        }
    }

}
