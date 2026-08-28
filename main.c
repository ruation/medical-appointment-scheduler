#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main()
{
	int op, op1;

	// Inicializa o VetPacientes com valores iniciais
	VetPacientes pacientes;
	
	//Inicia o VetMedicos
	VetMedicos medicos;
	read_medicos(&medicos);
	pacientes.qtd = 0;
	pacientes.cap = 10;
	pacientes.itens = (Paciente *) malloc(sizeof(Paciente) * pacientes.cap);

	do {
		printf("---- menu ----\n1 - pacientes\n2 - medicos\n9 - sair\n");
		scanf("%d", &op);
		switch(op) {
		case 1:
			read_pacientes(&pacientes); //this is inside the clientes menu, maybe replace it in the main menu.
			do {
				printf("---- menu pacientes ----\n1 - adicionar\n2 - procurar paciente\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1) {
				case 1:
					add_paciente(&pacientes);
					break;
				case 2:
					search_paciente(&pacientes);
					break;
				default:
					break;
				}
			} while(op1!=9);
		break;
		case 2:
			do {
				printf("---- menu medicos ----\n1 - adicionar\n2 - pesquisar medicos\n3 - remover medico\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1) {
				case 1:
					add_medico(&medicos);
					break;
				case 2:
				    pesquisar_medicos(&medicos);
				    break;
				case 3:
				    remover_medico(&medicos);
				default:
					break;
				}
			} while(op1!=9);
			break;
		default:
			break;
		}
	} while(op!=9);
	free(pacientes.itens);
	free(medicos.itens);
}
