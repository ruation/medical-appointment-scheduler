#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

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
const char* ler_especialidade(int n) {
	//Recebe um numero da especialidade e devolve a string correta
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
    do{
        printf("Digite o nome do medico: ");
    	getchar();
    	fgets(medico.nome,64, stdin);
    	medico.nome[strcspn(medico.nome, "\n")] = '\0';
    }while(verify_name(medico.nome)!= 1);
	
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
	//Cria agora o arquivo, caso fosse no inicio e o usuario não criasse o medico, o arquivo não seria fechado. Assim podendo corromper o arquivo
	FILE *file;

	file = fopen("medicos.txt", "a");
	if(file==NULL) {
		printf("Erro de memoria\n");
		return;
	}
	printf("Adicionando...\n\n");
	fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medico.id, medico.nome, medico.especialidade,con_horas(medico.inicioManha),con_horas(medico.fimManha),con_horas(medico.inicioTarde),con_horas(medico.fimTarde));

	fclose(file);

	//Como medicos->itens é do tipo Medico, ele consegue receber diretamente a variavel medico
	medicos->itens[medicos->qtd] = medico;

	medicos->qtd++;
}

int pesquisar_medicos(VetMedicos *medicos) {
	int numero, i;
	Medico medico1;
	
	if(medicos->qtd < 1){printf("Não existe medicos cadastrados.\n");return -2;}
	
	printf("Digite o id do medico desejado\n");
	scanf("%d", &numero);
	//Percorre todo vetor de medicos até encontrar o medico com id escolhido pelo usuario.

	for(i = 0; i < medicos->qtd && numero!=medicos->itens[i].id; i++);
	//Testa se encontrou o medico
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
void remover_medico(VetMedicos *medicos) {
	if(medicos->qtd == 0){
	    printf("Sem medicos cadastradas no sistema\n");
	    return;
	}
	int i, id;
	char choise;
	id = pesquisar_medicos(medicos);
	//Aproveita a função anterior para já pegar o id do medico selecionado para ser deletado
	if(id==-1)return;
	printf("Deseja remover esse medico?\n");

	while(1) {
		printf("Digite y para confirmar ou n para cancelar\n");
		scanf(" %c", &choise);
		if(choise == 'n')return;
		if(choise == 'y')break;
	}

	FILE *file;
	//Remove o medico
	//Basicamente joga todos elemetos a direita dele para esquerda. Sobreescrevendo o elemento apagado
	for(i = id; i < medicos->qtd-1 ; i++) {
		medicos->itens[i] = medicos->itens[i + 1];
	}
	medicos->qtd--;

	file = fopen("medicos.txt", "w");
	//Limpa todo o arquivo e em seguida preenche novamente com os dados restantes do vetor
	for(i = 0; i<medicos->qtd; i++) {
		fprintf(file, "%d|%s|%d|%d|%d|%d|%d\n",medicos->itens[i].id, medicos->itens[i].nome, medicos->itens[i].especialidade,con_horas(medicos->itens[i].inicioManha),con_horas(medicos->itens[i].fimManha),con_horas(medicos->itens[i].inicioTarde),con_horas(medicos->itens[i].fimTarde));
	}

	fclose(file);
	printf("Removendo medico...\n");
}
//Lista medicos filtrando por especialidade e mostra a quantidade de medicos cadastrados
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
    		//Pede a especialidade do medico e depois converte para a função ler_especialidade retornar um valor valido
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
    		//Caso não exista um medico com essa especialidade, retorna uma mensagem
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
