#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>

/**
 * @file mylib.h
 * @brief Core data structures and function declarations for the Medical Appointment Scheduling System.
 *
 * This header defines domain entities (Doctor, Patient, Appointment), dynamic array containers
 * (vectors) with capacity tracking, schedule representations, and prototypes for CRUD operations,
 * input validation, and file persistence.
 */

/**
 * @brief Represents a calendar date.
 */
typedef struct {
	int dia; /**< Day of the month (1-30). */
	int mes; /**< Month of the year (1-12). */
	int ano; /**< Year (e.g., 2026). */
} Data;

/**
 * @brief Represents a specific time of day using a 24-hour clock.
 */
typedef struct {
	int horas;   /**< Hour component (0-23). */
	int minutos; /**< Minute component (0-59). */
} Horario;

/**
 * @brief Medical specialties available in the clinic.
 */
typedef enum {
	ESPEC_CLINICO, /**< General Practitioner (Clínico Geral). */
	ESPEC_PEDIATRA,/**< Pediatrician. */
	ESPEC_DERMATO, /**< Dermatologist. */
	ESPEC_CARDIO,  /**< Cardiologist. */
	ESPEC_OUTRA    /**< Other specialty. */
} Especialidade;

/**
 * @brief Represents a doctor (physician) record in the system.
 */
typedef struct {
	int id;                     /**< Unique identifier for the doctor. */
	char nome[64];              /**< Doctor's full name (maximum 63 characters + null terminator). */
	Especialidade especialidade;/**< Medical specialty code. */
	// Typical shift service windows (e.g., 08:00-11:00 / 14:00-17:00; fixed 3-hour duration)
	Horario inicioManha, fimManha; /**< Morning shift operating window start and end times. */
	Horario inicioTarde, fimTarde; /**< Afternoon shift operating window start and end times. */
} Medico;

/**
 * @brief Represents a patient record in the system.
 */
typedef struct {
	int id;          /**< Unique identifier for the patient. */
	char nome[64];   /**< Patient's full name (maximum 63 characters + null terminator). */
	char contato[64];/**< Contact information (phone or email address). */
} Paciente;

/**
 * @brief Lifecycle status values for an appointment.
 */
typedef enum {
	CONS_AGENDADA, /**< Appointment is scheduled and confirmed. */
	CONS_CONCLUIDA,/**< Appointment was completed. */
	CONS_CANCELADA,/**< Appointment was canceled. */
	CONS_FALTA     /**< Patient missed the appointment (no-show). */
} StatusConsulta;

/**
 * @brief Represents an individual appointment slot connecting a doctor and a patient.
 */
typedef struct {
	int id;               /**< Unique identifier for the appointment. */
	int idMedico;         /**< Foreign key linking to the associated doctor's ID. */
	int idPaciente;       /**< Foreign key linking to the associated patient's ID. */
	Data data;            /**< Calendar date of the consultation. */
	Horario inicio;       /**< Consultation start time. */
	Horario fim;          /**< Consultation end time (automatically set to start + 1 hour). */
	StatusConsulta status;/**< Current lifecycle status of the appointment. */
	int prioridade;       /**< Priority level: 0 = Normal, 1 = Urgent (reserved for triage queue). */
} Consulta;

/**
 * @brief Dynamic array (vector) for storing and managing Doctor entities.
 */
typedef struct {
	Medico *itens; /**< Heap-allocated array of Doctor records. */
	int qtd;       /**< Current number of elements stored. */
	int cap;       /**< Current allocated capacity of the array. */
} VetMedicos;

/**
 * @brief Dynamic array (vector) for storing and managing Patient entities.
 */
typedef struct {
	Paciente *itens; /**< Heap-allocated array of Patient records. */
	int qtd;         /**< Current number of elements stored. */
	int cap;         /**< Current allocated capacity of the array. */
} VetPacientes;

/**
 * @brief Dynamic array (vector) for storing and managing Appointment entities.
 */
typedef struct {
	Consulta *itens; /**< Heap-allocated array of Appointment records. */
	int qtd;         /**< Current number of elements stored. */
	int cap;         /**< Current allocated capacity of the array. */
} VetConsultas;

/**
 * @brief Configuration rules for scheduling policies (optional extensible feature).
 */
typedef struct {
	int duracaoPadraoMin;         /**< Default consultation duration in minutes (e.g., 30). */
	int permiteSobreposicao;       /**< Overlap flag: 0 = forbidden, 1 = allowed (not recommended). */
	int prioridadeUrgentePrimeiro;/**< Urgent priority flag: 1 = schedule urgent requests in first slot. */
} PoliticaAgendamento;

// General utility function prototypes

/**
 * @brief Prompts user for hour and minute inputs via stdin and validates ranges.
 * @param[out] horario Pointer to Horario structure to receive validated time.
 * @note Continuously prompts until hour is in [0, 23] and minute is in [0, 59].
 */
void receber_hora(Horario *horario);

/**
 * @brief Packs a Horario structure into a single integer representation (HHMM).
 * @param[in] horario Time structure to convert.
 * @return Packed integer calculated as (horas * 100) + minutos.
 * @note Useful for simplified time comparisons and compact flat-file persistence.
 */
int con_horas(Horario horario);

/**
 * @brief Unpacks an integer (HHMM) into a Horario structure.
 * @param[in] n Packed time integer in HHMM format.
 * @param[out] horario Pointer to Horario structure to receive unpacked hours and minutes.
 */
void desconverter_horas(int n, Horario *horario);

/**
 * @brief Counts the number of newline-delimited lines in an open text file stream.
 * @param[in] arquivo Open file pointer to inspect.
 * @return Number of lines in the file, or -1 if the file pointer is NULL.
 * @note Uses rewind() to restore file position back to the beginning upon return.
 */
int contar_linhas(FILE *arquivo);

/**
 * @brief Validates start time boundaries and automatically computes end time based on shift/slot rules.
 * @param[in,out] inicio Pointer to requested start time; re-prompted if outside operational limits.
 * @param[out] fim Pointer to structure where computed end time will be stored.
 * @param[in] n Mode selector: 1 = Doctor shift validation, 2 = Appointment slot validation.
 * @param[in] turno Shift selector for doctor mode: 1 = Morning shift, 2 = Afternoon shift.
 * @note Morning shifts must start between 07:00 and 09:00 (end = start + 3h, max 12:00).
 *       Afternoon shifts must start between 13:00 and 17:00 (end = start + 3h, max 20:00).
 *       Appointments must start within 07:00-11:00 or 13:00-19:00 (end = start + 1h).
 */
void veri_horario(Horario *inicio, Horario *fim, int n, int turno);

/**
 * @brief Validates that a string is a valid person name containing no numeric digits.
 * @param[in] str Null-terminated string to validate.
 * @return 1 if valid, 0 if string is empty or contains numeric digits.
 */
int verify_name(const char *str);

/**
 * @brief Validates basic email formatting for patient contact entries.
 * @param[in] str Null-terminated string representing the email address.
 * @return 1 if the email format satisfies validation rules, 0 otherwise.
 * @note Requires exactly one '@', at least one '.' after '@', and prohibits consecutive dots or leading/trailing symbols.
 */
int verify_email(const char *str);

// Patient management function prototypes

/**
 * @brief Interactively collects patient data, saves to "pacientes.txt", and adds to in-memory vector.
 * @param[in,out] pacientes Pointer to dynamic patient vector.
 * @note Reallocates vector capacity if full. Generates auto-incrementing ID.
 */
void add_paciente(VetPacientes *pacientes);

/**
 * @brief Searches for a patient by unique ID via stdin prompt.
 * @param[in] pacientes Pointer to patient vector to search.
 * @return Vector index of the patient if found; -1 if not found; -2 if vector is empty.
 */
int search_paciente(VetPacientes *pacientes);

/**
 * @brief Loads all patient records from "pacientes.txt" into memory.
 * @param[out] pacientes Pointer to patient vector to initialize and populate.
 * @note Allocates heap memory for items using malloc. Creates an empty file if none exists.
 */
void read_pacientes(VetPacientes *pacientes);

/**
 * @brief Reallocates the patient array capacity by +10 if capacity is reached.
 * @param[in,out] pacientes Pointer to patient vector.
 * @return 1 on successful reallocation or if not needed; 0 if realloc fails.
 * @note Reallocates memory using realloc().
 */
int realocar_pacientes(VetPacientes *pacientes);

/**
 * @brief Deletes a patient by ID, updates "pacientes.txt", and cascades deletion to linked appointments.
 * @param[in,out] pacientes Pointer to patient vector.
 * @param[in,out] consultas Pointer to appointment vector for cascade deletion.
 * @note Shifts remaining elements leftward. Rewrites "pacientes.txt" and "consultas.txt".
 */
void remover_paciente(VetPacientes *pacientes, VetConsultas *consultas);

/**
 * @brief Prints all registered patients to standard output.
 * @param[in] pacientes Pointer to patient vector.
 */
void list_pacientes(VetPacientes *pacientes);

/**
 * @brief Interactively updates an existing patient's details and rewrites "pacientes.txt".
 * @param[in,out] pacientes Pointer to patient vector.
 */
void update_pacientes(VetPacientes *pacientes);

// Doctor management function prototypes

/**
 * @brief Loads all doctor records from "medicos.txt" into memory.
 * @param[out] medicos Pointer to doctor vector to initialize and populate.
 * @note Allocates heap memory for items using malloc. Creates an empty file if none exists.
 */
void read_medicos(VetMedicos *medicos);

/**
 * @brief Reallocates the doctor array capacity by +10 if capacity is reached.
 * @param[in,out] medicos Pointer to doctor vector.
 * @return 1 on successful reallocation or if not needed; 0 if realloc fails.
 * @note Uses temporary pointer to safely guard against realloc failure.
 */
int realocar_medicos(VetMedicos *medicos);

/**
 * @brief Interactively collects doctor data, saves to "medicos.txt", and adds to in-memory vector.
 * @param[in,out] medicos Pointer to dynamic doctor vector.
 * @note Reallocates vector capacity if full. Generates auto-incrementing ID.
 */
void add_medico(VetMedicos *medicos);

/**
 * @brief Displays formatted details of a single doctor to stdout.
 * @param[in] medico Doctor record to display.
 */
void mostrar_medico(Medico medico);

/**
 * @brief Searches for a doctor by unique ID via stdin prompt.
 * @param[in] medicos Pointer to doctor vector to search.
 * @return Vector index of the doctor if found; -1 if not found; -2 if vector is empty.
 */
int pesquisar_medicos(VetMedicos *medicos);

/**
 * @brief Deletes a doctor by ID, updates "medicos.txt", and cascades deletion to linked appointments.
 * @param[in,out] medicos Pointer to doctor vector.
 * @param[in,out] consultas Pointer to appointment vector for cascade deletion.
 * @note Shifts remaining elements leftward. Rewrites "medicos.txt" and "consultas.txt".
 */
void remover_medico(VetMedicos *medicos, VetConsultas *consultas);

/**
 * @brief Displays doctor records, optionally filtering by medical specialty.
 * @param[in] medicos Pointer to doctor vector.
 */
void listar_medicos(VetMedicos *medicos);

/**
 * @brief Interactively updates an existing doctor's details and rewrites "medicos.txt".
 * @param[in,out] medicos Pointer to doctor vector.
 */
void update_medicos(VetMedicos *medicos);

// Appointment management function prototypes

/**
 * @brief Interactively schedules a new appointment after schedule conflict validation.
 * @param[in,out] consultas Pointer to appointment vector.
 * @param[in,out] pacientes Pointer to patient vector for selecting patient.
 * @param[in,out] medicos Pointer to doctor vector for selecting doctor.
 * @note Checks for slot overlaps and shift availability. Appends record to "consultas.txt".
 */
void add_consulta(VetConsultas *consultas, VetPacientes *pacientes, VetMedicos *medicos);

/**
 * @brief Packs a Data structure into a single integer formatted as YYYYMMDD.
 * @param[in] data Date structure to convert.
 * @return Packed integer calculated as (ano * 10000) + (mes * 100) + dia.
 */
int con_data(Data data);

/**
 * @brief Interactively prompts user for date components and validates basic ranges.
 * @param[out] data Pointer to Data structure to receive validated date.
 * @note Validates dia in [1, 30], mes in [1, 12], and ano >= 2026.
 */
void add_data(Data *data);

/**
 * @brief Unpacks an integer in YYYYMMDD format into a Data structure.
 * @param[out] data Pointer to Data structure to receive unpacked date.
 * @param[in] n Packed date integer in YYYYMMDD format.
 */
void descon_data(Data *data, int n);

/**
 * @brief Displays appointment records, optionally filtered by status.
 * @param[in] consultas Pointer to appointment vector.
 */
void list_consultas(VetConsultas *consultas);

/**
 * @brief Loads all appointment records from "consultas.txt" into memory.
 * @param[out] consultas Pointer to appointment vector to initialize and populate.
 * @note Allocates heap memory for items using malloc. Creates an empty file if none exists.
 */
void read_consultas(VetConsultas *consultas);

/**
 * @brief Reallocates the appointment array capacity by +10 if capacity is reached.
 * @param[in,out] consultas Pointer to appointment vector.
 * @return 1 on successful reallocation or if not needed; 0 if realloc fails.
 * @note Reallocates memory using realloc().
 */
int realocar_consultas(VetConsultas *consultas);

/**
 * @brief Maps an appointment status enum integer to its symbolic string representation.
 * @param[in] n Status integer code (0-3).
 * @return Constant string literal representing the status name, or "Status invalido".
 */
const char* ler_status(int n);

/**
 * @brief Displays formatted details of a single appointment to stdout.
 * @param[in] consultas Pointer to appointment record to display.
 */
void printar_consultas(Consulta *consultas);

/**
 * @brief Searches for an appointment by unique ID via stdin prompt.
 * @param[in] consultas Pointer to appointment vector to search.
 * @return Vector index of the appointment if found; -1 if not found or vector is empty.
 */
int search_consultas(VetConsultas *consultas);

/**
 * @brief Interactively removes an appointment by ID and updates "consultas.txt".
 * @param[in,out] consultas Pointer to appointment vector.
 * @note Shifts remaining elements leftward and rewrites "consultas.txt".
 */
void del_consulta(VetConsultas *consultas);

/**
 * @brief Interactively updates an appointment's status and rewrites "consultas.txt".
 * @param[in,out] consultas Pointer to appointment vector.
 */
void update_status(VetConsultas *consultas);;

/**
 * @brief Validates that a proposed appointment time does not conflict with doctor shifts or existing bookings.
 * @param[in] medicos Pointer to doctor vector.
 * @param[in] consultas Pointer to appointment vector.
 * @param[in] data Requested appointment date.
 * @param[in] inicio Requested appointment start time.
 * @param[in] id_medico Index of the doctor in the medicos vector.
 * @return 1 if slot is available and within shifts; 0 if conflicting or out of shift bounds.
 */
int verify_consulta(VetMedicos *medicos, VetConsultas *consultas, Data data, Horario inicio, int id_medico);

/**
 * @brief Programmatically deletes an appointment by array index without user prompt.
 * @param[in,out] consultas Pointer to appointment vector.
 * @param[in] id Array index of appointment to remove.
 * @note Shifts remaining elements leftward and rewrites "consultas.txt". Used in cascade deletion.
 */
void auto_del_consulta(VetConsultas *consultas, int id);

#endif