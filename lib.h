#ifndef LIB_H_          /* Include guard*/
#define LIB_H_

extern int length_of_GET;
extern int length_of_URL;
extern int number_attr_URL;
extern int length_attr_URL;
extern char name[128];
extern char login[128];
extern char email[128];
extern char extension[4];
// Variáveis acumuladores;
extern int password_count;
extern int special_chars;
extern int keywords_count;

// Caracteres de substituição	
extern char rep;
extern char find;

void readfile (FILE *dataset);
char *get_type_http(FILE *dataset,FILE *csv);
char *get_attr(FILE *dataset,FILE *csv);
void readline(FILE *dataset);
int return_special_chars(char *src);
void get_attr_HTTP_GET(FILE *dataset,FILE *csv);
void get_attr_HTTP_POST(FILE *dataset,FILE *csv);
void copy_content(char dest[],char *src,int len);
void get_data_post(FILE *dataset,FILE *csv);
char* replace_char(char* str, char find, char replace);
void get_number_attr_URL(char *src);
char *get_attr_enconding(FILE *dataset,FILE *csv);
void get_extension_URL(char *src);
void get_length_attr_URL(char *src);

#endif // LIB_H

