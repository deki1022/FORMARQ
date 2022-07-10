#include <stdio.h>
#include <locale.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void divider();
void verificaAloc(char *arq);
int tiraExtensao(char *s);
void invert(char *nomearq, FILE *arq, char *ext);
void cript(char *nomearq, FILE *arq, char *ext, char *chave);
void UnixToDos(char *nomearq, FILE *arq);
void DosToUnix(char *nomearq, FILE *arq);


int main(int argc, char *argv[]){
    setlocale(LC_ALL, "portuguese");
    FILE *entrada;
    char *narquivo, tipo, *chave, *extensao;;
	int f = 0;
	
	// Aloca mem�ria para o nome do arquivo
    narquivo = (char*) malloc(20);
    verificaAloc(narquivo);
    
    // Aloca mem�ria para a extens�o
    extensao = (char*) malloc(4);
    verificaAloc(extensao);
    
    // Recebe o nome do arquivo digitado na linha de comando ou pede o nome do arquivo
    do{
	    if(argc >= 2 && f == 0){
	        strcpy(narquivo, argv[1]);
	        f = 1;
		}
	    else {
	    	do{
	        	divider();
	        	printf("|  Forne�a o nome do arquivo: ");
	        	gets(narquivo);
			}while(strchr(narquivo,'.') == NULL);
	    }
	}while(strlen(strchr(narquivo, '.')) > 4);
	f = 0;
	
    // Copia a extensao do nome do arquivo na vari�vel extens�o
    strcpy(extensao, strchr(narquivo,'.'));
	
	// Abre o arquivo
    entrada = fopen(narquivo, "r");
    if(entrada == NULL){
        divider();
        printf("|  N�o foi poss�vel localizar o arquivo <%s> \n", narquivo);
        divider();
        exit(1);
    }
    
    // Tira a extens�o do nome do arquivo
	tiraExtensao(narquivo);
	
	// Recebe o tipo de convers�o digitado na linha de comando ou pede o tipo de convers�o
    if(argc >= 3 && f == 0){
            tipo = argv[2][0];
            f = 1;
    }
    else{
        divider();
        printf("|                                                      |\n");
        printf("|   I -> O tipo de convers�o deve ser o Invert.        |\n");
		printf("|   C -> O tipo de convers�o deve ser o Cript.         |\n");
		printf("|   D -> Arquivo texto UNIX, gerar arquivo texto DOS.  |\n");
		printf("|   U -> Arquivo texto DOS, gerar arquivo texto UNIX.  |\n");
        printf("|                                                      |\n");
		divider();
		printf("|     Escolha um tipo de convers�o de arquivo          |\n");
        printf("|              (LETRA MAIUSCULA): ");
		tipo = getche();
		printf("                    |\n");
		divider();
    }
	
    loop:
    
    // Faz a rotina de acordo com a escolha do tipo de convers�o
    switch(tipo){
    	// Cria um arquivo complemento de 1 do arquivo de entrada
        case 'I':
            invert(narquivo, entrada, extensao);
            break;
		
		// Cria um arquivo criptografado do arquivo de entrada
        case 'C':
            chave = (char*) malloc(20);
            verificaAloc(chave);
            if(argc >= 4)
                strcpy(chave, argv[3]);
            else{
                printf("|     Digite a chave: ");
                scanf("%s", chave);
                divider();
            }
            cript(narquivo, entrada, extensao, chave);
            free(chave);
            break;
            
		// Converte um arquivo Unix para um arquivo Dos
        case 'D':
        	UnixToDos(narquivo, entrada);
            break;

		// Converte um arquivo Dos para um arquivo Unix
        case 'U':
        	DosToUnix(narquivo, entrada);
            break;

		// Se o usu�rio escolher um tipo inv�lido, pede para digitar at� ser um tipo v�lido
        default:
            while(tipo!='I' && tipo!='C' && tipo!='D' && tipo!='U'){
                printf("|  Tipo de convers�o inv�lida! Digite novamente: ");
                tipo = getche();
                printf("     |\n");
            }
            divider();
            goto loop;
            break;
    }
	
	// Libera espa�o alocado
	free(narquivo);
	free(extensao);
	
	// Fechar o arquivo de entrada
    fclose(entrada);
    return 0;
}

void divider(){
	printf("--------------------------------------------------------\n");
}

void verificaAloc(char *arq){
	if(arq == NULL){
        printf("|  Mem�ria insuficiente       ");
        exit(1);
    }
}

int tiraExtensao(char *s){
    int i;
    for(i = 0; s[i] != '\0'; i++)
        if(s[i] == '.'){
            s[i] = '\0';
            return i;
        }
    return 0;
}

void invert(char *nomearq, FILE *arq, char *ext){
    char c;
    int invertido;
    FILE *saida;
    
    // Concatena o nome do arquivo com a extensao correta
    if(!strcmp(ext, ".INV") || !strcmp(ext,".inv")){
        strcat(nomearq, ".dnv");
    }else{
        strcat(nomearq, ".inv");
    }
    
    // Abre o arquivo de sa�da
    saida = fopen(nomearq, "w");
    if(saida == NULL){
        printf("|              Falha ao criar o arquivo                |\n");
        divider();
        exit(1);
    }
    
    // L� o arquivo de entrada
    c = fgetc(arq);
    while(c != EOF){
    	// Inverte o caractere lido e escreve no arquivo de sa�da
        invertido = ~c;
        fputc(invertido, saida);
        c = fgetc(arq);
    }
    
    // Fecha o arquivo de sa�da
    fclose(saida);
    printf("|           O arquivo %s foi criado        \n", nomearq);
    divider();
}

void cript(char *nomearq, FILE *arq, char *ext, char *chave){
    char c;
    int i;
    FILE *saida;
    
    // Concatena o nome do arquivo com a extens�o correta
    if(!strcmp(ext, ".CRP") || !strcmp(ext,".crp")){
        strcat(nomearq, ".drp");
    }else{
        strcat(nomearq, ".crp");
    }
    
    // Abre o arquivo de sa�da
    saida = fopen(nomearq, "w");
    if(saida == NULL){
        printf("|              Falha ao criar o arquivo                |\n");
        divider();
        exit(1);
    }
    
    // L� o arquivo de entrada
    c = fgetc(arq);
    while(c != EOF){
    	// Faz a opera��o bit a bit XOR entre o caractere lido e cada caractere da chave
        for(i = 0; chave[i] != '\0' ; i++){
            c = c ^ chave[i];
            // Escreve no arquivo de sa�da
            fputc(c, saida);
            c = fgetc(arq);
            if(c == EOF){
                break;
            }
        }
    }
    
    // Fecha o arquivo de sa�da
    fclose(saida);
    printf("|           O arquivo %s foi criado        \n", nomearq);
    divider();
}

void UnixToDos(char *nomearq, FILE *arq){
	FILE *saida;
	char c;
	
	// Concatena o nome do arquivo com a extens�o
    strcat(nomearq, ".dos");
    // Abre o arquivo de sa�da
    saida = fopen(nomearq, "w");
    if(saida == NULL){
        printf("|              Falha ao criar o arquivo                |\n");
        divider();
        exit(1);
    }
    
    // L� o arquivo de entrada escreve no arquivo de sa�da com o \n formatado em arquivo DOS
    c = fgetc(arq);
    while(c != EOF){
        if (c == 0x0A){
            fputc(0x0D, saida);
    	}
    	fputc(c, saida);
        c = fgetc(arq);
    }
    
    // Fechar o arquivo de sa�da
    fclose(saida);
    printf("|           O arquivo %s foi criado        \n", nomearq);
    divider();
}

void DosToUnix(char *nomearq, FILE *arq){
	FILE *saida;
	char c;
	
	// Concatena o nome do arquivo com a extens�o
    strcat(nomearq, ".unx");
    
    // Abre o arquivo
    saida = fopen(nomearq, "w");
    if(saida == NULL){
        printf("|              Falha ao criar o arquivo                |\n");
        divider();
        exit(1);
    }
    
    // L� o arquivo de entrada e escreve no arquivo de sa�da com o \n formatado em arquivo Unix
    c = fgetc(arq);
    while(c != EOF){
        if (c != 0x0D){
            fputc(c, saida);
    	}
        c = fgetc(arq);
    }
    
    // Fecha o arquivo de sa�da
    fclose(saida);
    printf("|           O arquivo %s foi criado        \n", nomearq);
    divider();
}



