# Medical Appointment Scheduling System 🩺

## About the Project
This repository contains the source code for a "Smart Medical Appointment Scheduling System" (Sistema Inteligente para Agendamento de Consultas Médicas). It was developed as a final practical assignment for the Introduction to Programming course at the Universidade Federal do Agreste de Pernambuco (UFAPE), under the guidance of Professor René Gusmão. 

The primary goal of this project is to apply core C programming concepts, specifically dynamic memory allocation, file manipulation, and modularization.

## Features
The system is accessed via an interactive menu and includes the following functionalities:

* **Full CRUD Operations:** Create, Read (List), Update, and Delete records for Doctors, Patients, and Appointments.
* **Dynamic Memory Allocation:** The system initially allocates space for 10 records and dynamically expands its capacity by 10 new records whenever necessary.
* **Data Persistence:** All records are securely saved and loaded using text files.
* **Utility Functions:** Includes auxiliary functions for:
  * Searching records by unique ID.
  * Validating inputs (e.g., date formatting, time, and mandatory fields).
  * Generating filtered reports.
  * Checking schedule availability.
* **Error Handling:** Validates all user inputs and displays appropriate error messages for invalid data.

## Project Structure
To maintain good programming practices, the code is modularized:
* `.h` files: Contain standard libraries, `struct` and `typedef` definitions, and function prototypes. 
* `.c` files: Contain the core logic and implementation of the functions.

### Core Entities
The system models the following entities.
* **Médico (Doctor):** ID, name, specialty, and typical shift hours.
* **Paciente (Patient):** ID, name, and contact information.
* **Consulta (Appointment):** Links a doctor and patient with a specific date, time slot, priority status, and current appointment status (Scheduled, Completed, Canceled, or Missed).

## Status

- In progress...
