# Medical Appointment Scheduling System 🩺

A modular, high-performance command-line application written in C for managing healthcare clinic operations, patient admissions, physician registries, and medical appointment scheduling with schedule conflict detection, dynamic memory management, and file persistence.

---

## 🏛️ Academic Context & Background

This project was engineered as a comprehensive final practical assignment for the **Introduction to Programming** (*Introdução à Programação*) course at the **Universidade Federal do Agreste de Pernambuco (UFAPE)**, under the academic guidance of **Prof. René Gusmão**.

### Core Pedagogical Objectives
* **Manual Dynamic Memory Management**: Deep application of pointer arithmetic, heap allocation (`malloc`), runtime buffer expansion (`realloc`), and memory leak prevention (`free`).
* **Relational Flat-File Persistence**: Designing file serialization schemas and parsers using standard C stream I/O (`fopen`, `fscanf`, `fprintf`, `rewind`, `fclose`).
* **Modular Software Architecture**: Decoupling declarations, generic utility routines, domain entities, and CLI control flow across independent compilation units.
* **Defensive Programming**: Validating user input boundaries, parsing compound strings, ensuring scheduling consistency, and implementing cascading deletions to prevent orphaned records.

---

## 📐 Project Overview & Architecture

The system models a healthcare clinic environment centered around three primary domain entities:

```
+----------------+          1 : N          +--------------------+          N : 1          +------------------+
|   Paciente     |<------------------------|      Consulta      |------------------------>|      Medico      |
|   (Patient)    |                         |   (Appointment)    |                         |     (Doctor)     |
+----------------+                         +--------------------+                         +------------------+
| - id: int      |                         | - id: int          |                         | - id: int        |
| - nome: str    |                         | - idMedico: int    |                         | - nome: str      |
| - contato: str |                         | - idPaciente: int  |                         | - especialidade  |
+----------------+                         | - data: Data       |                         | - inicioManha    |
                                           | - inicio: Horario  |                         | - fimManha       |
                                           | - fim: Horario     |                         | - inicioTarde    |
                                           | - status: enum     |                         | - fimTarde       |
                                           | - prioridade: int  |                         +------------------+
                                           +--------------------+
```

### Domain Entities

1. **Doctor (`Medico`)**:
   * Represents an attending physician.
   * Attributes: Unique integer ID, full name (up to 63 characters), medical specialty (`Clinico`, `Pediatra`, `Dermatologista`, `Cardiologista`, `Outro`), and two daily operational shift windows (Morning and Afternoon, each standardized to 3 hours).
2. **Patient (`Paciente`)**:
   * Represents an individual receiving care.
   * Attributes: Unique integer ID, full name, and validated contact details (email or telephone).
3. **Appointment (`Consulta`)**:
   * Relational link between a Doctor and a Patient.
   * Attributes: Unique integer ID, foreign key `idMedico`, foreign key `idPaciente`, calendar date (`Data`), start time (`Horario`), computed end time (`Horario` = start + 1h), lifecycle status (`CONS_AGENDADA`, `CONS_CONCLUIDA`, `CONS_CANCELADA`, `CONS_FALTA`), and triage priority flag.

---

## ⚡ Core Technical Highlights

### 1. Chunk-Based Dynamic Memory Allocation
* In-memory arrays are maintained using vector wrapper structs (`VetPacientes`, `VetMedicos`, `VetConsultas`) that track current occupancy (`qtd`) and capacity (`cap`).
* Storage is initially allocated for baseline capacity (10 records or `existing_file_lines + 10`).
* When an array fills (`qtd == cap`), reallocation functions (`realocar_pacientes`, `realocar_medicos`, `realocar_consultas`) expand the buffer dynamically by **10 additional slots** using `realloc()`.
* Reallocation employs temporary pointers to prevent dangling references or memory leaks should allocation fail:
  ```c
  Medico *medicos1 = (Medico*) realloc(medicos->itens, (medicos->cap + 10) * sizeof(Medico));
  if (medicos1 != NULL) {
      medicos->itens = medicos1;
      medicos->cap += 10;
  }
  ```
* All heap memory is safely reclaimed upon exiting the main menu loop via `free()`.

### 2. Flat-File Persistence & Stream Serialization
* Data is persisted across sessions in plain-text flat files: `pacientes.txt`, `medicos.txt`, and `consultas.txt`.
* Record creation uses file append mode (`"a"`), writing single formatted entries directly to disk.
* Record updates and deletions perform in-place memory array modifications and atomically rewrite the entire flat file (`"w"`), ensuring the disk state remains identical to in-memory state.
* Startup loading functions (`read_pacientes`, `read_medicos`, `read_consultas`) automatically count file lines with `contar_linhas()`, allocate matching memory buffers, and deserialize fields using formatted scans (`fscanf`).

### 3. Cascading Deletions (Referential Integrity)
* To prevent orphaned appointment records, the system enforces cascading deletions:
  * When a **Patient** is removed via `remover_paciente()`, the system scans all appointments and automatically purges any appointment where `idPaciente` matches the removed ID using `auto_del_consulta()`.
  * When a **Doctor** is removed via `remover_medico()`, the system similarly sweeps and deletes all appointments where `idMedico` matches the removed ID.
* Remaining vector elements are shifted leftward (`itens[i] = itens[i+1]`), and indices are adjusted dynamically during iteration.

### 4. Schedule Verification & Conflict Detection
* The `verify_consulta()` function validates potential appointments against:
  1. **Shift Windows**: Ensures the appointment start and end times fall strictly within the doctor's active morning (`07:00–12:00`) or afternoon (`13:00–20:00`) shift.
  2. **Double-Booking Prevention**: Checks for overlaps with any existing scheduled appointment (`CONS_AGENDADA`) for that doctor on the same calendar date within a 1-hour window.

### 5. Robust Input Validation & Sanitization
* **Name Validation (`verify_name`)**: Enforces non-empty strings and checks character-by-character to reject numeric digits.
* **Email Validation (`verify_email`)**: Validates presence of exactly one `@`, requires at least one `.` following the `@`, forbids consecutive dots, and rejects leading/trailing punctuation symbols.
* **Time Serialization (`con_horas` / `desconverter_horas`)**: Compresses 24-hour time structures into compact integers (`(horas * 100) + minutos`) for simplified comparisons and clean disk formatting.
* **Date Serialization (`con_data` / `descon_data`)**: Packs calendar dates into `YYYYMMDD` integer values (`(ano * 10000) + (mes * 100) + dia`).

---

## 🛠️ Compilation & Run Instructions

### Prerequisites
* A standard C compiler supporting C99 or later (e.g., `gcc` or `clang`).
* Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`).
* Linux, macOS, or Windows (via WSL, MSYS2, or MinGW).

### Build Command
Compile all modular source files into a single standalone executable using `gcc`:

```bash
gcc main.c medico.c paciente.c consulta.c mylib.c -o clinic_system
```

For strict compiler warnings and optimization flags:

```bash
gcc -Wall -Wextra -std=c99 -O2 main.c medico.c paciente.c consulta.c mylib.c -o clinic_system
```

### Execution
Run the compiled binary directly from your terminal:

```bash
./clinic_system
```

On Windows (Command Prompt / PowerShell):
```cmd
clinic_system.exe
```

---

## 🗄️ File & Data Schema Reference

All data records are stored in the project's root directory in pipe-delimited (`|`) UTF-8 flat text files.

### 1. `pacientes.txt` (Patient Records)
* **Schema**: `<Nome> | <Contato> | <ID>`
* **Field Details**:
  * `Nome` (`char[64]`): Patient full name (alphabetic strings and spaces).
  * `Contato` (`char[64]`): Patient email address or telephone number.
  * `ID` (`int`): Unique auto-incremented patient identifier.
* **Example**:
  ```text
  Ana Beatriz Silva | ana.silva@email.com | 1
  Carlos Eduardo Lima | carlos.lima@email.com | 2
  ```

### 2. `medicos.txt` (Doctor Records)
* **Schema**: `<ID>|<Nome>|<Especialidade>|<InicioManha>|<FimManha>|<InicioTarde>|<FimTarde>`
* **Field Details**:
  * `ID` (`int`): Unique auto-incremented physician identifier.
  * `Nome` (`char[64]`): Physician full name.
  * `Especialidade` (`int`): Enum integer representation:
    * `0`: Clínico Geral (General Practitioner)
    * `1`: Pediatra (Pediatrician)
    * `2`: Dermatologista (Dermatologist)
    * `3`: Cardiologista (Cardiologist)
    * `4`: Outra (Other)
  * `InicioManha` / `FimManha` (`int`): Packed morning shift times in `HHMM` format (e.g., `800` for 08:00, `1100` for 11:00).
  * `InicioTarde` / `FimTarde` (`int`): Packed afternoon shift times in `HHMM` format (e.g., `1400` for 14:00, `1700` for 17:00).
* **Example**:
  ```text
  1|Dr. Roberto Alves|0|800|1100|1400|1700
  2|Dra. Mariana Costa|1|900|1200|1300|1600
  ```

### 3. `consultas.txt` (Appointment Records)
* **Schema**: `<ID> | <ID_Medico> | <ID_Paciente> | <Data> | <Inicio> | <Fim> | <Status>`
* **Field Details**:
  * `ID` (`int`): Unique auto-incremented appointment identifier.
  * `ID_Medico` (`int`): Foreign key referencing a valid Doctor `ID`.
  * `ID_Paciente` (`int`): Foreign key referencing a valid Patient `ID`.
  * `Data` (`int`): Packed date representation in `YYYYMMDD` format (e.g., `20260415` for April 15, 2026).
  * `Inicio` (`int`): Packed consultation start time in `HHMM` format.
  * `Fim` (`int`): Packed consultation end time in `HHMM` format (standard 1-hour duration).
  * `Status` (`int`): Appointment lifecycle status code:
    * `0`: `CONS_AGENDADA` (Scheduled)
    * `1`: `CONS_CONCLUIDA` (Completed)
    * `2`: `CONS_CANCELADA` (Canceled)
    * `3`: `CONS_FALTA` (Missed / No-Show)
* **Example**:
  ```text
  1 | 1 | 1 | 20260510 | 800 | 900 | 0
  2 | 2 | 2 | 20260512 | 1400 | 1500 | 1
  ```

---

## 📂 Codebase File Structure

| File | Type | Description |
| :--- | :--- | :--- |
| `mylib.h` | C Header | Central header containing struct definitions, enums, constants, and function prototypes. |
| `mylib.c` | C Source | Core utility library implementing time conversion, input validation, and line counting. |
| `paciente.c` | C Source | Patient subsystem implementing registration, lookup, modification, deletion, and listing. |
| `medico.c` | C Source | Doctor subsystem implementing physician management, shift validation, and specialty filtering. |
| `consulta.c` | C Source | Appointment subsystem implementing scheduling, conflict validation, status updates, and cascade deletion. |
| `main.c` | C Source | Entry point orchestrating system startup, interactive CLI menus, and memory cleanup. |
| `README.md` | Markdown | Comprehensive project documentation, technical architecture, and execution guide. |

---

## 🚀 Project Status

* **Current State**: Academic practical project completed.
* **Tested Environment**: Linux (GCC 9+, Clang 10+), C99 compliant.
* **Features Implemented**:
  - [x] Full CRUD operations for Doctors, Patients, and Appointments
  - [x] Dynamic memory reallocation in chunks of 10 slots
  - [x] Flat-file persistence with automatic startup loading
  - [x] Cascading deletions across relational records
  - [x] Shift boundary and appointment overlap validation
  - [x] Interactive multi-level terminal menu navigation
  - [x] Standardized Doxygen code documentation across all modules
