typedef struct {
	int dia, mes, ano;
} Data;

typedef struct {
	int horas, minutos;
} Horario;

typedef enum {
	ESPEC_CLINICO, ESPEC_PEDIATRA, ESPEC_DERMATO, ESPEC_CARDIO,
	ESPEC_OUTRA
} Especialidade;

typedef struct {
	int id;
	char nome[64];
	Especialidade especialidade;
	// Janela típica de atendimento (ex.: 08:00–12:00 / 14:00–18:00)
	Horario inicioManha, fimManha;
	Horario inicioTarde, fimTarde;
} Medico;

typedef struct {
	int id;
	char nome[64];
	char contato[64]; // telefone/email

} Paciente;

typedef enum {
	CONS_AGENDADA, CONS_CONCLUIDA, CONS_CANCELADA,
	CONS_FALTA
} StatusConsulta;

// Slot de 1 consulta
typedef struct {
	int id;
	int idMedico;
	int idPaciente;
	Data data;
	Horario inicio;
	Horario fim;
	StatusConsulta status;
	int prioridade; // 0 normal; 1 urgente (para “inteligente”/fila)
} Consulta;


typedef struct {
	Medico *itens;
	int qtd, cap;
} VetMedicos;

typedef struct {
	Paciente *itens;
	int qtd, cap;
} VetPacientes;

typedef struct {
	Consulta *itens;
	int qtd, cap;
} VetConsultas;

// (Opcional) Regras de agendamento inteligente
typedef struct {
	int duracaoPadraoMin; // ex.: 30 minutos
	int permiteSobreposicao; // 0 não, 1 sim (nunca recomendado)
	int prioridadeUrgentePrimeiro;// 1: sempre tentar marcar urgente no 1o slot livre
} PoliticaAgendamento;

//Prototipos de funções gerais
void receber_hora(Horario *horario);
int con_horas(Horario horario);
void desconverter_horas(int n, Horario *horario);
int contar_linhas(FILE *arquivo);
void veri_horario(Horario *inicio, Horario *fim, int n);
int verify_name(const char *str);
int verify_email(const char *str);

//Prototipos de funções de pacientes
void add_paciente(VetPacientes *pacientes);
int search_paciente(VetPacientes *pacientes);
void read_pacientes(VetPacientes *pacientes);
int realocar_pacientes(VetPacientes *pacientes);
void remover_paciente(VetPacientes *pacientes);
void list_pacientes(VetPacientes *pacientes);

//Prototipos de funções de medicos

void read_medicos(VetMedicos *medicos);
int realocar_medicos(VetMedicos *medicos);
void add_medico(VetMedicos *medicos);
void mostrar_medico(Medico medico);
int pesquisar_medicos(VetMedicos *medicos);
void remover_medico(VetMedicos *medicos);
void listar_medicos(VetMedicos *medicos);

//Prototipos de funções de consultas 

void add_consulta(VetConsultas *consultas, VetPacientes *pacientes, VetMedicos *medicos);
int con_data(Data data);
void add_data(Data *data);
void descon_data(Data *data, int n);