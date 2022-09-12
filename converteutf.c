/* Leo Lomardo - 2020201 - 3WA */
/* Lucas Lucena -  - 3WA  */

#include "converteutf.h"


int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida){

}

int converteUtf32Para8(FILE *arquivo_entrada, FILE *arquivo_saida){
    //ler primeiros 4 bytes =  ler 4 char 
    unsigned int bom  = 0xFFFE0000;
    
    unsigned int check;

    fread(&check, sizeof(int),1, arquivo_entrada);
    printf("%d", check);
    
    
}