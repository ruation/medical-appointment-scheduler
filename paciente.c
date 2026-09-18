#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

/**
 * @brief Validates basic email formatting rules for patient contact entries.
 * @param[in] str Null-terminated string containing the email address.
 * @return 1 if the email format is valid, 0 otherwise.
 * @note Enforces email rules: exactly one '@', at least one '.' after '@',
 *       no consecutive dots, and no leading/trailing dots or '@'.
 */
int verify_email(const char *str){
    if(*str == '\0') return 0; // Return 0 if str is empty
    int qtd_arroba = 0, flag_arroba = -1, flag_ponto = -1, i; 
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] == '@'){flag_arroba = i; qtd_arroba++;
            if(i == 0){printf("e-mail inválido.\n"); return 0;} // The first char cannot be an "@"
            else if(str[i-1] == '.' || str[i+1] == '.'){printf("e-mail inválido.\n"); return 0;} // Char immediately before or after "@" cannot be a "."
        }
        else if(str[i] == '.'){flag_ponto = i;
            if(i == 0 || str[i+1] == '.'){printf("e-mail inválido.\n"); return 0;} // First char cannot be "." and cannot have consecutive "."
        }
    }
    
    if(qtd_arroba != 1 || flag_ponto == -1 || flag_ponto < flag_arroba || str[i-1] == '.'){
        printf("e-mail inválido.\n"); return 0; // Must have exactly 1 "@", at least 1 "." after "@", and last char cannot be "."
    }
    return 1;
    
}

/**
 * @brief Interactively creates a new patient record, appends it to "pacientes.txt", and adds it to the vector.
 * @param[in,out] pacientes Pointer to the dynamic patient vector.
 * @note Automatically reallocates array capacity if full (+10). Generates an auto-incrementing ID.
 *       Appends record to "pacientes.txt" in format "Nome | Contato | ID".
 */
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


	printf("Adicionar o paciente %s, contato: %s com o id: %d? (y/n)\n", nome, contato, maior+1); // Store in local variables prior to vector insertion
    while(1){
        scanf(" %c", &choice);
    
    if(choice == 'n') return;
    if(choice=='y')break;
    }

	printf("Adicionando...\n\n");


	file = fopen("pacientes.txt","a"); // Open in append mode

	if(file == NULL) {
		printf("Erro ao abrir arquivo");
		return;
	}

	// Write the formatted record to the persistent flat file
	fprintf(file, "%s | %s | %d\n", nome, contato, maior+1);

	fclose(file);

	strcpy(pacientes->itens[pacientes->qtd].nome, nome);
	strcpy(pacientes->itens[pacientes->qtd].contato, contato);
	pacientes->itens[pacientes->qtd].id = maior+1;

	pacientes->qtd++;

	printf("Paciente cadastrado.\n");


}

/**
 * @brief Interactively searches for a patient by unique ID via linear scan of the vector.
 * @param[in] pacientes Pointer to the dynamic patient vector.
 * @return Array index of the patient if found; -1 if not found; -2 if the vector contains no records.
 * @note Displays patient ID, name, and contact details to stdout upon successful match.
 */
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
            return i; // Return the array index of the matched patient
        }
    }
    printf("Paciente não encontrado no sistema.\n");
    return -1;
}

/**
 * @brief Loads patient records from "pacientes.txt" into the in-memory dynamic vector.
 * @param[out] pacientes Pointer to the dynamic patient vector to initialize and populate.
 * @note If the file does not exist, allocates baseline capacity of 10 and creates an empty file.
 *       Otherwise, allocates (line_count + 10) elements via malloc and parses records with fscanf.
 */
void read_pacientes(VetPacientes *pacientes) {
	FILE *file;

	int i = 0;
	file = fopen("pacientes.txt", "r");

	if(file == NULL) {// Initialize with baseline capacity if file does not exist
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
	pacientes->itens = (Paciente *) malloc(sizeof(Paciente) * pacientes->cap); // Initialize heap memory with appropriate capacity

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

/**
 * @brief Expands the capacity of the patient vector by 10 elements if the array is full.
 * @param[in,out] pacientes Pointer to the dynamic patient vector.
 * @return 1 on successful reallocation or when capacity is sufficient; 0 on memory allocation failure.
 * @note Uses realloc() to expand memory block size by sizeof(Paciente) * (cap + 10).
 */
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

/**
 * @brief Removes a patient by ID, rewrites "pacientes.txt", and cascades deletion to linked appointments.
 * @param[in,out] pacientes Pointer to the dynamic patient vector.
 * @param[in,out] consultas Pointer to the dynamic appointment vector for cascading deletion.
 * @note Shifts remaining elements leftward in memory, rewrites "pacientes.txt", and invokes
 *       auto_del_consulta() for every consultation associated with this patient's ID.
 */
void remover_paciente(VetPacientes *pacientes, VetConsultas *consultas){
    
    if(pacientes->qtd == 0){
	    printf("Sem pacientes cadastradas no sistema\n");
	    return;
	}
    int id = search_paciente(pacientes);
    char choice;
    if(id == -1){return;}
    printf("Deseja remover esse paciente? (y/n)\n");
    scanf(" %c", &choice);
    if(choice == 'n')return;
    
    FILE *file;
    
    int id_paciente = pacientes->itens[id].id; // Save patient ID before element overwrite
    
    for(int i = id; i<pacientes->qtd-1; i++){
        pacientes->itens[i] = pacientes->itens[i+1]; // Shift elements leftward to overwrite deleted patient
    }
    pacientes->qtd--;
    
    file = fopen("pacientes.txt", "w");
    
    for(int i = 0; i<pacientes->qtd; i++){
        fprintf(file, "%s | %s | %d\n", pacientes->itens[i].nome, pacientes->itens[i].contato, pacientes->itens[i].id);
    }
    fclose(file);
    printf("Paciente removido.\n");
    
    for(int i = 0; i < consultas->qtd; i++){
        if(consultas->itens[i].idPaciente == id_paciente){auto_del_consulta(consultas, i); i--;} // Cascade delete all linked appointments
    }
    
}

/**
 * @brief Interactively updates an existing patient's details and persists changes to "pacientes.txt".
 * @param[in,out] pacientes Pointer to the dynamic patient vector.
 * @note Searches for patient by ID, collects updated fields, updates the vector, and rewrites the file.
 */
void update_pacientes(VetPacientes *pacientes){
    
    if(pacientes->qtd == 0){
	    printf("Sem pacientes cadastradas no sistema\n");
	    return;
	}
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

/**
 * @brief Prints all registered patients in the vector to standard output.
 * @param[in] pacientes Pointer to the dynamic patient vector.
 */
void list_pacientes(VetPacientes *pacientes){
    for(int i = 0; i < pacientes->qtd; i++){
        printf("id: %d nome: %s contato: %s\n", pacientes->itens[i].id, pacientes->itens[i].nome, pacientes->itens[i].contato);
    }
}