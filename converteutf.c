/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena - 2010796 - 3WB */

#include "converteutf.h"

char confere_endian(unsigned int bom){
    if(bom == 0x0000FEFF){   //little endian
        return 'b';}
    else if(bom == 0xFFFE0000){  //big endian
        return 'l';}
    else{
        return 'e';
    }
}

unsigned int inverteEndian(unsigned int entrada){
    unsigned int resultado = 0x00;

    resultado |= ((0xff & entrada) << 24);
    resultado |= (((0xff << 8) & entrada) <<8);
    resultado |= (((0xff << 16) & entrada) >> 8);
    resultado |= (((0xff << 24) & entrada) >> 24);

    return resultado;
}


/*              CHECKPOINT              */

//numero de caracteres no arquivo( == numero de bytes)
int numChar(FILE *arquivo_entrada){
    char c;
    int count = -1;
    for(c = fgetc(arquivo_entrada); c!=EOF; c = fgetc(arquivo_entrada)){
        count+=1;
    }
    return count;
}

int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida){
    char bom;
    unsigned int leitura;
       
    
}
int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){
    unsigned char resultado[4] = {0x00, 0x80, 0x80, 0x80}; //independente do tamanho, do segundo byte em diante ele vai comecar com 0x80 
    unsigned char bom;
    unsigned int leitura;

    fread(&leitura, sizeof(int), 1, arquivo_entrada);

    //utilizei a funcao que confere a endian do arquivo atraves do bom para verificar se o bom esta presente
    if(confere_endian(leitura) == 'e'){
        printf("BOM invalido\n");
        return -1;
    }else if(confere_endian(leitura) == 'b'){
        inverteEndian(leitura);
    }else if(confere_endian(leitura) == 'l'){
        while(fread(&leitura, sizeof(int), 1, arquivo_entrada) != 0){
            if(leitura > 0x10FFFF){
                printf("Ultrapassou valor maximo (U+10FFFF)\n"); //no enunciado fala que nao precisa, mantive apenas para organizar pensamento
                return -1;
            }
            

        }
    }

}