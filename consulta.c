#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

/**
 * @brief Interactively schedules a new appointment after verifying doctor and patient existence and slot availability.
 * @param[in,out] consultas Pointer to the dynamic appointment vector.
 * @param[in,out] pacientes Pointer to the dynamic patient vector (searched for patient selection).
 * @param[in,out] medicos Pointer to the dynamic doctor vector (searched for doctor selection).
 * @note Reallocates appointment vector capacity if full (+10). Ensures the selected time slot
 *       does not conflict with doctor shifts or existing bookings. Appends to "consultas.txt".
 */
void add_consulta(VetConsultas *consultas, VetPacientes *pacientes, VetMedicos *medicos){
    int choise1, status = 0; // Initialize appointment with scheduled status (CONS_AGENDADA)
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
    } // Find the highest ID among existing appointments
    
    FILE *file;
    
    file = fopen("consultas.txt", "a");
    
    fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n", maior+1, medicos->itens[id_medico].id, pacientes->itens[id_paciente].id, con_data(data), con_horas(inicio), con_horas(fim), status);
    
    fclose(file);
    
    // Store confirmed appointment record into the in-memory vector
    
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

/**
 * @brief Validates proposed appointment time against doctor shift bounds and existing appointment conflicts.
 * @param[in] medicos Pointer to the dynamic doctor vector.
 * @param[in] consultas Pointer to the dynamic appointment vector.
 * @param[in] data Proposed date of the appointment.
 * @param[in] inicio Proposed start time of the appointment.
 * @param[in] id_medico Array index of the doctor in the medicos vector.
 * @return 1 if slot is valid and free of conflict; 0 if outside shift or in conflict.
 * @note Checks morning or afternoon shift boundaries, and verifies that no scheduled appointment
 *       (status == 0) exists for the same doctor on the same date within 100 time units (1 hour).
 */
int verify_consulta(VetMedicos *medicos, VetConsultas *consultas, Data data, Horario inicio, int id_medico){
    int data_con = con_data(data);// Pack date into integer for straightforward comparison
    
    int flag = 1;
    
    if(inicio.horas>12){// Branch validation depending on morning or afternoon shift
        
        if( con_horas(medicos->itens[id_medico].inicioTarde) > con_horas(inicio)){ // Ensure consultation start is not prior to doctor afternoon shift start
            printf("A consulta não pode ser antes do expediente do médico.\n"); return 0;
        }else{
            if( con_horas(medicos->itens[id_medico].fimTarde) < (con_horas(inicio) + 100)){ // Ensure consultation end does not exceed afternoon shift end
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
    
    for(int i = 0; i < consultas->qtd; i++){// Loop through all registered appointments to check for overlap
        if(con_data(consultas->itens[i].data) == data_con && consultas->itens[i].idMedico == medicos->itens[id_medico].id){ // Match by date and doctor ID
            if(abs(con_horas(consultas->itens[i].inicio) - con_horas(inicio)) < 100 && consultas->itens[i].status == 0)flag = 0;
        }
    }
    
    if(flag == 0){printf("O medico ja tem uma consulta nesse horario, escolha outro.\n"); return 0;}
    return 1;
}

/**
 * @brief Maps an integer status code to its symbolic enum representation name.
 * @param[in] n Status integer code (0-3).
 * @return Constant string literal corresponding to the status, or "Status invalido".
 */
const char* ler_status(int n) {
	// Return the descriptive string corresponding to the appointment status code
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

/**
 * @brief Displays formatted details of a single appointment record to stdout.
 * @param[in] consultas Pointer to the appointment record to print.
 */
void printar_consultas(Consulta *consultas){
    printf("id: %d id_medico: %d id_paciente: %d data: %d/%d/%d horario: %dh%d as %dh%d status: %s\n", consultas->id, consultas->idMedico, consultas->idPaciente, consultas->data.dia, consultas->data.mes, consultas->data.ano, consultas->inicio.horas, consultas->inicio.minutos, consultas->fim.horas, consultas->fim.minutos, ler_status(consultas->status));
}

/**
 * @brief Displays appointments, providing an option to view all or filter by lifecycle status.
 * @param[in] consultas Pointer to the dynamic appointment vector.
 * @note Status filter options: 1 = All, 2 = Scheduled, 3 = Completed, 4 = Canceled, 5 = Missed.
 */
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
        }
    }
}

/**
 * @brief Packs a Data structure into a single integer formatted as YYYYMMDD.
 * @param[in] data Data struct containing day, month, and year.
 * @return Packed integer calculated as (ano * 10000) + (mes * 100) + dia.
 */
int con_data(Data data){
    data.ano *= 10000;
    data.mes *= 100;
    return data.dia + data.mes + data.ano;
}

/**
 * @brief Interactively prompts the user to input and validate date components.
 * @param[out] data Pointer to Data structure where validated day, month, and year are written.
 * @note Enforces day in [1, 30], month in [1, 12], and year >= 2026.
 */
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

/**
 * @brief Unpacks an integer in YYYYMMDD format into day, month, and year in a Data struct.
 * @param[out] data Pointer to Data structure where unpacked components will be stored.
 * @param[in] n Packed date integer in YYYYMMDD format.
 */
void descon_data(Data *data, int n){
    data->ano = n / 10000; 
    data->mes = n % 10000 / 100;
    data->dia = n % 10000 % 100;
}

/**
 * @brief Loads appointment records from "consultas.txt" into the in-memory dynamic vector.
 * @param[out] consultas Pointer to the dynamic appointment vector to initialize and populate.
 * @note If "consultas.txt" does not exist, allocates baseline capacity of 10 and creates an empty file.
 *       Otherwise, allocates (line_count + 10) elements via malloc and deserializes pipe-delimited records.
 */
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

		// Create a new empty flat file
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

/**
 * @brief Checks if the appointment vector capacity is full and expands it by 10 elements.
 * @param[in,out] consultas Pointer to the dynamic appointment vector.
 * @return 1 on successful reallocation or when capacity is sufficient; 0 on memory allocation failure.
 * @note Employs a temporary pointer with realloc() to prevent memory leaks on allocation failure.
 */
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

/**
 * @brief Interactively searches for an appointment by unique ID via linear scan of the vector.
 * @param[in] consultas Pointer to the dynamic appointment vector.
 * @return Vector index of the appointment if found; -1 if not found or vector is empty.
 * @note Displays appointment details using printar_consultas() upon finding a match.
 */
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
			return i; // Return the array index of the matched appointment
		}
	} else {
		printf("Consulta não encontrado no sistema\n");
		return -1;
	}
}

/**
 * @brief Interactively removes an appointment by ID, updates vector, and rewrites "consultas.txt".
 * @param[in,out] consultas Pointer to the dynamic appointment vector.
 * @note Prompts for ID, prompts confirmation, shifts elements leftward, and rewrites the flat file.
 */
void del_consulta(VetConsultas *consultas){
    
    if(consultas->qtd == 0){
        printf("Nenhuma consulta cadastrada no sistema\n");
        return;
    }
    int i, id;
	char choise;
	
	id = search_consultas(consultas); // Find array index of target appointment

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

	file = fopen("consultas.txt", "w"); // Rewrite the file with all remaining records

	for(i = 0; i< consultas->qtd; i++) {
        fprintf(file, "%d | %d | %d | %d | %d | %d | %d\n",consultas->itens[i].id ,consultas->itens[i].idMedico ,consultas->itens[i].idPaciente, con_data(consultas->itens[i].data), con_horas(consultas->itens[i].inicio) ,con_horas(consultas->itens[i].fim) ,consultas->itens[i].status );
	}

	fclose(file);
	printf("Consulta removida\n");
}

/**
 * @brief Programmatically removes an appointment by array index without user prompt.
 * @param[in,out] consultas Pointer to the dynamic appointment vector.
 * @param[in] id Array index of the appointment to delete.
 * @note Shifts remaining elements leftward and rewrites "consultas.txt". Used in cascade deletions.
 */
void auto_del_consulta(VetConsultas *consultas, int id){

    // Function accepts target consultation index directly as argument for automated cascade deletion
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

/**
 * @brief Interactively updates an appointment's lifecycle status and rewrites "consultas.txt".
 * @param[in,out] consultas Pointer to the dynamic appointment vector.
 * @note Prompts for appointment ID via search_consultas(), allows selecting new status
 *       (1: Scheduled, 2: Completed, 3: Canceled, 4: Missed), updates vector, and rewrites file.
 */
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