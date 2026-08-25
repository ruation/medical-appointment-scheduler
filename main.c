#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main()
{
	int op;

	// Inicializa o VetPacientes com valores iniciais
	VetPacientes pacientes;
	VetMedicos medicos;
	read_medicos(&medicos);
	pacientes.qtd = 0;
	pacientes.cap = 10;
	pacientes.itens = (Paciente *) malloc(sizeof(Paciente) * pacientes.cap);

	do {
		printf("---- menu ----\n1 - pacientes\n9 - sair\n");
		scanf("%d", &op);
		switch(op) {
		case 1:
			int op1;
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

		default:
			break;
		}
	} while(op!=9);
	free(pacientes.itens);
	free(medicos.itens);
}
