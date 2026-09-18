#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

/**
 * @brief Validates that a string is a non-empty person name containing no numeric digits.
 * @param[in] str Null-terminated string representing the name to validate.
 * @return 1 if the string is valid, 0 if it is empty or contains numeric digits.
 * @note Displays an error message to stdout if numeric digits are found.
 */
int verify_name(const char *str){
    if (*str == '\0') return 0; // Return 0 if the string is empty
    for(int i = 0; str[i] != '\0'; i++){
        if(isdigit(str[i])){printf("O nome não pode conter números.\n"); return 0;}
    }
    return 1; // Return 1 if the string is valid
}

/**
 * @brief Interactively prompts the user via stdin to enter and validate hours and minutes.
 * @param[out] horario Pointer to Horario structure where validated hours and minutes are written.
 * @note Continuously prompts until the entered hour is in [0, 23] and minutes are in [0, 59].
 */
void receber_hora(Horario *horario) {
	int hora, minutos;
	// Loop to ensure the entered hour value is valid (0-23)
	while(1) {
		printf("Digite a hora\n");
		scanf("%d",&hora);
		if(24>hora && hora>=0) {
			break;
		} else {
			printf("Horario invalido, digite uma hora existente\n");
		}
	}
	// Loop to ensure the entered minute value is valid (0-59)
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
	printf("Horario escolhido: %d:%d",horario->horas,horario->minutos);
}

/**
 * @brief Validates time boundary rules for shifts and appointments, auto-calculating the end time.
 * @param[in,out] inicio Pointer to start time. If invalid, repeatedly re-prompts the user until valid.
 * @param[out] fim Pointer to end time structure, computed based on shift/appointment duration rules.
 * @param[in] n Mode selector: 1 for doctor work shift, 2 for appointment slot.
 * @param[in] turno Shift selector for doctor mode (1 = morning, 2 = afternoon). Ignored in mode 2.
 * @note For doctors (n=1), morning shift must start between 07:00 and 09:00 (duration = 3h, max 12:00),
 *       and afternoon shift must start between 13:00 and 17:00 (duration = 3h, max 20:00).
 *       For appointments (n=2), appointments must start between 07:00-11:00 or 13:00-19:00 (duration = 1h).
 */
void veri_horario(Horario *inicio, Horario *fim, int n, int turno) {
	// Mode selector: n=1 validates doctor shifts, n=2 validates appointment slots. Default handles invalid mode.
	while(1) {
			switch(n) {
			case 1:
			    switch(turno){
			        case 1:
			            while(inicio->horas > 9 || (inicio->horas == 9 && inicio->minutos > 0) || inicio->horas < 7){
			                printf(" fim: %d:%d\n", inicio->horas+3, inicio->minutos);
			                printf("O expediente da manhã precisa ser entre 7h e 12h. ");
			                receber_hora(inicio);
			            }
        		       
        		       	fim->horas = inicio->horas + 3; // Shift ends exactly 3 hours after start time
        			    fim->minutos = inicio->minutos;
        			    
        			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
        			    return;
		            case 2:
		                while(inicio->horas <13 || inicio->horas > 17 || (inicio->horas == 17 && inicio->minutos > 0)){
		                    printf(" fim: %d:%d\n", inicio->horas+3, inicio->minutos);
		                    printf("O expediente da tarde precisa ser entre 13h e 20h. ");
		                    receber_hora(inicio);
		                }
        		       	fim->horas = inicio->horas + 3; // Shift ends exactly 3 hours after start time
        			    fim->minutos = inicio->minutos;
        			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
		                return;
			    }

			case 2:
			    while((inicio->horas == 19 && inicio->minutos > 0) || inicio->horas > 19 || inicio->horas<7 || (inicio->horas == 11 && inicio->minutos > 0) || inicio->horas == 12){
			         printf(" fim: %d:%d\n", inicio->horas+1, inicio->minutos);
			         printf("A consulta precisa ser entre 7h e 12h ou 13h e 20h ");
			         receber_hora(inicio);
			    }
			    fim->horas = inicio->horas + 1; // Appointment duration is fixed to 1 hour
			    fim->minutos = inicio->minutos;
			    printf(" fim: %d:%d\n", fim->horas, fim->minutos);
                return;
			default:
				printf("Erro\n");
				return;
			}
		}
}

/**
 * @brief Packs a Horario structure into a single integer representation (HHMM).
 * @param[in] horario Horario struct containing hours and minutes.
 * @return Integer packed as (horas * 100) + minutos.
 * @note Compresses two separate time fields into a single integer for simplified comparisons and flat-file serialization.
 */
int con_horas(Horario horario) {

	return (horario.horas * 100) + horario.minutos;
}

/**
 * @brief Unpacks an integer in HHMM format into hours and minutes in a Horario structure.
 * @param[in] n Packed time integer in HHMM format.
 * @param[out] horario Pointer to Horario structure where unpacked values will be stored.
 * @note Computes hours via integer division (n / 100) and minutes via remainder (n % 100).
 */
void desconverter_horas(int n, Horario *horario) {
	horario->horas = n/100;
	horario->minutos = n%100;
}

/**
 * @brief Counts the total number of lines in an open text file stream.
 * @param[in] arquivo Open file pointer to count lines from.
 * @return Total number of lines, or -1 if the file pointer is NULL.
 * @note Uses rewind() to reset the stream position back to the beginning of the file.
 */
int contar_linhas(FILE *arquivo) {
	// Verify that the file pointer is valid
	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo!\n");
		return -1;
	}

	int linhas = 0;
	char buffer[1024]; // Temporary line read buffer

	// Read line by line until reaching end-of-file
	while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
		linhas++;
	}
	// After reaching EOF, rewind the stream position to the beginning of the file
	rewind(arquivo);
	return linhas;
}