#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

int verify_email(const char *str){
    if(*str == '\0') return 0; // return 0 if str is empty
    int qtd_arroba = 0, flag_arroba = -1, flag_ponto = -1, i; 
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] == '@'){flag_arroba = i; qtd_arroba++;
            if(i == 0){printf("e-mail inválido.\n"); return 0;} // the first char cannot be an "@"
            else if(str[i-1] == '.' || str[i+1] == '.'){printf("e-mail inválido.\n"); return 0;} // the last char before "@" or the first one after "@" cannot be an "."
        }
        else if(str[i] == '.'){flag_ponto = i;
            if(i == 0 || str[i+1] == '.'){printf("e-mail inválido.\n"); return 0;} // the first char cannot be an "." and a email cannot have two "." together.
        }
    }
    
    if(qtd_arroba != 1 || flag_ponto == -1 || flag_ponto < flag_arroba || str[i-1] == '.'){
        printf("e-mail inválido.\n"); return 0; // e-mail only have 1 "@" and need at least 1 "." after the "@". The last char cannot be an "."
    }
    return 1;
    
}
void add_paciente(VetPacientes *pacientes){
    FILE *file;
    char nome[64];char contato[64]; char choice;
    
    int flag = realocar_pacientes(pacientes);
    
    if(flag == 0){
        return;
    }
    
    do{printf("Digite o nome do paciente: ");
        getchar();
        fgets(nome,64, stdin);
        nome[strcspn(nome, "\n")] = '\0';
    }while(verify_name(nome)==0);
    
    do{printf("Digite o e-mail do paciente: ");
    fgets(contato,64, stdin);
    contato[strcspn(contato, "\n")] = '\0';
    }while(verify_email(contato)==0);
    
    	
	int maior = 0;
	if(pacientes->qtd > 0) {
		for(int i = 0; i<pacientes->qtd; i++) {
			if(pacientes->itens[i].id > maior) {
				maior = pacientes->itens[i].id;
			}
		}
	}


	printf("Adicionar o paciente %s, contato: %s com o id: %d? (y/n)\n", nome, contato, maior+1);
    while(1){
        scanf(" %c", &choice);
    
    if(choice == 'n') return;
    if(choice=='y')break;
    }

	printf("Adicionando...\n\n");


	file = fopen("pacientes.txt","a"); //a-append

	if(file == NULL) {
		printf("Erro ao abrir arquivo");
		return;
	}

	//fprintf é uma função para escrever no arquivo.
	fprintf(file, "%s | %s | %d\n", nome, contato, maior+1);

	fclose(file);

	strcpy(pacientes->itens[pacientes->qtd].nome, nome);
	strcpy(pacientes->itens[pacientes->qtd].contato, contato);
	pacientes->itens[pacientes->qtd].id = maior+1;

	pacientes->qtd++;

	printf("Paciente cadastrado.\n");


}

int search_paciente(VetPacientes *pacientes) {
	int id, i;
	if(pacientes->qtd == 0) {
		printf("Não existe pacientes cadastrados.\n");
		return -2;
	}
	printf("Digite o id do paciente: ");
	scanf("%d", &id);
    for(i=0; i<pacientes->qtd; i++){
        if(pacientes->itens[i].id == id){
            printf("id: %d nome: %s contato: %s\n", pacientes->itens[i].id, pacientes->itens[i].nome, pacientes->itens[i].contato);
            return i;
        }
    }
    printf("Paciente não encontrado no sistema.\n");
    return -1;
}

void read_pacientes(VetPacientes *pacientes) {
	FILE *file;

	int i = 0;
	file = fopen("pacientes.txt", "r");

	if(file == NULL) {
		pacientes->qtd = 0;
		pacientes->cap = 10;
		pacientes->itens = (Paciente *) malloc(sizeof(Paciente) * pacientes->cap);

		if(pacientes->itens == NULL) {
			printf("Erro de memoria\n");
			return;
		}

		file = fopen("pacientes.txt","w");

		if(file == NULL) {
			printf("Erro ao abrir o arquivo\n");
		}
		fclose(file);

		return;
	}

	pacientes->qtd = contar_linhas(file);
	pacientes->cap = pacientes->qtd + 10;
	pacientes->itens = (Paciente *) malloc(sizeof(Paciente) * pacientes->cap);

	if(pacientes->itens == NULL) {
		printf("Erro ao realocar o vetor\n");
		fclose(file);
		return;
	}


	while(fscanf(file, "%63[^|] | %s | %d\n", &pacientes->itens[i].nome, &pacientes->itens[i].contato, &pacientes->itens[i].id) != EOF) {
		i++;
	}
	pacientes->qtd = i;

	fclose(file);
}

int realocar_pacientes(VetPacientes *pacientes) {
	if(pacientes->qtd == pacientes->cap) {
		pacientes->itens = (Paciente *) realloc(pacientes->itens, sizeof(Paciente) * (pacientes->cap+10));
		if(pacientes->itens == NULL) {
			printf("Erro de memoria!\n");
			return 0;
		}
		pacientes->cap += 10;
	}
	return 1;
}

void remover_paciente(VetPacientes *pacientes){
    int id = search_paciente(pacientes);
    char choice;
    if(id == -1){return;}
    printf("Deseja remover esse paciente? (y/n)\n");
    scanf(" %c", &choice);
    if(choice == 'n')return;
    
    FILE *file;
    
    for(int i = id; i<pacientes->qtd-1; i++){
        pacientes->itens[i] = pacientes->itens[i+1];
    }
    pacientes->qtd--;
    
    file = fopen("pacientes.txt", "w");
    
    for(int i = 0; i<pacientes->qtd; i++){
        fprintf(file, "%s | %s | %d\n", pacientes->itens[i].nome, pacientes->itens[i].contato, pacientes->itens[i].id);
    }
    fclose(file);
    printf("Paciente removido.\n");
    
}

void update_pacientes(VetPacientes *pacientes){
    
    char choise;
    int i, choise1;
    Paciente paciente;
    
    i = search_paciente(pacientes);
    
    if(i == -1)return;
    do{
        printf("Deseja alterar aos dados desse paciente?(y/n)\n");
        scanf(" %c", &choise);
        if(choise == 'n')return;
    }while(choise != 'y');
    
    do{printf("Digite o nome do paciente: ");
        getchar();
        fgets(paciente.nome,64, stdin);
        paciente.nome[strcspn(paciente.nome, "\n")] = '\0';
    }while(verify_name(paciente.nome)==0);
    
    do{printf("Digite o e-mail do paciente: ");
    fgets(paciente.contato,64, stdin);
    paciente.contato[strcspn(paciente.contato, "\n")] = '\0';
    }while(verify_email(paciente.contato)==0);
    
    paciente.id = pacientes->itens[i].id;
    
    printf("Paciente atualizado\nNome: %s\nContato: %s\nId: %d\n", paciente.nome, paciente.contato,paciente.id);

	FILE *file;
    
	file = fopen("pacientes.txt", "w");
	
	pacientes->itens[i] = paciente;
	
	printf("Atualizando...\n\n");
	for(i = 0; i < pacientes->qtd;i++){
	    	fprintf(file, "%s | %s | %d \n",pacientes->itens[i].nome, pacientes->itens[i].contato, pacientes->itens[i].id );

	}
	fclose(file);
}
void list_pacientes(VetPacientes *pacientes){
    for(int i = 0; i < pacientes->qtd; i++){
        printf("id: %d nome: %s contato: %s\n", pacientes->itens[i].id, pacientes->itens[i].nome, pacientes->itens[i].contato);
    }
}
