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
	
	//Inicializa o VetConsultas com valores iniciais
	VetConsultas consultas;
	read_consultas(&consultas);


	do {
		printf("---- menu ----\n1 - pacientes\n2 - medicos\n3 - consultas\n9 - sair\n");
		scanf("%d", &op);
		switch(op) {
		case 1:
			do {
				printf("---- menu pacientes ----\n1 - adicionar\n2 - procurar paciente\n3 - remover paciente\n4 - listar pacientes\n5 - atualizar paciente\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1) {
				case 1:
					add_paciente(&pacientes);
					break;
				case 2:
					search_paciente(&pacientes);
					break;
				case 3:
				    remover_paciente(&pacientes, &consultas);
				    break;
				case 4:
				    list_pacientes(&pacientes);
				    break;
				case 5:
				    update_pacientes(&pacientes);
				default:
					break;
				}
			} while(op1!=9);
		break;
		case 2:
			do {
				printf("---- menu medicos ----\n1 - adicionar\n2 - pesquisar medicos\n3 - remover medico\n4 - listar medicos\n5 - atualizar medico\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1) {
				case 1:
					add_medico(&medicos);
					break;
				case 2:
				    pesquisar_medicos(&medicos);
				    break;
				case 3:
				    remover_medico(&medicos, &consultas);
				    break;
				case 4:
				    listar_medicos(&medicos);
				    break;
				case 5:
				    update_medicos(&medicos);
				    break;
				default:
					break;
				}
			} while(op1!=9);
			break;
		case 3:
			do {
				printf("---- menu consultas ----\n1 - adicionar\n2 - editar consulta\n3 - listar consultas\n4 - pesquisar consulta\n5 - deletar consulta\n9 - voltar\n");
				scanf("%d", &op1);
				switch(op1){
				    case 1:
				        add_consulta(&consultas, &pacientes, &medicos);
				        break;
				    case 2:
				        update_status(&consultas);
				        break;
				    case 3:
				        list_consultas(&consultas);
				        break;
				    case 4:
				        search_consultas(&consultas);
				        break;
				    case 5:
				        del_consulta(&consultas);
				    default:
				        break;
				}
			}while(op1!=9);
			break;
		default:
			break;
		}
	} while(op!=9);
	
	free(pacientes.itens);
	free(medicos.itens);
	free(consultas.itens);
}