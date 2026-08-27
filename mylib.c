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
	printf("Horario escolhido: %d:%d\n",horario->horas,horario->minutos);
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

//Para a todo momento verificar o tamanho do vetor, caso precise aumentar. Adiciona mais 10

int realocar_medicos(VetMedicos *medicos) {
	//Para comparar se a quantidade do vetor ocupado chegou no limite
	if(medicos->qtd == medicos->cap) {

		Medico *medicos1;
		//declara um novo struct para garantir que o primeiro não vai se perder ao tentar realocar o espaço
		medicos1 = (Medico*) realloc(medicos->itens, (medicos->cap+10) * sizeof(Medico));

		if(medicos1 != NULL) {
			//caso der certo, o vetor medicos.itens será realocado para ter o novo tamanho
			medicos->itens = medicos1;
			medicos->cap += 10;
			return 1;
		} else {
			return 0;
		}
	}
	return 1;
}

//Para converter o numero da especialidade em uma string
//Especialidade é do tipo enum, ou seja, um inteiro
char* ler_especialidade(int n) {
	char especialidade[20];
	//Recebe um numero da especialidade e devolve a string correta
	switch(n) {
	case 0:
		return strcpy(especialidade, "Clinico");
	case 1:
		return strcpy(especialidade, "Pediatra");
	case 2:
		return strcpy(especialidade, "Dermatologista");
	case 3:
		return strcpy(especialidade, "Cardiologista");
	case 4:
		return strcpy(especialidade, "Outro");
	}

}

//Ler a quantidade de medicos no inicio da aplicação, para conseguir transcrever todos em um vetor
//Converter todos medicos cadastrados no arquivo em um vetor
void read_medicos(VetMedicos *medicos) {
	FILE *file;

	file = fopen("medicos.txt","r");

	if(file == NULL) {
		//Caso não exista arquivo nenhum, cria um vetor dinamico de tamanho 10

		medicos->qtd = 0, medicos->cap = 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);
		//testa se há memaria para fazer a alocação
		if(medicos->itens == NULL) {
			printf("Erro de memoria\n");
			return;
		}

		//cria um novo arquivo
		file = fopen("medicos.txt","w");
		if(file == NULL) {
			printf("Erro ao abrir o arquivo\n");
		}
		fclose(file);
		return;

	} else {

		//Chama uma função geral que serve para contar quantas linhas tem no arquivo,
		//cria um vetor com a quantidade de linhas + 10.
		//Considere cada linha um objeto

		int n1, n2, n3, n4, i = 0;
		//Variaveis n vão ser usadas para pegar os numeros inteiros que serão usado para pegar os hararios
		medicos->qtd = contar_linhas(file);

		medicos->cap = medicos->qtd + 10;
		medicos->itens = (Medico *) malloc(sizeof(Medico) * medicos->cap);

		//testa a alocação
		if(medicos->itens == NULL) {
			printf("Erro ao realocar o vetor\n");
			fclose(file);
			return;
		}
		//Varrer por todos elementos no arquivo, pegando cada atributo e colocando
		//Usa | para dividir o elementos
		//O %63[^|] garante pegar uma string com 63 elementos, ela vai lendo até parar no |
		//Serve para pegar nomes compostos
		while(fscanf(file, "%d|%63[^|]|%d|%d|%d|%d|%d",&medicos->itens[i].id, medicos->itens[i].nome, &medicos->itens[i].especialidade, &n1,&n2,&n3,&n4) ==7) {
			//Adendo: strutc Especialidade é tipo enum, tem como representar como inteiro
			//Como o struct Medico tem 4 variaveis Horario
			//Chamo as funções desconverter_horas para já prencher cada espaço da variavel. Otimizando o processo
			desconverter_horas(n1,&medicos->itens[i].inicioManha);
			desconverter_horas(n2,&medicos->itens[i].fimManha);
			desconverter_horas(n3,&medicos->itens[i].inicioTarde);
			desconverter_horas(n4,&medicos->itens[i].fimTarde);
			i++;
		}
		//Por garantia pega agora o numero de fato que existe em medicos
		medicos->qtd = i;
		fclose(file);
		return;
	}
}
//Para quando necessario mostrar os medicos
void mostrar_medico(Medico medico) {
	printf("Medico: %s      id: %d      Especildiade: %s\n", medico.nome,medico.id, ler_especialidade(medico.especialidade));
	printf("Horario de manhâ das %d:%d até %d:%d\n",medico.inicioManha.horas, medico.inicioManha.minutos, medico.fimManha.horas, medico.fimManha.minutos);
	printf("Horario de tarde das %d:%d até %d:%d\n",medico.inicioTarde.horas, medico.inicioTarde.minutos, medico.fimTarde.horas, medico.fimTarde.minutos);

}
void add_medico(VetMedicos *medicos) {
	int maior = 0, choise1;
	char choise;
	//medico é usado como uma variavel de espaço temporario, só quando é confirmado adição que os dados são guardados
	Medico medico;
	
	//testa se é necessario aumentar o vetor de medicos
	choise1 = realocar_medicos(medicos);
	
	if(choise1==0) {
		printf("Não há mais memória para armazenar novos medicos. Encerrando função adicionar.\n");
		return;
	}
    
    //Percorre todo vetor de medicos, no final pega o maior id existente e adiciana +1. Caso não haja nenhum medico, o novo recebe 1
	if(medicos->qtd > 0) {
		for(int i=0; i<medicos->qtd; i++) {
			if(medicos->itens[i].id>=maior) {
				maior = medicos->itens[i].id;
			}
		}
	}
	medico.id = maior+1;

	printf("Digite o nome do medico: ");
	getchar();
	fgets(medico.nome,64, stdin);
	medico.nome[strcspn(medico.nome, "\n")] = '\0';
    //Recebe a especialidade do medico e converte para ficar da maneira correta
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
    
	printf("    Preencha o horário da consulta do inicio da manhã\n");
	receber_hora(&medico.inicioManha);
	printf("    Preencha o horário de consulta do fim da manhã\n");
	receber_hora(&medico.fimManha);
	printf("    Preencha o horário de consulta do inicio da tarde\n");
	receber_hora(&medico.inicioTarde);
	printf("    Preencha o horário de consulta do fim da tarde\n");
	receber_hora(&medico.fimTarde);

	printf("        Deseja adicionar\n");
	mostrar_medico(medico);
	printf("Digite y para confirmar ou n para cancelar\n");
	scanf(" %c", &choise);

    while(1){
        printf("Digite y para confirmar ou n para cancelar\n");
	    scanf(" %c", &choise);
    	if(choise == 'n')return;
    	if(choise == 'y')break;
    }
    //Cria agora o arquivo, caso fosse no inicio e o usuario não criasse o medico, o arquivo não seria fechado. Assim podendo corromper o arquivo
	FILE *file;

	file = fopen("medicos.txt", "a");
	if(file==NULL) {
		printf("Erro de memoria\n");
		return;
	}
	printf("Adicionando...\n\n");
	fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medico.id, medico.nome, choise1,con_horas(medico.inicioManha),con_horas(medico.fimManha),con_horas(medico.inicioTarde),con_horas(medico.fimTarde));

	fclose(file);
    
    //Como medicos->itens é do tipo Medico, ele consegue receber diretamente a variavel medico
	medicos->itens[medicos->qtd] = medico;

	medicos->qtd++;
}

void pesquisar_medicos(VetMedicos *medicos){
    int numero, i;
    
    printf("Digite o id do medico desejado\n");
    scanf("%d", &numero);
    //Percorre todo vetor de medicos até encontrar o medico com id escolhido pelo usuario.
    for(i = 0; i<medicos->qtd && numero!=medicos->itens[i].id; i++);
    //Testa se encontrou o medico
    if(numero == medicos->itens[i].id && numero>0){
        printf("Medico encontrado\n");
        mostrar_medico(medicos->itens[i]);
    }else{
        printf("Medico não encontrado no sistema\n");
    }
}