#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

/**
 * @file main.c
 * @brief Application entry point and interactive CLI menu driver for the Medical Appointment Scheduling System.
 *
 * Coordinates initialization of patient, doctor, and appointment data structures, presents
 * interactive command-line menus for CRUD operations, and guarantees proper resource release
 * upon exit.
 */

/**
 * @brief Main execution entry point and interactive menu controller.
 * @return 0 upon successful execution and clean exit.
 * @note Loads persisted data from flat files ("pacientes.txt", "medicos.txt", "consultas.txt")
 *       into dynamic arrays at startup, guides user interactions through nested switch-case menus,
 *       and releases all allocated heap memory buffers before exiting.
 */
int main()
{
	int op, op1;

	// Initialize dynamic vector and load persisted patient records
	VetPacientes pacientes;
	read_pacientes(&pacientes);
	
	// Initialize dynamic vector and load persisted doctor records
	VetMedicos medicos;
	read_medicos(&medicos);
	
	// Initialize dynamic vector and load persisted appointment records
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
	
	// Release dynamically allocated memory buffers
	free(pacientes.itens);
	free(medicos.itens);
	free(consultas.itens);
}