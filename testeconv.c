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
    }else if((aux>>5) == 0xF0){
        printf("Char de 4 byte");
        return 4;
    }else{
        printf("Fora do alcance");
        return -1;
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
int sizeChar(unsigned char c){
    int size = 1;
    if((c & 0x80) == 1){
        size+=1;
    }else if((c & 0xC0) == 1){
        size+=2;
    }else if((c & 0xE0) == 1){
        size+=3;
    }
    return size;
}
int main(void){

    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_saida = fopen("utf32Saida.bin", "wb");
    arquivo_entrada = fopen("utf8.bin", "rb");

    unsigned int bom = 0x0000FE;
    unsigned char percorreArq;
    unsigned int escreveByte;
    unsigned int aux = 0;
    int numBytes = 1;
    

    while(fread(&percorreArq,sizeof(int),1,arquivo_entrada)==1){

        // while((percorreArq & 0x80)){ //0x8000 = 10000000 00000000 mostrando q o byte seguinte e continuacao do mesmo char
        //     ++numBytes; //incrementa para verificar se estrapola o limite de 4 bytes por char do utf8
        //     percorreArq  = (percorreArq <<1) | 0x00; //verificar sintaxe na hora de rodar
        // }
        numBytes = sizeChar(percorreArq);

        printf("%d\n", numBytes);
            if(numBytes > 4){
                printf("Estrapolou limite de 4 bytes por char.");
                return -1;

            }
            if(numBytes == 1){ //basta imprimir o byte com 25 bits a esquerda zerados
                escreveByte  = 0x0 | percorreArq;

            }else{
                aux = percorreArq;
                aux >>= 1+(4*(numBytes-2));
                escreveByte  = (0x0 | aux);

                for(int i = 1; i<numBytes; i++){

                    fread(&percorreArq,sizeof(char),1,arquivo_entrada);  // le proximo byte

                    aux=percorreArq;

                    aux= (aux & 0x7FFF);  // zera primeiro byte


                    escreveByte=(escreveByte>>8*(numBytes-i)| aux);
                }

            }
                fwrite(&escreveByte,sizeof(int),1,arquivo_saida); //imprime

        }
    


    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}



/*
  //ler primeiros 4 bytes =  ler 4 char 

  //essa funcao ta +- certa, ela ta lendo a cada 4 bytes, precisamos arrumar ela pra ela ler byte por byte e ver se é char 1, 2,3 ou 4 bytes, dai traduzir 
    unsigned int  percorreArqLeitura = 0;   //caracter que le byte por byte 
    char bom [4] = {0xFF, 0xFE, 0x00, 0x00};
    fwrite(bom, sizeof(bom),1, arquivo_saida);
    
    for(percorreArqLeitura = fgetc(arquivo_entrada); percorreArqLeitura !=EOF; percorreArqLeitura = fgetc(arquivo_entrada)){
            char byteSaida[4] = {0x00, 0x00, 0x00, 0x00};

        if(percorreArqLeitura <= 0x7F){
            byteSaida[0] = percorreArqLeitura;
            fwrite(byteSaida,sizeof(char),1,arquivo_saida);
            //16 bits char
        }else if(percorreArqLeitura >= 0x80 && percorreArqLeitura <= 0x7FF){
            byteSaida[0] = 0xC0 | (percorreArqLeitura >> 6); //110xxxxx || percorreArqLeitura 6 bits pra direita para armazenar os dados do 1 byte
            byteSaida[1] = 0x80 | (percorreArqLeitura & 0x3F); //10xxxxxx || percorreArqLeitura & 0x3F            
            //24 bits char
            fwrite(byteSaida,sizeof(char),1,arquivo_saida);

        }else if(percorreArqLeitura >=0x800 && percorreArqLeitura <= 0xFF){
            byteSaida[0] = 0xE0 | (percorreArqLeitura >> 12); //1110xxxx || 1 byte percorreArqLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((percorreArqLeitura >> 6) & 0x3F); //10xxxxxx || percorreArqLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | (percorreArqLeitura & 0X3F); //10xxxxxx || percorreArqLeitura 2 bits pra direita
                        fwrite(byteSaida,sizeof(char),1,arquivo_saida);

            //32 bits
        }else if(percorreArqLeitura >=0x10000 && percorreArqLeitura <= 0x10FF){
            byteSaida[0] = 0xF0 | (percorreArqLeitura >> 18); //11110xxx ||  1 byte percorreArqLeitura 3 bits pra direita
            byteSaida[1] = 0x80 | ((percorreArqLeitura >> 12) & 0X3F); //10xxxxxx || percorreArqLeitura 2 bits para direita 
            byteSaida[2] = 0x80 | ((percorreArqLeitura >> 6) & 0X3F); //10xxxxxx || percorreArqLeitura 2 bits pra direita
            byteSaida[3] = 0x80 | (percorreArqLeitura & 0x3F); //10xxxxxx || percorreArqLeitura 2 bits pra direita
            fwrite(byteSaida, sizeof(byteSaida), 1, arquivo_saida);
        }else{
            printf("Caracter invalido");
        }

*/