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
int verificaBom(unsigned int bom){
    unsigned int bigE_bom = 0x0000FEFF;
    unsigned int littleE_bom = 0xfffe0000;

    if((bom ^ bigE_bom) == 0){
        return 1;
    }else if((bom ^ littleE_bom) ==0){
        return 2;
    }else{
        return -1;
    }

}
/*      FUNCAO ABAIXO ESTA ESCREVENDO 1 CHAR REFERENTE AO : ERRADO, CONFERIR DEPOIS*/
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
                aux1 = aux1>>1;
                percorreArq = (percorreArq <<3) | 0x00;
                percorreArq = (percorreArq >>3) | 0x00;
                escreveByte = (escreveByte <<8) |aux1;
                escreveByte = escreveByte | percorreArq;
            }
            if(numBytes == 3){
                printf("3\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                fread(&aux2, sizeof(char),1,arquivo_entrada);
                
                aux1 = (aux1<<1) |0x00;
                aux2 = (aux2<<1) |0x00;
                aux1 = aux1>>1;
                aux2 = aux2>>1;
                escreveByte = (escreveByte <<16) |aux2;
                escreveByte = (escreveByte <<8) |aux1;

                percorreArq = (percorreArq <<4) | 0x00;
                percorreArq = (percorreArq >>4) | 0x00;
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

                aux1 = aux1>>1;
                aux2 = aux2>>1;
                aux3 = aux3>>1;

                escreveByte = (escreveByte <<24) |aux2;
                escreveByte = (escreveByte <<16) |aux2;
                escreveByte = (escreveByte <<8) |aux1;
                percorreArq = (percorreArq <<5) | 0x00;
                percorreArq = (percorreArq >>5) | 0x00;
                escreveByte = escreveByte | percorreArq;
            }
            if(fwrite(&escreveByte,4,1,arquivo_saida) !=1){
                printf("Erro ao escrever no arquivo");
                return -1;
            } 

        }while(fread(&percorreArq,sizeof(char),1,arquivo_entrada));


    return 0;
}
/*          FUNCAO ABAIXO ESTA ACERTANDO QUASE TODOS OS BYTES, ERRANDO 2 DE 20 */
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){
    unsigned int endian;
    int i = 0;
    unsigned int percorreArq = 0x00;
    unsigned char escreverArq = 0x00;

    
    fread(&percorreArq, sizeof(int), 1 , arquivo_entrada);
    
    endian = verificaBom(percorreArq);
        fread(&percorreArq, sizeof(int), 1 , arquivo_entrada);
        if(endian == 1){
            do{
                escreverArq = 0x00;
                if((percorreArq >= 0x00) && (percorreArq <= 0x007F)){
                    escreverArq =  (percorreArq & 0x7F);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;
                }else if((percorreArq >= 0x0080) && (percorreArq <= 0x07FF)){
                    escreverArq = 0xC0 | ((percorreArq >> 10) & 0xDF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;

                }else if((percorreArq >= 0x0800) && (percorreArq <= 0xFFFF)){
                    escreverArq = 0xE0 | ((percorreArq >> 18) & 0xEF);  //E0 = 11100000 & DF = 11011111
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 12) & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;  

                }else if((percorreArq >= 0x10000) && (percorreArq < 0x10FFFF)){
                    escreverArq = 0xF0 | ((percorreArq >> 24) & 0xF7);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 21) & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 15) & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;
                    
                    escreverArq = 0x80 | (percorreArq>>9 & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;
                }else{
                    printf("ERRO NO CHAR %X -- %c\n", percorreArq,percorreArq);
                }
            escreverArq = 0x00;
            }while(fread(&percorreArq, sizeof(int), 1, arquivo_entrada));

        }else if(endian == 2){


        }else{
            printf("BOM Invalido");
            return 1;
        }


}
