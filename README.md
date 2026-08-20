# Medical Appointment Scheduling System 🩺

## About the Project
This repository contains the source code for a "Smart Medical Appointment Scheduling System" (Sistema Inteligente para Agendamento de Consultas Médicas). It was developed as a final practical assignment for the Introduction to Programming course at the Universidade Federal do Agreste de Pernambuco (UFAPE), under the guidance of Professor René Gusmão[cite: 1]. 

The primary goal of this project is to apply core C programming concepts, specifically dynamic memory allocation, file manipulation, and modularization[cite: 1].

## Features
The system is accessed via an interactive menu and includes the following functionalities[cite: 1]:

* **Full CRUD Operations:** Create, Read (List), Update, and Delete records for Doctors, Patients, and Appointments[cite: 1].
* **Dynamic Memory Allocation:** The system initially allocates space for 10 records and dynamically expands its capacity by 10 new records whenever necessary[cite: 1].
* **Data Persistence:** All records are securely saved and loaded using text files[cite: 1].
* **Utility Functions:** Includes auxiliary functions for[cite: 1]:
  * Searching records by unique ID[cite: 1].
  * Validating inputs (e.g., date formatting, time, and mandatory fields)[cite: 1].
  * Generating filtered reports[cite: 1].
  * Checking schedule availability[cite: 1].
* **Error Handling:** Validates all user inputs and displays appropriate error messages for invalid data[cite: 1].

## Project Structure
To maintain good programming practices, the code is modularized[cite: 1]:
* `.h` files: Contain standard libraries, `struct` and `typedef` definitions, and function prototypes[cite: 1]. 
* `.c` files: Contain the core logic and implementation of the functions[cite: 1].

### Core Entities
The system models the following entities[cite: 1]:
* **Médico (Doctor):** ID, name, specialty, and typical shift hours[cite: 1].
* **Paciente (Patient):** ID, name, and contact information[cite: 1].
* **Consulta (Appointment):** Links a doctor and patient with a specific date, time slot, priority status, and current appointment status (Scheduled, Completed, Canceled, or Missed)[cite: 1].

## Status

- In progress...
