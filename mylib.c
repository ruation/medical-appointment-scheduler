#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"


int verify_name(const char *str){
    if (*str == '\0') return 0; // return 0 if the string is empty
    for(int i = 0; str[i] != '\0'; i++){
        if(isdigit(str[i])){printf("O nome não pode conter números.\n"); return 0;}
    }
    return 1; //return 1 if the string is ok.
}

//Função para preencher horario mais facilmente
void receber_hora(Horario *horario) {
	int hora, minutos;
	//Loop para garantir que o horario digitado é válido
	while(1) {
		printf("Digite a hora\n");
		scanf("%d",&hora);
		if(24>hora && hora>=0) {
			break;
		} else {
			printf("Horario invalido, digite uma hora existente\n");
		}
	}
	while(1) {
		printf("Digite os minutos\n");
		scanf("%d",&minutos);
		if(minutos>=0 && minutos < 60) {
			break;
		} else {
			printf("Horario invalido, digite uma minutagem valida\n");
		}
	}
	horario->minutos = minutos;
	horario->horas = hora;
	printf("Horario escolhido: %d:%d",horario->horas,horario->minutos);
}

//Trata ainda mais os dados de inicio e fim de uma consulta ou turno
//Para assim conseguir padronizar um horario correto
void veri_horario(Horario *inicio, Horario *fim, int n, int turno) {
	//n é a variavel que é inserido direto no código, caso 1 trata para medicos e caso 2 para consultas. Default é por segurança que o código não quebre
	while(1) {
			switch(n) {
			case 1:
			    switch(turno){
			        case 1:
			            while(inicio->horas > 9 || (inicio->horas == 9 && inicio->minutos > 0) || inicio->horas < 7){
			                printf(" fim: %d:%d\n", inicio->horas+3, inicio->minutos);
			                printf("O expediente da manhã precisa ser entre 7h e 12h. ");
			                receber_hora(inicio);
			            }
        		       
        		       	fim->horas = inicio->horas + 3; //ajusta o fim do expediente somando 3 horas do inicio.
        			    fim->minutos = inicio->minutos;
        			    
        			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
        			    return;
		            case 2:
		                while(inicio->horas <13 || inicio->horas > 17 || (inicio->horas == 17 && inicio->minutos > 0)){
		                    printf(" fim: %d:%d\n", inicio->horas+3, inicio->minutos);
		                    printf("O expediente da tarde precisa ser entre 13h e 20h. ");
		                    receber_hora(inicio);
		                }
        		       	fim->horas = inicio->horas + 3; //ajusta o fim do expediente somando 3 horas do inicio.
        			    fim->minutos = inicio->minutos;
        			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
		                return;
			    }

			case 2:
			    while((inicio->horas == 19 && inicio->minutos > 0) || inicio->horas > 19 || inicio->horas<7 || (inicio->horas == 11 && inicio->minutos > 0) || inicio->horas == 12){
			         printf(" fim: %d:%d\n", inicio->horas+1, inicio->minutos);
			         printf("A consulta precisa ser entre 7h e 12h ou 13h e 20h ");
			         receber_hora(inicio);
			    }
			    fim->horas = inicio->horas + 1; //ajusta o fim da consulta adicionando 1 hora do inicio
			    fim->minutos = inicio->minutos;
			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
                return;
			default:
				printf("Erro\n");
				return;
			}
		}
}
//Tratando os dados para alocar mais facil as horas
//Para otimizar o espaço, isso serve para converter horario que seria duas variaveis em uma só
//Assim dá para guardar mais facilmente no arquivo

int con_horas(Horario horario) {

	return (horario.horas * 100) + horario.minutos;
}

//Pega váriavel compacta em uma só, e já devolve descompactada para o struct Horario

void desconverter_horas(int n, Horario *horario) {
	horario->horas = n/100;
	horario->minutos = n%100;
}

//Conta quantas linhas tem no arquivo, serve para quaisquer tipo de arquivo

int contar_linhas(FILE *arquivo) {
	//testa se consegue ler o arquivo ou não
	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo!\n");
		return -1;
	}

	int linhas = 0;
	char buffer[1024]; // Tamanho arbitrário para cada linha

	// Lê linha por linha
	while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
		linhas++;
	}
	//quando termina de ler o arquivo, o ponteiro de arquivo fica no EOF(final dele)
	//Assim tem que usar o rewind para apontar o inicio do arquivo
	rewind(arquivo);
	return linhas;
}