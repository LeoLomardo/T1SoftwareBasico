/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena - 2010796 - 3WB */

#include "converteutf.h"


int utf8_size(unsigned char aux){
    if((aux>=0x00 && aux<=0x7F)){
        return 1;
    }else if((aux>=0xC0 && aux<=0xDF)){
        return 2;
    }else if((aux>=0xE0 && aux<= 0xEF)){
        return 3;
    }else{
        return 4;
    }
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
    int numBytes = 0;
    unsigned char aux1 = 0x00;
    unsigned char aux2 = 0x00;
    unsigned char aux3 = 0x00;

    
    unsigned int bom = 0x0000feff;
    fwrite(&bom, 4,1,arquivo_saida);
    fread(&percorreArq,sizeof(char),1,arquivo_entrada);

    do{
        numBytes = utf8_size(percorreArq);
        escreveByte = 0x00;
        aux1 = 0x00;
        aux2= 0x00;
        aux3= 0x00;
            if(numBytes > 4){
                printf("Estrapolou limite de 4 bytes por char.");
                return -1;

            }
            if(numBytes == 1){ 
                escreveByte = 0x00;
                printf("1\n");
                percorreArq = (percorreArq<<1) | 0x00;
                escreveByte  = 0x00 | (percorreArq>>1);
            }else if(numBytes == 2){
                escreveByte = 0x00;
                printf("2\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);

                aux1 = (aux1<<2) |0x00;
                aux1 = aux1>>2;
                //110xxxxx
                percorreArq = (percorreArq <<3) | 0x00;
                percorreArq = (percorreArq >>3) | 0x00;

                escreveByte = 0x00 | percorreArq;
                escreveByte = (escreveByte << 6) |aux1;

            }else if(numBytes == 3){
                escreveByte = 0x00;
                printf("3\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                fread(&aux2, sizeof(char),1,arquivo_entrada);

                aux1 = (aux1<<2) |0x00;
                aux2 = (aux2<<2) |0x00;
                aux1 = aux1>>2;
                aux2 = aux2>>2;
                //1110xxxx
                percorreArq = (percorreArq <<4) | 0x00;
                percorreArq = (percorreArq >>4) | 0x00;

                escreveByte = 0x00 | percorreArq;

                escreveByte = (escreveByte <<6) |aux1;
                escreveByte = (escreveByte <<6) |aux2;
                

                
            }else if(numBytes == 4){
                escreveByte = 0x00;
                printf("4\n");
                fread(&aux1, sizeof(char),1,arquivo_entrada);
                fread(&aux2, sizeof(char),1,arquivo_entrada);
                fread(&aux3, sizeof(char),1,arquivo_entrada);

                aux1 = (aux1<<2) |0x00;
                aux2 = (aux2<<2) |0x00;
                aux3 = (aux3<<2) |0x00;

                aux1 = aux1>>2;
                aux2 = aux2>>2;
                aux3 = aux3>>2;
                //11110xxx
                percorreArq = (percorreArq <<5) | 0x00;
                percorreArq = (percorreArq >>5) | 0x00;

                escreveByte = 0x00 | percorreArq;

                escreveByte = (escreveByte <<6) |aux1;
                escreveByte = (escreveByte <<6) |aux2;
                escreveByte = (escreveByte <<6) |aux3;
                
                
                
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
                    escreverArq = 0xC0 | ((percorreArq >> 9) & 0xDF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;

                }else if((percorreArq >= 0x0800) && (percorreArq <= 0xFFFF)){
                    escreverArq = 0xE0 | ((percorreArq >> 14) & 0xEF);  //E0 = 11100000 & DF = 11011111
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 7) & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq>>1 & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;  

                }else if((percorreArq >= 0x10000) && (percorreArq <= 0x10FFFF)){
                    escreverArq = 0xF0 | ((percorreArq >> 29) & 0xF7);  //0xF7 = 11110111  0xF0 = 11110000
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 21) & 0x3F); // 0x3F = 00111111
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 14) & 0x3F);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;
                    
                    escreverArq = 0x80 | (percorreArq>>1 & 0x3F);   //*
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;
                }else{
                    printf("ERRO NO CHAR %X -- %c\n", percorreArq,percorreArq);
                }
            escreverArq = 0x00;
            }while(fread(&percorreArq, sizeof(int), 1, arquivo_entrada));

        }else if(endian == 2){ //FALTA FAZER TUDO ABAIXO 
            do{
                escreverArq = 0x00;
                if((percorreArq >= 0x00) && (percorreArq <= 0x007F)){
                    escreverArq =  (percorreArq & 0x7F);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                }else if((percorreArq >= 0x0080) && (percorreArq <= 0x07FF)){
                    escreverArq = 0xC0 | ((percorreArq >> 9) & 0xDF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;

                }else if((percorreArq >= 0x0800) && (percorreArq <= 0xFFFF)){
                    escreverArq = 0xE0 | ((percorreArq >> 14) & 0xEF);  //E0 = 11100000 & DF = 11011111
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 7) & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | (percorreArq>>1 & 0xBF);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;  

                }else if((percorreArq >= 0x10000) && (percorreArq <= 0x10FFFF)){
                    escreverArq = 0xF0 | ((percorreArq >> 28) & 0xF7);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 21) & 0x3F);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;

                    escreverArq = 0x80 | ((percorreArq >> 14) & 0x3F);
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);
                    escreverArq = 0x00;
                    
                    escreverArq = 0x80 | (percorreArq>>1 & 0x3F);   //*
                    fwrite(&escreverArq, sizeof(char), 1, arquivo_saida);   
                    escreverArq = 0x00;
                }else{
                    printf("ERRO NO CHAR %X -- %c\n", percorreArq,percorreArq);
                }
            escreverArq = 0x00;
            }while(fread(&percorreArq, sizeof(int), 1, arquivo_entrada));

        }else{
            printf("BOM Invalido");
            return 1;
        }

}
