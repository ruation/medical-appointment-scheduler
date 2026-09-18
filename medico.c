#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

/**
 * @brief Checks if the doctor vector capacity is full and expands it by 10 elements.
 * @param[in,out] medicos Pointer to the dynamic doctor vector.
 * @return 1 on successful reallocation or when capacity is sufficient; 0 on memory allocation failure.
 * @note Uses a temporary pointer to guard against data loss if realloc() fails.
 */
int realocar_medicos(VetMedicos *medicos) {
	// Check if current element count has reached allocated capacity
	if(medicos->qtd == medicos->cap) {

		Medico *medicos1;
		// Allocate to temporary pointer to prevent memory leaks if realloc fails
		medicos1 = (Medico*) realloc(medicos->itens, (medicos->cap+10) * sizeof(Medico));

		if(medicos1 != NULL) {
			// On successful reallocation, assign new buffer and increment capacity
			medicos->itens = medicos1;
			medicos->cap += 10;
			return 1;
		} else {
			return 0;
		}
	}
	return 1;
}

/**
 * @brief Maps an integer specialty code to its human-readable string representation.
 * @param[in] n Integer specialty identifier corresponding to the Especialidade enum.
 * @return Constant string literal representing specialty name, or "Especialidade invalida".
 */
const char* ler_especialidade(int n) {
	// Return the descriptive string corresponding to the specialty code
	switch(n) {
	case 0:
		return "Clinico";
	case 1:
		return "Pediatra";
	case 2:
		return "Dermatologista";
	case 3:
		return "Cardiologista";
	case 4:
		return "Outro";
	default:
		return "Especialidade invalida";
	}

}

/**
 * @brief Loads doctor records from "medicos.txt" into the in-memory dynamic vector.
 * @param[out] medicos Pointer to the dynamic doctor vector to initialize and populate.
 * @note If "medicos.txt" does not exist, allocates baseline capacity of 10 and creates an empty file.
 *       Otherwise, allocates (line_count + 10) capacity via malloc and deserializes pipe-delimited records.
 */
void read_medicos(VetMedicos *medicos) {
	FILE *file;

	file = fopen("medicos.txt","r");

	if(file == NULL) {
		// If no file exists, initialize empty dynamic array with baseline capacity of 10

		medicos->qtd = 0, medicos->cap = 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);
		// Verify successful heap allocation
		if(medicos->itens == NULL) {
			printf("Erro de memoria\n");
			return;
		}

		// Create a new empty flat file
		file = fopen("medicos.txt","w");
		if(file == NULL) {
			printf("Erro ao abrir o arquivo\n");
		}
		fclose(file);
		return;

	} else {

		// Count total lines in the file and allocate capacity for lines + 10 records
		int n1, n2, n3, n4, i = 0;
		// Variables n1..n4 store packed integer representations of shift times
		medicos->qtd = contar_linhas(file);

		medicos->cap = medicos->qtd + 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);

		// Verify heap allocation success
		if(medicos->itens == NULL) {
			printf("Erro ao realocar o vetor\n");
			fclose(file);
			return;
		}
		// Iterate through file lines, reading fields delimited by the pipe ('|') character.
		// %63[^|] reads up to 63 characters until the delimiter to support compound names.
		while(fscanf(file, "%d|%63[^|]|%d|%d|%d|%d|%d",&medicos->itens[i].id, medicos->itens[i].nome, &medicos->itens[i].especialidade, &n1,&n2,&n3,&n4) ==7) {
			// Unpack packed integer times (HHMM) into Horario structures
			desconverter_horas(n1,&medicos->itens[i].inicioManha);
			desconverter_horas(n2,&medicos->itens[i].fimManha);
			desconverter_horas(n3,&medicos->itens[i].inicioTarde);
			desconverter_horas(n4,&medicos->itens[i].fimTarde);
			i++;
		}
		// Set actual count of loaded doctor records
		medicos->qtd = i;
		fclose(file);
		return;
	}
}

/**
 * @brief Formats and displays a single doctor's record and shift schedule to stdout.
 * @param[in] medico Doctor structure containing the data to display.
 */
void mostrar_medico(Medico medico) {
	printf("Medico: %s      id: %d      Especildiade: %s\n", medico.nome,medico.id, ler_especialidade(medico.especialidade));
	printf("Horario de manhâ das %d:%d até %d:%d\n",medico.inicioManha.horas, medico.inicioManha.minutos, medico.fimManha.horas, medico.fimManha.minutos);
	printf("Horario de tarde das %d:%d até %d:%d\n",medico.inicioTarde.horas, medico.inicioTarde.minutos, medico.fimTarde.horas, medico.fimTarde.minutos);

}

/**
 * @brief Interactively collects doctor details, appends to "medicos.txt", and adds to in-memory vector.
 * @param[in,out] medicos Pointer to the dynamic doctor vector.
 * @note Reallocates array capacity if full (+10). Generates an auto-incrementing ID.
 *       Enforces 3-hour fixed duration for morning and afternoon shifts.
 */
void add_medico(VetMedicos *medicos) {
	int maior = 0, choise1;
	char choise;
	// Temporary structure to hold inputs until user confirms registration
	Medico medico;

	// Verify and expand vector capacity if required
	choise1 = realocar_medicos(medicos);

	if(choise1==0) {
		printf("Não há mais memória para armazenar novos medicos. Encerrando função adicionar.\n");
		return;
	}

	// Find the highest existing doctor ID and assign ID = highest + 1
	if(medicos->qtd > 0) {
		for(int i=0; i<medicos->qtd; i++) {
			if(medicos->itens[i].id>=maior) {
				maior = medicos->itens[i].id;
			}
		}
	}
	medico.id = maior+1;
    do{
        printf("Digite o nome do medico: ");
    	getchar();
    	fgets(medico.nome,64, stdin);
    	medico.nome[strcspn(medico.nome, "\n")] = '\0';
    }while(verify_name(medico.nome)!= 1);
	
	// Collect and validate medical specialty option
	while(1) {
		printf("Escolha uma das seguintes especialidade do médico\nClinico: 1\nPediatra: 2\nDermatologista: 3\nCardiologista: 4\nOutra: 5\n");
		scanf("%d",&choise1);
		if(choise1>=1 && 5>=choise1) {
			break;
		}
		else {
			printf("Digite uma opção válida\n");
		}
	}

	medico.especialidade = choise1 - 1;

	printf("Adendo: cada turno do medico vai ser calculado com 3 horas de duração\n");

	printf("    Preencha o horário do expediente do inicio da manhã\n");
	receber_hora(&medico.inicioManha);
	veri_horario(&medico.inicioManha,&medico.fimManha,1, 1);
	
	printf("    Preencha o horário do expediente do inicio da tarde\n");
	receber_hora(&medico.inicioTarde);
	veri_horario(&medico.inicioTarde,&medico.fimTarde,1, 2);

	printf("        Deseja adicionar\n");
	mostrar_medico(medico);

	while(1) {
		printf("Digite y para confirmar ou n para cancelar\n");
		scanf(" %c", &choise);
		if(choise == 'n')return;
		if(choise == 'y')break;
	}
	// Open file only after confirmation to prevent dangling or corrupted files on cancellation
	FILE *file;

	file = fopen("medicos.txt", "a");
	if(file==NULL) {
		printf("Erro de memoria\n");
		return;
	}
	printf("Adicionando...\n\n");
	fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medico.id, medico.nome, medico.especialidade,con_horas(medico.inicioManha),con_horas(medico.fimManha),con_horas(medico.inicioTarde),con_horas(medico.fimTarde));

	fclose(file);

	// Insert confirmed record into in-memory array
	medicos->itens[medicos->qtd] = medico;

	medicos->qtd++;
}

/**
 * @brief Interactively searches for a doctor by unique ID via linear scan.
 * @param[in] medicos Pointer to the dynamic doctor vector.
 * @return Vector index of the doctor if found; -1 if not found; -2 if vector is empty.
 * @note Displays doctor details using mostrar_medico() upon finding a match.
 */
int pesquisar_medicos(VetMedicos *medicos) {
	int numero, i;
	Medico medico1;
	
	if(medicos->qtd < 1){printf("Não existe medicos cadastrados.\n");return -2;}
	
	printf("Digite o id do medico desejado\n");
	scanf("%d", &numero);
	// Scan the vector until finding the doctor with the matching ID

	for(i = 0; i < medicos->qtd && numero!=medicos->itens[i].id; i++);
	// Verify if a valid match was found
	if( i < medicos->itens[i].id && numero!=0) {
		if(numero == medicos->itens[i].id) {
			printf("Medico encontrado\n");
			mostrar_medico(medicos->itens[i]);
			return i;
		}
	} else {
		printf("Medico não encontrado no sistema\n");
		return -1;
	}
}

/**
 * @brief Interactively updates an existing doctor's attributes and persists changes to "medicos.txt".
 * @param[in,out] medicos Pointer to the dynamic doctor vector.
 * @note Re-prompts all doctor attributes, updates in-memory vector, and overwrites "medicos.txt".
 */
void update_medicos(VetMedicos *medicos){
    
    if(medicos->qtd == 0){
	    printf("Sem medicos cadastradas no sistema\n");
	    return;
	}
    char choise;
    int i, choise1;
    Medico medico;
    
    i = pesquisar_medicos(medicos);
    
    if(i == -1)return;
    do{
        printf("Deseja alterar aos dados desse medico?(y/n)\n");
        scanf(" %c", &choise);
        if(choise == 'n')return;
    }while(choise != 'y');
    
    do{
        printf("Digite o nome do medico: ");
    	getchar();
    	fgets(medico.nome,64, stdin);
    	medico.nome[strcspn(medico.nome, "\n")] = '\0';
    }while(verify_name(medico.nome)!= 1);
	
	while(1) {
		printf("Escolha uma das seguintes especialidade do médico\nClinico: 1\nPediatra: 2\nDermatologista: 3\nCardiologista: 4\nOutra: 5\n");
		scanf("%d",&choise1);
		if(choise1>=1 && 5>=choise1) {
			break;
		}
		else {
			printf("Digite uma opção válida\n");
		}
	}

	medico.especialidade = choise1 - 1;

	printf("Adendo: cada turno do medico deve ser de exatamente 3 horas de duração\n");

	printf("    Preencha o horário da consulta do inicio da manhã\n");
	receber_hora(&medico.inicioManha);
	veri_horario(&medico.inicioManha,&medico.fimManha,1, 1);
	printf("    Preencha o horário de consulta do inicio da tarde\n");
	receber_hora(&medico.inicioTarde);
	veri_horario(&medico.inicioTarde,&medico.fimTarde,1, 2);

	printf("Medico atualizado\n");
	mostrar_medico(medico);

	FILE *file;
    
	file = fopen("medicos.txt", "w");
	
	medico.id = medicos->itens[i].id;
	medicos->itens[i] = medico;
	
	printf("Atualizando...\n\n");
	for(i = 0; i < medicos->qtd;i++){
	    	fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medicos->itens[i].id, medicos->itens[i].nome, medicos->itens[i].especialidade,con_horas(medicos->itens[i].inicioManha),con_horas(medicos->itens[i].fimManha),con_horas(medicos->itens[i].inicioTarde),con_horas(medicos->itens[i].fimTarde));

	}
	fclose(file);
    
}

/**
 * @brief Removes a doctor by ID, rewrites "medicos.txt", and cascades deletion to linked appointments.
 * @param[in,out] medicos Pointer to the dynamic doctor vector.
 * @param[in,out] consultas Pointer to the dynamic appointment vector for cascade deletion.
 * @note Shifts elements leftward to overwrite deleted slot, rewrites "medicos.txt",
 *       and calls auto_del_consulta() on all appointments associated with this doctor.
 */
void remover_medico(VetMedicos *medicos, VetConsultas *consultas) {
	if(medicos->qtd == 0){
	    printf("Sem medicos cadastradas no sistema\n");
	    return;
	}
	int i, id;
	char choise;
	id = pesquisar_medicos(medicos);
	// Obtain the doctor index to delete
	if(id==-1)return;
	printf("Deseja remover esse medico?\n");

	while(1) {
		printf("Digite y para confirmar ou n para cancelar\n");
		scanf(" %c", &choise);
		if(choise == 'n')return;
		if(choise == 'y')break;
	}
	
	int id_medico = medicos->itens[id].id; // Retain doctor ID before element overwrite

	FILE *file;
	// Remove doctor by shifting all elements to the right of target one position to the left
	for(i = id; i < medicos->qtd-1 ; i++) {
		medicos->itens[i] = medicos->itens[i + 1];
	}
	medicos->qtd--;

	file = fopen("medicos.txt", "w");
	// Overwrite file with updated vector records
	for(i = 0; i<medicos->qtd; i++) {
		fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medicos->itens[i].id, medicos->itens[i].nome, medicos->itens[i].especialidade,con_horas(medicos->itens[i].inicioManha),con_horas(medicos->itens[i].fimManha),con_horas(medicos->itens[i].inicioTarde),con_horas(medicos->itens[i].fimTarde));
	}

	fclose(file);
	printf("Removendo medico...\n");
	
	for(int i = 0; i < consultas->qtd; i++){
        if(consultas->itens[i].idMedico == id_medico){auto_del_consulta(consultas, i); i--;}
    }
}

/**
 * @brief Displays doctors, providing an interactive choice to list all or filter by specialty.
 * @param[in] medicos Pointer to the dynamic doctor vector.
 * @note Displays total registered doctor count, and allows filtered viewing by specialty.
 */
void listar_medicos(VetMedicos *medicos) {
	
	if(medicos->qtd == 0){
	    printf("Sem medicos cadastradas no sistema\n");
	    return;
	}
	int choise,i, flag = 0;
	char choise1;

	printf("Quantidade total de medicos cadastrados no sistema: %d\n", medicos->qtd);

    while(1) {
			printf("1 - listar todos\n2 - especialidade especifica\n");
			scanf("%d",&choise);
			if(choise==1 || 2==choise) {
				break;
			}
			else {
				printf("Digite uma opção válida\n");
			}
		}
	if(choise == 1){
	    for(i = 0; i<medicos->qtd; i++) {
			mostrar_medico(medicos->itens[i]);
			printf("\n");
		}
	}else{
	    while(1) {
    		// Prompt for specialty filter and validate selection
    		while(1) {
    			printf("Escolha uma das seguintes especialidade do médico para filtrar e listar\nClinico: 1\nPediatra: 2\nDermatologista: 3\nCardiologista: 4\nOutra: 5\n");
    			scanf("%d",&choise);
    			if(choise>=1 && 5>=choise) {
    				break;
    			}
    			else {
    				printf("Digite uma opção válida\n");
    			}
    		}
    
    		choise--;
    
    		for(i = 0; i<medicos->qtd; i++) {
    			if(choise == medicos->itens[i].especialidade) {
    				mostrar_medico(medicos->itens[i]);
    				printf("\n");
    				flag++;
    			}
    		}
    		// Inform user if no doctors exist with the selected specialty
    		if(flag==0) {
    			printf("Nenhum medico dessa especialidade cadastrado\n");
    			while(1) {
    				printf("Deseja filtrar por outra especialdiade?\nDigite y para confirmar ou n para cancelar\n");
    				scanf(" %c", &choise);
    				if(choise == 'n')return;
    				if(choise == 'y')break;
    			}
    		}
    		else {
    			return;
    		}
    	}
	}
	
}