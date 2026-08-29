#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main()
{
	int op, op1;

	// Inicializa o VetPacientes com valores iniciais
	VetPacientes pacientes;
	read_pacientes(&pacientes);
	
	//Inicia o VetMedicos
	VetMedicos medicos;
	read_medicos(&medicos);



	do {
		printf("---- menu ----\n1 - pacientes\n2 - medicos\n9 - sair\n");
		scanf("%d", &op);
		switch(op) {
		case 1:
			do {
				printf("---- menu pacientes ----\n1 - adicionar\n2 - procurar paciente\n3 - remover paciente\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1) {
				case 1:
					add_paciente(&pacientes);
					break;
				case 2:
					search_paciente(&pacientes);
					break;
				case 3:
				    remover_paciente(&pacientes);
				    break;
				default:
					break;
				}
			} while(op1!=9);
		break;
		case 2:
			do {
				printf("---- menu medicos ----\n1 - adicionar\n2 - pesquisar medicos\n3 - remover medico\n4 - listar medicos\n9 - voltar\n");
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
				    break;
				case 4:
				    listar_medicos(&medicos);
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
