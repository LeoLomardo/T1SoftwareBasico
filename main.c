#include <stdio.h>
#include <stdlib.h>


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

int main(void){    
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_saida = fopen("utf32Saida.bin", "wb");
    arquivo_entrada = fopen("utf8_peq.bin", "rb");

    
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
    


    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}

