#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lib.h"

void get_attr_HTTP_GET(FILE *dataset,FILE *csv){
	char user_agent[64],pragma[64],cache_control[64],accept_enconding[64],accept_language[64],host[64],connection[64];

	fprintf(csv,"%d,",0);                   // contentlength
	fprintf(csv,"%s,","none");		// content_type
	fprintf(csv,"%s,","norm");		// label

	char *ptr = get_attr(dataset,csv);   	// user-agent
	copy_content(user_agent,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv); 		// pragma
	copy_content(pragma,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv); 		// cache-control
	copy_content(cache_control,ptr,strlen(ptr));
	readline(dataset);			// accept
	ptr = get_attr_enconding(dataset,csv);  // accept-enconding
	copy_content(accept_enconding,ptr,strlen(ptr) - 1);
	readline(dataset);			// accept-charset
	ptr  = get_attr(dataset,csv); 		// accept-language
	copy_content(accept_language,ptr,strlen(ptr));
	ptr  = get_attr(dataset,csv);	    	// host
	copy_content(host,ptr,strlen(ptr));

	readline(dataset);			// cookie
	ptr  = get_attr(dataset,csv);		// connection
	copy_content(connection,ptr,strlen(ptr));

	readline(dataset);
	readline(dataset);

	// Escreve no arquivo os atributos recuperados
	fprintf(csv,"%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%s,%d,%d,%d,%s,%d\n",keywords_count,length_of_GET,
			pragma,host,connection,cache_control,accept_language,accept_enconding,email,user_agent,
			name,password_count,login,special_chars,length_of_URL,number_attr_URL,extension,length_attr_URL);

}

// Função que copia o conteúdo de um ponteiro para um vetor
// É útil já que o valor do ponteiro muda constantemente a cada execução da função get_attr
void copy_content(char dest[],char *src,int len){

	memcpy(dest,src,len);
	dest[len] = '\0';

}

void get_attr_HTTP_POST(FILE *dataset,FILE *csv){

	char user_agent[64],pragma[64],cache_control[64],accept_enconding[64],
	accept_language[64],host[32],content_type[64],connection[64],content_length[64];

	char *ptr = get_attr(dataset,csv);   	// user-agent
	copy_content(user_agent,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv); 		// pragma
	copy_content(pragma,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv); 		// cache-control
	copy_content(cache_control,ptr,strlen(ptr));
	readline(dataset);			// accept
	ptr = get_attr_enconding(dataset,csv);  // accept-enconding
	copy_content(accept_enconding,ptr,strlen(ptr) - 1);
	readline(dataset);			// accept-charset
	ptr  = get_attr(dataset,csv); 		// accept-language
	copy_content(accept_language,ptr,strlen(ptr));
	ptr  = get_attr(dataset,csv);	    	// host
	copy_content(host,ptr,strlen(ptr));

	readline(dataset);			// cookie
	ptr  = get_attr(dataset,csv);		// content-type
	copy_content(content_type,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv); 		// connection
	copy_content(connection,ptr,strlen(ptr));
	ptr = get_attr(dataset,csv);		// content-length
	copy_content(content_length,ptr,strlen(ptr));



	readline(dataset);
	get_data_post(dataset,csv);
	readline(dataset);
	
	// Escreve no arquivo os atributos recuperados
	fprintf(csv,"%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%s,%d,%d,%d,%s,%d\n",content_length,content_type,"norm",keywords_count,length_of_GET,
			pragma,host,connection,cache_control,accept_language,accept_enconding,email,user_agent,
			name,password_count,login,special_chars,length_of_URL,number_attr_URL,extension,length_attr_URL);


}


// Função que consome a linha sem retornar nada
void readline(FILE *dataset){
	char buffer = fgetc(dataset);
	while(buffer != 10)
		buffer = fgetc(dataset);

}

void get_data_post(FILE *dataset,FILE *csv){

	// Resetando o valor da variável acumuladores
	password_count = 0;
	keywords_count = 0;

	char *http;
	char *number_attr_URL_ptr;
	char *url;
	char *name_ptr;
	char *password_ptr;
	char *login_ptr;
	char *email_ptr;
	char *keywords_ptr;
	char buffer[2048];
	int tam=0;

	// Consome o primeiro caractere
	buffer[tam] = fgetc(dataset);
	// Consome todos os caracteres presente na linha
	// ASCII 10 --> '\n' - newline
	while(buffer[tam] != 10){
		buffer[++tam] = fgetc(dataset);
	}
	// Em C, é necessário finalizar a string com o '\0' (ASCII Null Character)
	buffer[tam] = '\0';

	// Acumulador para o tamanho total da requisição HTTP
	length_of_GET +=strlen(buffer);
	url = buffer;
	number_attr_URL_ptr = buffer;

		// Se o conteúdo do POST não for vazio
		if(url != NULL){
			special_chars = return_special_chars(url);
			name_ptr = strstr(url,"nombre=");			// Retorna um ponteiro para ocorrência da string "nombre=" na string url
			password_ptr = strstr(url,"password=");
			login_ptr = strstr(url,"login=");
			email_ptr = strstr(url,"email=");
			if(name_ptr != NULL){
				// Copia o conteúdo para vetor de char name (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				name_ptr = replace_char(name_ptr, find, rep);
				copy_content(name,name_ptr+7,strlen(name_ptr)-strlen(strstr(name_ptr,"&"))-7);
				//fprintf(stderr,"%s\n",name_ptr);
			}
			else{
				// Se não possuir o atributo em questão, seu valor é null
				copy_content(name,"null",4);
			}
			if(password_ptr != NULL)
				password_count = strlen(password_ptr)-strlen(strstr(password_ptr,"&") - 9);
			else{
				password_ptr = strstr(url,"pwd=");
					if(password_ptr != NULL)
						password_count = strlen(password_ptr)-strlen(strstr(password_ptr,"&") - 4);
					else
						password_count = 0;

			}
			if(login_ptr != NULL){
				// Copia o conteúdo para vetor de char login (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				login_ptr = replace_char(login_ptr, find, rep);
				copy_content(login,login_ptr+6,strlen(login_ptr)-strlen(strstr(login_ptr,"&") - 6));
			}
			else
				copy_content(login,"null",4);
			if(email_ptr != NULL)
				// Copia o conteúdo para vetor de char email (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				copy_content(email,email_ptr+6,strlen(email_ptr)-strlen(strstr(email_ptr,"&") - 6));
			else
				copy_content(email,"null",4);


			// Procurando por keywords SQL
			if(strstr(url,"SELECT") != NULL)
				keywords_count++;

			if(strstr(url,"DROP") != NULL)
				keywords_count++;

			if(strstr(url,"UNION") != NULL)
				keywords_count++;

			if(strstr(url,"DELETE") != NULL)
				keywords_count++;

			if(strstr(url,"MODIFY") != NULL)
				keywords_count++;

			}


	get_number_attr_URL(number_attr_URL_ptr);

}


char *get_type_http(FILE *dataset,FILE *csv){

	// Resetando o valor das variáveis acumuladores
	password_count = 0;
	length_of_GET = 0;
	number_attr_URL = 0;
	length_attr_URL = 0;

	char *http;
	char *number_attr_URL_ptr;
	char *url;
	char *name_ptr;
	char *password_ptr;
	char *login_ptr;
	char *email_ptr;
	char buffer[2048];
	int tam=0;

	// Consome o primeiro caractere
	buffer[tam] = fgetc(dataset);
	// Consome todos os caracteres presente na linha
	// ASCII 10 --> '\n' - newline
	while(buffer[tam] != 10){
		buffer[++tam] = fgetc(dataset);
	}
	// Em C, é necessário finalizar a string com o '\0' (ASCII Null Character)
	buffer[tam] = '\0';

	// Acumulador para o tamanho total da requisição HTTP
	length_of_GET +=strlen(buffer);

	// Strtok separa a string por espaço
	// HTTP possui o valor GET ou POST
	http = strtok(buffer," ");
		
	//fprintf(csv,"%s,",http);
	//fprintf(csv,"%s,",strtok(NULL," "));
	//fprintf(csv,"%s,",strtok(NULL," "));

	// URL possui a string url do método HTTP
	url = strtok(NULL, " ");
	// Armazenando o tamanho de caracteres da URL
	length_of_URL = strlen(url);

	// Ponteiro auxiliar aponta para a URL
	number_attr_URL_ptr = url;

	// Função que armazena o valor da quantidade de atributos na requisição URL;
	get_number_attr_URL(number_attr_URL_ptr);
	// Função que recupera a extensão do arquivo da URL
	get_extension_URL(url);
	// Função que armazena o valor da quantidade de caracteres nos campos de atributos da URL
	get_length_attr_URL(url);

		// Se a requisição for do tipo GET e se existir o parâmetro url
		if(url != NULL && !strcmp(http,"GET")){
			name_ptr = strstr(url,"nombre=");			// Retorna um ponteiro para ocorrência da string "nombre=" na string url
			password_ptr = strstr(url,"password=");
			login_ptr = strstr(url,"login=");
			email_ptr = strstr(url,"email=");
			if(name_ptr != NULL){
				// Copia o conteúdo para vetor de char name (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				name_ptr = replace_char(name_ptr, find, rep);
				copy_content(name,name_ptr+7,strlen(name_ptr)-strlen(strstr(name_ptr,"&"))-7);
				//fprintf(stderr,"%s\n",name);
			}
			else{
				// Se não possuir o atributo em questão, seu valor é null
				copy_content(name,"null",4);
			}
			if(password_ptr != NULL)
				password_count = strlen(password_ptr)-strlen(strstr(password_ptr,"&") - 9);
			if(login_ptr != NULL){
				// Copia o conteúdo para vetor de char login (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				login_ptr = replace_char(login_ptr, find, rep);
				copy_content(login,login_ptr+6,strlen(login_ptr)-strlen(strstr(login_ptr,"&")) - 6);
}
			else
				copy_content(login,"null",4);
			if(email_ptr != NULL)
				// Copia o conteúdo para vetor de char email (A variável é global, portanto seu valor não é perdido quando a função é terminada)
				copy_content(email,email_ptr+6,strlen(email_ptr)-strlen(strstr(email_ptr,"&") - 6));
			else
				copy_content(email,"null",4);


			// Procurando por keywords SQL
			if(strstr(url,"SELECT") != NULL)
				keywords_count++;

			if(strstr(url,"DROP") != NULL)
				keywords_count++;

			if(strstr(url,"UNION") != NULL)
				keywords_count++;

			if(strstr(url,"DELETE") != NULL)
				keywords_count++;

			if(strstr(url,"MODIFY") != NULL)
				keywords_count++;

			special_chars = return_special_chars(url);


		}

	// Retorna string contendo o valor "GET","POST"
	return http;
}

void get_length_attr_URL(char *src){
	char *aux = src;
        aux = strchr(aux,'?');
	if(aux != NULL)
		length_attr_URL = strlen(aux) - 1; // --> -1 para retirar o caractere '?'


}

void get_extension_URL(char *url){
	char *extension_ptr = strchr(url,'.');
	// Copia o valor do ponteiro para um buffer estático
	// Buffer possui a extensão da URL
	if (extension_ptr != NULL)
		copy_content(extension,extension_ptr+1,3);
	else{
		extension_ptr="nul";
		copy_content(extension,extension_ptr,3);
	}
}

void get_number_attr_URL(char *src){
	src = strchr(src,'=');
	while(src){
		number_attr_URL++;
		src++;
		src = strchr(src,'=');
	}
}



// Função que retorna o número de caracteres especiais em uma string
// Teste separadamente essa função, ainda não testei integrando ao programa
// A lógica da função é testar a string 31 vezes (número de caracteres especiais)
// Em cada vez, é testado quantas vezes existe um certo caractere especial
int return_special_chars(char *src){
	// O ponteiro original aponta o início da palavra
	// Usaremos aux para armazenar essa valor, pois andaremos com a string através de src
	char *aux = src;

	// Uma 'listinha' de caracteres especiais
	char charSpecial[] = {'!','"','#','$','%','\'','(',')','*',
			     '+','-','.','/',':',';','<','=','>','?','@',
			     '[',']','^','_','`','{','|','}','~','\\'};

	int number_special= 30;
	int i=0;
	int number=0;
	while(i < number_special){					// Para testar cada caractere
		while(src != NULL){					// Sai do laço quando não existe o caractere --> src aponta para NULL
		 src = strchr(src,charSpecial[i]);	 		// strchr retorna um ponteiro para o caractere mais próximo correspondente
		  if(src != NULL){					// Se for NULL, o caractere não existe
		 	 //fprintf(stderr,"%c e %d\n",charSpecial[i],i);  // Para ficar mais visual a saída
			 src++;						// Avança o ponteiro --> Aponta para a próxima letra
			 number++;
		  }
		  else
			  src = NULL;					// src aponta para nada --> Significa que não existe o caractere, e portanto iremos testar se existe o próximo caractere

		}
		src = aux;						// src aponta para o início da palavra
		i++;							// Incrementa o contador que controla qual caractere será testado
	}

	return number;							// Retorna o valor de quantidade de caracteres especiais

}

// Função que recupera o valor do atributo Accept-Enconding
char *get_attr_enconding(FILE *dataset,FILE *csv){
	
	char *http;
	char buffer[500];
	int tam=0;

	/*Captura todos os caracteres da linha*/
        buffer[tam] = fgetc(dataset);
        while(buffer[tam] != 10){
                buffer[++tam] = fgetc(dataset);
        }
        // Termina a string
        buffer[tam] = '\0';
        // Soma na variável acumuladora
        length_of_GET += strlen(buffer);

        // HTTP aponta para o buffer
        http = buffer;

        // Substitui o caractere de vírgula do atributo por '|'
	replace_char(http,',','|');
        //Retorna o ponteiro da string -> HTTP possui o valor de Accept-Enconding
	return http + strlen("Accept-Encoding: ");

}


// Função responsável por capturar o valor dos atributos HTTP
// Basicamente pega o valor depois da ocorrência do caractere ':'
char *get_attr(FILE *dataset,FILE *csv){

	char *http;
        char buffer[500];
        int tam=0;

	/*Captura todos os caracteres da linha*/
        buffer[tam] = fgetc(dataset);
        while(buffer[tam] != 10){
                buffer[++tam] = fgetc(dataset);
        }
	// Termina a string
        buffer[tam] = '\0';
	// Soma na variável acumuladora
	length_of_GET += strlen(buffer);

	// HTTP aponta para o nome do atributo (Exemplo: 'Accept-Language:')
        http = strtok(buffer," ");
	// HTTP aponta para o valor do atributo (Exemplo: 'en')
	http = strtok(NULL," ");

	//Retorna o ponteiro da string
	return http;

}


char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){   
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

