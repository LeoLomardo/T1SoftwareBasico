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
    }else
        printf("Char de 4 byte");
        return 4;
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

    unsigned int bom = 0xFFFE0000;
    unsigned int percorreArq;
    unsigned int escreveByte;
    int numBytes = 1;
	
    unsigned char aux1 = 0;
    unsigned char aux2 = 0;
    unsigned char aux3 = 0;
    
	while(percorreArq =fgetc(arquivo_entrada)){
		if((percorreArq > 0x00) & (percorreArq < 0x7F)){

		}else if((percorreArq > 0x80) & (percorreArq < 0x7FF)){

		}else if((percorreArq > 0x800) & (percorreArq < 0xFFFF)){

		}else if((percorreArq > 0x10000) & (percorreArq < 0x10FFFF)){

		}else{
			prinf("Char fora do alcance;\n");
			return -1;
		}
	}
    


    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}

