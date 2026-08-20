#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mylib.h"

void add_paciente(VetPacientes *pacientes){
    FILE *file;
    char nome[64];char contato[64]; char choice;
    
    printf("Digite o nome do paciente: ");
    getchar();
    fgets(nome,64, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    printf("Digite o e-mail do paciente: ");
    fgets(contato,64, stdin);
    contato[strcspn(contato, "\n")] = '\0';
    printf("Adicionar o paciente %s, contato: %s com o id: %d? (y/n)\n", nome, contato, pacientes->qtd);
    scanf(" %c", &choice);
    
    if(choice == 'n'){return;}
    printf("Adicionando...\n\n");
    

	file = fopen("pacientes.txt","a"); //a-append

	if(file == NULL){
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
void search_paciente(VetPacientes *pacientes){
    int id;
    if(pacientes->qtd == 0){
        printf("Não existe pacientes cadastrados.\n"); return;
    }
    printf("Digite o id do paciente: ");
    scanf("%d", &id);
    if(id >= pacientes->qtd || id<0){printf("Paciente não encotrado.\n");return;} //gambiarra
    printf("id: %d nome: %s contato: %s\n", pacientes->itens[id].id, pacientes->itens[id].nome, pacientes->itens[id].contato);
}

void read_pacientes(VetPacientes *pacientes){
    FILE *file;
    
    int i = 0;
	file = fopen("pacientes.txt", "r");

	if(file == NULL){
	    printf("erro");
	}
	while(fscanf(file, "%s %s %d\n", &pacientes->itens[i].nome, &pacientes->itens[i].contato, &pacientes->itens[i].id) != EOF){i++;}
	pacientes->qtd = i;
	
	fclose(file);
}

