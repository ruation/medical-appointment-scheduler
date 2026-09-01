#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

void add_consulta(VetConsultas *consultas, VetPacientes *pacientes, VetMedicos *medicos){
    int choise1, status = 0; //teste
    int id_paciente, id_medico;
    char choice;
    
    if(realocar_consultas(consultas) == 0){
        printf("Sem memoria para adicionar novas consultas\n");
        return;
    }
    do{id_paciente = search_paciente(pacientes);if(id_paciente==-2)return;}while(id_paciente==-1);
    do{id_medico = pesquisar_medicos(medicos);if(id_medico==-2)return;}while(id_medico==-1);
    
    for(int i = 0; i<consultas->qtd; i++){
            if(consultas->itens[i].idMedico == medicos->itens[id_medico].id && consultas->itens[i].status == 0)printf("id: %d id_medico: %d id_paciente: %d data: %d/%d/%d horario: %dh%d as %dh%d status: %s\n", consultas->itens[i].id, consultas->itens[i].idMedico, consultas->itens[i].idPaciente, consultas->itens[i].data.dia, consultas->itens[i].data.mes, consultas->itens[i].data.ano, consultas->itens[i].inicio.horas, consultas->itens[i].inicio.minutos, consultas->itens[i].fim.horas, consultas->itens[i].fim.minutos, ler_status(consultas->itens[i].status));
        }
    Data data; Horario inicio, fim;
    
    do{add_data(&data);
    
    printf("Horario de inicio. ");
    receber_hora(&inicio); 
    veri_horario(&inicio, &fim, 2, 0);
    }while(verify_consulta(medicos, consultas, data, inicio, id_medico) == 0);
    
    printf("agendar consulta? (y/n)");
    while(1){
        scanf(" %c", &choice);
    
    if(choice == 'n') return;
    if(choice=='y')break;
    }
    
    int maior = 0;
    if(consultas->qtd > 0){
        for(int i = 0; i < consultas->qtd; i++){
            if(consultas->itens[i].id > maior) maior = consultas->itens[i].id;
        }
    }
    
    FILE *file;
    
    file = fopen("consultas.txt", "a");
    
    fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n", maior+1, medicos->itens[id_medico].id, pacientes->itens[id_paciente].id, con_data(data), con_horas(inicio), con_horas(fim), status);
    
    fclose(file);
    
    //feeding the vector:
    
    consultas->itens[consultas->qtd].id = maior+1;
    consultas->itens[consultas->qtd].idPaciente = pacientes->itens[id_paciente].id;
    consultas->itens[consultas->qtd].idMedico = medicos->itens[id_medico].id;
    consultas->itens[consultas->qtd].data.dia = data.dia;
    consultas->itens[consultas->qtd].data.mes = data.mes;
    consultas->itens[consultas->qtd].data.ano = data.ano;
    consultas->itens[consultas->qtd].inicio.horas = inicio.horas;
    consultas->itens[consultas->qtd].inicio.minutos = inicio.minutos;
    consultas->itens[consultas->qtd].fim.horas = fim.horas;
    consultas->itens[consultas->qtd].fim.minutos = fim.minutos;
    consultas->itens[consultas->qtd].status = 0;
    
    consultas->qtd++;
    printf("Consulta agendada.\n");
    
}

int verify_consulta(VetMedicos *medicos, VetConsultas *consultas, Data data, Horario inicio, int id_medico){
    int data_con = con_data(data);//convertendo pra ficar mais facil
    
    int flag = 1;
    
    if(inicio.horas>12){// Separa a condicional para o caso da consulta ser de tarde ou ser de manhã.
        
        if( con_horas(medicos->itens[id_medico].inicioTarde) > con_horas(inicio)){ //Verifica se o inicio da consulta é antes do inicioTarde do médico.
            printf("A consulta não pode ser antes do expediente do médico.\n"); return 0;
        }else{
            if( con_horas(medicos->itens[id_medico].fimTarde) < (con_horas(inicio) + 100)){ //Verifica se o fim da consulta é depois do fimTarde do médico.
            printf("A consulta não pode terminar depois do expediente do médico.\n"); return 0;}
        }
    }else{
        if( con_horas(medicos->itens[id_medico].inicioManha) > con_horas(inicio)){
            printf("A consulta não pode ser antes do expediente do médico.\n"); return 0;
        }else{
            if( con_horas(medicos->itens[id_medico].fimManha) < (con_horas(inicio) + 100)){
            printf("A consulta não pode terminar depois do expediente do médico.\n"); 
            return 0;}
        }
        
    }
    
    for(int i = 0; i < consultas->qtd; i++){//looping para ver todas as consultas cadastradas.
        if(con_data(consultas->itens[i].data) == data_con && consultas->itens[i].idMedico == medicos->itens[id_medico].id){ //Verifica se existe uma consulta cadastrada com a mesma data e o mesmo médico.
            if(abs(con_horas(consultas->itens[i].inicio) - con_horas(inicio)) < 100 && consultas->itens[i].status == 0)flag = 0;
        }
    }
    
    if(flag == 0){printf("O medico ja tem uma consulta nesse horario, escolha outro.\n"); return 0;}
    return 1;
}

const char* ler_status(int n) {
	//Recebe um numero do status e devolve a string correta
	switch(n) {
	case 0:
		return "CONS_AGENDADA";
	case 1:
		return "CONS_CONCLUIDA";
	case 2:
		return "CONS_CANCELADA";
	case 3:
		return "CONS_FALTA";
	default:
		return "Status invalido";
	}

}
void printar_consultas(Consulta *consultas){
    printf("id: %d id_medico: %d id_paciente: %d data: %d/%d/%d horario: %dh%d as %dh%d status: %s\n", consultas->id, consultas->idMedico, consultas->idPaciente, consultas->data.dia, consultas->data.mes, consultas->data.ano, consultas->inicio.horas, consultas->inicio.minutos, consultas->fim.horas, consultas->fim.minutos, ler_status(consultas->status));
}

void list_consultas(VetConsultas *consultas){
    
    if(consultas->qtd == 0){
	    printf("Sem consultas cadastradas no sistema\n");
	    return;
	}
    int choice = 0;
    printf("1 - Todas\n2 - Agendadas\n3 - Concluidas\n4 - Canceladas\n5 - Faltas\n");
    do{scanf("%d", &choice);}while(choice > 5 || choice < 1);
    if(choice == 1){
        for(int i = 0; i < consultas->qtd; i++){
            printf("id: %d id_medico: %d id_paciente: %d data: %d/%d/%d horario: %dh%d as %dh%d status: %s\n", consultas->itens[i].id, consultas->itens[i].idMedico, consultas->itens[i].idPaciente, consultas->itens[i].data.dia, consultas->itens[i].data.mes, consultas->itens[i].data.ano, consultas->itens[i].inicio.horas, consultas->itens[i].inicio.minutos, consultas->itens[i].fim.horas, consultas->itens[i].fim.minutos, ler_status(consultas->itens[i].status));
        }
    }else{
        for(int i = 0; i<consultas->qtd; i++){
            if(consultas->itens[i].status == (choice-2))printf("id: %d id_medico: %d id_paciente: %d data: %d/%d/%d horario: %dh%d as %dh%d status: %s\n", consultas->itens[i].id, consultas->itens[i].idMedico, consultas->itens[i].idPaciente, consultas->itens[i].data.dia, consultas->itens[i].data.mes, consultas->itens[i].data.ano, consultas->itens[i].inicio.horas, consultas->itens[i].inicio.minutos, consultas->itens[i].fim.horas, consultas->itens[i].fim.minutos, ler_status(consultas->itens[i].status));
    }}
}

int con_data(Data data){
    data.ano *= 10000;
    data.mes *= 100;
    return data.dia + data.mes + data.ano;
}
void add_data(Data *data){
    while(1){
        printf("Digite um dia para consulta:\n");
        scanf("%d", &data->dia);
        if(30 >= data->dia && data->dia >=1){
            break;
        }else{printf("Erro, digite um dia valido\n");}
    }
    while(1){
        printf("Digite um mes para consulta:\n");
        scanf("%d", &data->mes);
        if(12 >= data->mes && data->mes >=1){
            break;
        }else{printf("Erro, digite um mes valido\n");}
    }
    while(1){
        printf("Digite um ano para consulta:\n");
        scanf("%d", &data->ano);
        if(data->ano >=2026){
            break;
        }else{printf("Erro, digite um ano valido\n");}
    }
}
void descon_data(Data *data, int n){
    data->ano = n / 10000; 
    data->mes = n % 10000 / 100;
    data->dia = n % 10000 % 100;
}

void read_consultas(VetConsultas *consultas){
	FILE *file;

	file = fopen("consultas.txt","r");

	if(file == NULL) {

		consultas->qtd = 0, consultas->cap = 10;
		consultas->itens = (Consulta *) malloc(sizeof(Consulta) * consultas->cap);
		if(consultas->itens == NULL) {
			printf("Erro de memoria\n");
			return;
		}

		//cria um novo arquivo
		file = fopen("consultas.txt","w");
		if(file == NULL) {
			printf("Erro ao abrir o arquivo\n");
		}
		fclose(file);
		return;

	} else {

		int n1, n2,n3, i = 0;

		consultas->qtd = contar_linhas(file);

		consultas->cap = consultas->qtd + 10;
		consultas->itens = (Consulta *) malloc(sizeof(Consulta) * consultas->cap);

		if(consultas->itens == NULL) {
			printf("Erro ao realocar o vetor\n");
			fclose(file);
			return;
		}
		
		while(fscanf(file, "%d | %d | %d | %d | %d | %d | %d",&consultas->itens[i].id, &consultas->itens[i].idMedico, &consultas->itens[i].idPaciente, &n1,&n2,&n3, &consultas->itens[i].status) ==7) {
			descon_data(&consultas->itens[i].data, n1);
			desconverter_horas(n2,&consultas->itens[i].inicio);
			desconverter_horas(n3,&consultas->itens[i].fim);
			i++;
		}
		
		consultas->qtd = i;
		fclose(file);
		return;
	}
}

int realocar_consultas(VetConsultas *consultas) {
	if(consultas->qtd == consultas->cap) {
		Consulta *consultas1;
		consultas1 = (Consulta*) realloc(consultas->itens, (consultas->cap+10) * sizeof(Consulta));
		if(consultas1 != NULL) {
			consultas->itens = consultas1;
			consultas->cap += 10;
			return 1;
		} else {
			return 0;
		}
	}
	return 1;
}

int search_consultas(VetConsultas *consultas){
    int numero, i;
	
	if(consultas->qtd == 0){
	    printf("Sem consultas cadastradas no sistema\n");
	    return -1;
	}
	printf("Digite o id da consulta desejado\n");
	scanf("%d", &numero);
    
    printf("Quantidade de consultas: %d\n", consultas->qtd);
	for(i = 0; i < consultas->qtd && numero!=consultas->itens[i].id; i++);

	printf("ID de consultas: %d\n", consultas->itens[i].id);

	if( i < consultas->itens[i].id && numero!=0) {
		if(numero == consultas->itens[i].id) {
			printf("Consulta encontrado\n");
			printar_consultas(&consultas->itens[i]);
			return i;
		}
	} else {
		printf("Consulta não encontrado no sistema\n");
		return -1;
	}
}
void del_consulta(VetConsultas *consultas){
    
    if(consultas->qtd == 0){
        printf("Nenhuma consulta cadastrada no sistema\n");
        return;
    }
    int i, id;
	char choise;
	
	id = search_consultas(consultas);

	if(id==-1)return;
	printf("Deseja remover essa consulta?\n");
	while(1) {
		printf("Digite y para confirmar ou n para cancelar\n");
		scanf(" %c", &choise);
		if(choise == 'n')return;
		if(choise == 'y')break;
	}

	FILE *file;
	
	for(i = id; i < consultas->qtd-1 ; i++) {
		consultas->itens[i] = consultas->itens[i + 1];
	}
	consultas->qtd--;

	file = fopen("consultas.txt", "w");

	for(i = 0; i< consultas->qtd; i++) {
        fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n",consultas->itens[i].id ,consultas->itens[i].idMedico ,consultas->itens[i].idPaciente, con_data(consultas->itens[i].data), con_horas(consultas->itens[i].inicio) ,con_horas(consultas->itens[i].fim) ,consultas->itens[i].status );
	}

	fclose(file);
	printf("Consulta removida\n");
}

void auto_del_consulta(VetConsultas *consultas, int id){


	FILE *file;
	
	for(int i = id; i < consultas->qtd-1 ; i++) {
		consultas->itens[i] = consultas->itens[i + 1];
	}
	consultas->qtd--;

	file = fopen("consultas.txt", "w");

	for(int i = 0; i< consultas->qtd; i++) {
        fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n",consultas->itens[i].id ,consultas->itens[i].idMedico ,consultas->itens[i].idPaciente, con_data(consultas->itens[i].data), con_horas(consultas->itens[i].inicio) ,con_horas(consultas->itens[i].fim) ,consultas->itens[i].status );
	}

	fclose(file);
	printf("Consulta removida\n");
}

void update_status(VetConsultas *consultas){
    if(consultas->qtd == 0){
        printf("Nenhuma consulta cadastrada no sistema\n");
        return;
    }
    int i, id, choise1;
	char choise;
	
	i = search_consultas(consultas);
    if(i==-1)return;
    printf("Deseja atualizar o status dessa consulta?\n");
    while(1) {
		printf("Digite y para confirmar ou n para cancelar\n");
		scanf(" %c", &choise);
		if(choise == 'n')return;
		if(choise == 'y')break;
	}
    do{
        printf("1 - Agendada\n2 - Concluida\n3 - Cancelada\n4 - Faltada\n");
        scanf("%d", &choise1);
    }while(choise1 < 1 && choise1>4);
    
    consultas->itens[i].status = choise1 - 1;
    
    FILE *file;
    
    file = fopen("consultas.txt", "w");

	for(i = 0; i< consultas->qtd; i++) {
        fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n",consultas->itens[i].id ,consultas->itens[i].idMedico ,consultas->itens[i].idPaciente, con_data(consultas->itens[i].data), con_horas(consultas->itens[i].inicio) ,con_horas(consultas->itens[i].fim) ,consultas->itens[i].status );
	}

	fclose(file);
	printf("Consulta atualizada\n");
}