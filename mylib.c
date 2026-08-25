#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mylib.h"

void add_paciente(VetPacientes *pacientes) {
	FILE *file;
	char nome[64];
	char contato[64];
	char choice;

	printf("Digite o nome do paciente: ");
	getchar();
	fgets(nome,64, stdin);
	nome[strcspn(nome, "\n")] = '\0';
	printf("Digite o e-mail do paciente: ");
	fgets(contato,64, stdin);
	contato[strcspn(contato, "\n")] = '\0';
	printf("Adicionar o paciente %s, contato: %s com o id: %d? (y/n)\n", nome, contato, pacientes->qtd);
	scanf(" %c", &choice);

	if(choice == 'n') {
		return;
	}
	printf("Adicionando...\n\n");


	file = fopen("pacientes.txt","a"); //a-append

	if(file == NULL) {
		printf("Erro ao abrir arquivo");
		return;
	}

	//fprintf é uma função para escrever no arquivo.
	fprintf(file, "%s %s %d\n", nome, contato, pacientes->qtd);

	fclose(file);

	strcpy(pacientes->itens[pacientes->qtd].nome, nome);
	strcpy(pacientes->itens[pacientes->qtd].contato, contato);
	pacientes->itens[pacientes->qtd].id = pacientes->qtd;

	pacientes->qtd++;

	printf("Paciente cadastrado.\n");


}
void search_paciente(VetPacientes *pacientes) {
	int id;
	if(pacientes->qtd == 0) {
		printf("Não existe pacientes cadastrados.\n");
		return;
	}
	printf("Digite o id do paciente: ");
	scanf("%d", &id);
	if(id >= pacientes->qtd || id<0) {
		printf("Paciente não encotrado.\n");    //gambiarra
		return;
	}
	printf("id: %d nome: %s contato: %s\n", pacientes->itens[id].id, pacientes->itens[id].nome, pacientes->itens[id].contato);
}

void read_pacientes(VetPacientes *pacientes) {
	FILE *file;

	int i = 0;
	file = fopen("pacientes.txt", "r");

	if(file == NULL) {
		printf("erro");
	}
	while(fscanf(file, "%s %s %d\n", &pacientes->itens[i].nome, &pacientes->itens[i].contato, &pacientes->itens[i].id) != EOF) {
		i++;
	}
	pacientes->qtd = i;

	fclose(file);
}
//Para a todo momento verificar o tamanho do vetor, caso precise aumentar. Adiciona mais 10

int realocar_medicos(VetMedicos *medicos) {
	if(medicos->qtd == medicos->cap) {
		Medico *medicos1;
		medicos1 = (Medico*) realloc(medicos->itens, (medicos->cap+10) * sizeof(Medico));
		if(medicos1 != NULL) {
			medicos->itens = medicos1;
			medicos->cap += 10;
			return 1;
		} else {
			return 0;
		}
	}
	return 2;
}

//Tratando os dados para alocar mais facil as horas
//Ainda em desenvolvimento

int read_horas(Horario horario) {

	return (horario.horas * 100) + horario.minutos;
}

void desconverter_horas(int n, Horario *horario) {
	horario->horas = n/100;
	horario->minutos = n%100;
}

//Conta quantas linhas tem no arquivo, serve para quaisquer tipo de arquivo

int contar_linhas(FILE *arquivo) {
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
	rewind(arquivo);
	return linhas;
}

//Ler a quantidade de medicos no inicio da aplicação, para conseguir transcrever todos em um vetor
//Ainda em desenvolvimento

void read_medicos(VetMedicos *medicos) {
	FILE *file;

	int i=0;
	file = fopen("medicos.txt","r");

	if(file == NULL) {
		//Caso não exista arquivo nenhum, cria um vetor dinamico de tamanho 10
		//printf("Não foi possivel ler o arquivo, criando um novo em branco\n");

		medicos->qtd = 0, medicos->cap = 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);
		if(medicos->itens == NULL) {
			printf("Erro de memoria\n");
			return;
		}

		file = fopen("medicos.txt","w");
		if(file == NULL) {
			printf("Erro ao abrir o arquivo\n");
		}
		fclose(file);
		return;
	} else {

		//Chama uma função geral que serve para contar quantas linhas tem no arquivo, cria um vetor com a quantidade de linhas + 10.
		//Considere cada linha um objeto

		int n1, n2, n3, n4, i = 0;
		
		medicos->qtd = contar_linhas(file);
		medicos->cap = medicos->qtd + 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);
		if(medicos->itens == NULL){
		    printf("Erro ao realocar o vetor\n");
		    fclose(file);
		    return;
		}

		while(fscanf(file,"%d %s %d %d %d %d %d\n",&medicos->itens[i].id, medicos->itens[i].nome, &medicos->itens[i].especialidade, &n1,&n2,&n3,&n4) != EOF) {
			desconverter_horas(n1,&medicos->itens[i].inicioManha);
			desconverter_horas(n2,&medicos->itens[i].fimManha);
			desconverter_horas(n3,&medicos->itens[i].inicioTarde);
			desconverter_horas(n4,&medicos->itens[i].fimTarde);
			i++;
		}
		medicos->qtd = i;
		fclose(file);
		return;
	}
}