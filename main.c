#include <stdio.h>
#include <stdlib.h>

#define TUDO_ZERO 0x00000000

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
    }
    printf("Char de 4 byte");
    return 4;
}
void dump (void *p, int n, FILE *arquivo_saida){
  unsigned char *p1 = p;
  while (n--){
    fwrite(p1, 1, 1, arquivo_saida);
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}
unsigned int inverteEndian(unsigned int entrada){
    unsigned int resultado = 0x00;

    resultado |= ((0xFF & entrada) << 24);
    resultado |= (((0xFF << 8) & entrada) <<8);
    resultado |= (((0xFF << 16) & entrada) >> 8);
    resultado |= (((0xFF << 24) & entrada) >> 24);

    return resultado;
}
//fgetc sempre le char por char, independente do tamanho da variavel q ta armazenando
//numero de caracteres no arquivo( == numero de bytes)
// int sizeChar(unsigned char c){
//     int size = 1;
//     if((c & 0x80) == 1){
//         size+=1;
//     }else if((c & 0xC0) == 1){
//         size+=2;
//     }else if((c & 0xE0) == 1){
//         size+=3;
//     }
//     return size;
// }
int main(void){

    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_saida = fopen("utf32Saida.bin", "wb");
    arquivo_entrada = fopen("utf8.bin", "rb");

    
    unsigned char percorreArq;
    unsigned int escreveByte;
    int numBytes = 1;
    unsigned char aux1 = 0;
    unsigned char aux2 = 0;
    unsigned char aux3 = 0;

    //unsigned int bom = 0xFFFE0000;
    

    // unsigned char bom = 0x0000EFFF;
    // unsigned int bom = 0xfffe0000;
    // unsigned char bom[4] = {0xFF, 0xFE, 0x00,0x00 };
    unsigned int bom = 0x0000feff;
    fwrite(&bom, 4,1,arquivo_saida);
    // dump(&bom, sizeof(bom), arquivo_saida);

    //ESSA DESGRACA DE 'BOM' TA FODA DE IMPLEMENTAR, CHEGUEI ATE 70% E PRECISO JANTAR, 23 E NN COMI NADA TENTANDO FAZER ISSO, 
    //ODEIO BITS, TO DOIDO PRA CRIAREM O COMPUTADOR QUANTICO PRA ELES DEIXAREM DE EXISTIR QBITS>>bits 
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

