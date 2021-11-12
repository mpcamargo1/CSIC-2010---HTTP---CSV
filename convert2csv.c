#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lib.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------
// Obs: se testar diretamente no dataset original, irá recuperar um caractere inválido. Isso ocorre pois o dataset foi feito no Windows, e o padrão
// para nova linha é '\r\n', enquanto que no Linux é somente '\n'. Aproveitei e já coloquei na pasta o dataset modificado (apenas selecionei todas
// as linhas do dataset e salvei novamente e assim salvou somente com '\n')
// Para compilar no Windows, é necessário retirar o include da biblioteca 'unistd.h'. Utilizei essa ferramenta para dar uma breve pausa na execução
// Compilar no Linux - gcc convert2csv.c lib.c -o programa ---> Executar: ./programa --> O programa não emite nenhuma informação na tela --> Visualizar arquivo de saída out.csv
// Em lib.c está a implementação das funções
// Para melhor visualização dos comentários, configurar na IDE o valor TAB_SIZE = 8
// ---------------------------------------------------------------------------------------------------------------------------------------------------

/* Extracted Feature*/
// Inicialmente, optei por deixar essa variáveis globais
// Depois, eu organizo uma estrutura melhor para elas
int length_of_GET=0;
int length_of_URL=0;
int length_attr_URL=0;
char name[128];
char login[128];
char email[128];
char extension[4];
// Variáveis acumuladores;
int password_count=0;
int special_chars=0;
int keywords_count=0;
int number_attr_URL=0;

// Caracteres de substituição	
char rep = '@';
char find = '%';


// Protótipos das funções
// Necessário para o compilador
FILE *loadfile();

int main(void){
	FILE *dataset = loadfile();
	clock_t begin = clock();
	readfile(dataset);
	fprintf(stderr,"Tempo total de execução %lf segundos\n",(double) (clock() - begin) / CLOCKS_PER_SEC);
	return 1;

}


// Lógica principal do programa
void readfile(FILE *dataset){
	// Inicia um ponteiro para o arquivo (Em outras palavras, abro um arquivo para gravação)
	FILE *csv = fopen("out.csv","w");
	char *http;
	int parseCompleted = 0;
	// Laço principal
	// Responsável por realizar o parser de 25000 requisições anômalas
	// Deixei assim para facilitar o debug do programa
	while(parseCompleted != 25000){	
		http = get_type_http(dataset,csv);		// Recupera valor da requisição HTTP("GET","POST" ou "PUT")
		if(strcmp("GET",http) == 0)			// Compara a string com "GET" --> Se for 0, então são iguais
			get_attr_HTTP_GET(dataset,csv);
		else if( (strcmp("POST",http) == 0) || (strcmp("PUT",http) == 0 )) // Compara a string com "POST" e "PUT"
			get_attr_HTTP_POST(dataset,csv);
		parseCompleted++;				// Incrementa o contador
	}
	fclose(csv);						// Fecha o arquivo

}


// Inicia um ponteiro para o arquivo
// 'r' indica arquivo somente para leitura
FILE *loadfile(){
	FILE *fp = fopen("anomalousTrafficTest.txt","r");
	return fp;

}
