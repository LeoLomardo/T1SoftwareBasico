#include <stdio.h>
#include <stdlib.h>


// int utf8_size(unsigned char aux){
//     if(((aux>>5) == 0x6)){
//         printf("Char de 2 byte");
//         return 2;
//     }else if((aux >> 4) == 0xE){
//         printf("Char de 3 byte");
//         return 3;
//     }else if((aux>>3) == 0x1E){
//         printf("Char de 4 byte");
//         return 4;
//     }
//     printf("Char de 1 byte");
//     return 1;
// }
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

int main(void){    
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_saida = fopen("utf32Saida.bin", "wb");
    arquivo_entrada = fopen("utf8_peq.bin", "rb");

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


    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}

