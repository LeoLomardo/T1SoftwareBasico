# Trabalho 1-SoftwareBasico(INF1018) 2022.2
Trabalho 1 de software básico PUC-Rio

Nota final: 7,5/10

Autores: Leo Lomardo

## Conversão entre codificações UNICODE

O objetivo deste trabalho é implementar, na linguagem C, duas funções ( **`converteUtf8Para32`** e **`converteUtf32Para8`** ), que recebem como entrada um arquivo contendo um texto codificado em um formato UNICODE (UTF-8 ou UTF-32) e geram como saida um arquivo contendo o mesmo texto, codificado no outro formato.

## Função `converteUtf8Para32`

```c
int converteUtf8Para32(FILE *arquivo_entrada, FILE *arquivo_saida)
```
As principais variáveis da função **`converteUtf8Para32`** são:
- `percorreArq`: para realizar a leitura do arquivo. Por ser um arquivo no formato **UTF-8**, utilizei ela do tipo ``` unsigned char```, para realizar a leitura byte por byte
- `escreveByte`: armazena o valor convertido para **UTF-32** e é escrita no  `arquivo_saida`. Como cada caracter após a conversão ocupa 32 bits, utilizei ela do tipo ```unsigned int```
- `numBytes`: valor retornado da função `int utf8_size(unsigned char aux)`, informando quantos bytes formam o caracter lido.
- `bom`: variável que armazena o valor do BOM, para ser escrito no começo do `arquivo_saida`, informando a ordem de armazenamento do arquivo( *big-endian* ou *little-endian*)

nao terminei Readme ainda;....
