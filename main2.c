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
int main(void){    
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    arquivo_entrada = fopen("utf32.bin", "rb");
    arquivo_saida = fopen("utf8Saida.bin", "wb");
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

        }else if(endian == 2){


        }else{
            printf("BOM Invalido");
            return 1;
        }


    system("hexdump -C utf8Saida.bin");



    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    return 0;
}



/*
 unsigned char aux[4] = {0x00, 0x00, 0x00, 0x00};
                    //tranformar em um vetor aux, e mexer em cada posicao
                    aux[0] = percorreArq ;
                    
                    aux[1] = percorreArq >> 8;
                    
                    aux[2] = percorreArq >> 16;
                    
                    printf("%X -- %X -- %X -- %X\n", aux[0] , aux[1], aux[2], aux[3]);
                    if(aux[3] != 0x00){
                        aux[0] |= 0xF0;
                        aux[1] |= 0x80;
                        aux[2] |= 0x80;
                        aux[3] =percorreArq >>  0x80;
                        // fwrite();                    
                        for(i = 0; i<4; i++){
                            fwrite(&aux[3-i],sizeof(aux[i]),1,arquivo_saida);
                        }
                    }else if((aux[2] != 0x00) && (aux[3] == 0x00)){
                        // aux[0] |= 0xE0;
                        // aux[1] |= 0x80;
                        // aux[2] |= 0x80;
                        for(i = 0; i<3; i++){
                            fwrite(&aux[2-i],sizeof(aux[i]),1,arquivo_saida);
                        }
                    }else if((aux[3] == 0x00) && (aux[2] == 0x00) && (aux[1] > 0x00)){
                        aux[0] |= 0xC0;
                        aux[1] = 0x80;
                        for(i = 0; i<2; i++){
                            fwrite(&aux[1-i],sizeof(aux[i]),1,arquivo_saida);
                        }
                    }else if((aux[3] == 0x00) && (aux[2] == 0x00) && (aux[1] == 0x00) && (aux[0] > 0x00)){
                                
                        fwrite(&aux[0],sizeof(aux[i]),1,arquivo_saida);
                    }

*/