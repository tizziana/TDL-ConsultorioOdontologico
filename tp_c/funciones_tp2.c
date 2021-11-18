#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "hash.h"
#include "lista.h"
#include "abb.h"
#include <string.h> 
#include <stdlib.h>
#include "multicolas.h"
#include "paciente.h"
#include "doctor.h"
#include "csv.h"

#define URGENTE "URGENTE"
#define REGULAR "REGULAR"

bool asignar_prioridad(char* prioridad) {
    if (strcmp(prioridad, URGENTE) == 0) return true;
    return false;
}

int comparar_anios(const paciente_t* paciente1, const paciente_t* paciente2) {
    return strcmp(paciente2->anio, paciente1->anio);
}

int _comparar_anios(const void* paciente1, const void* paciente2) {
    return comparar_anios(paciente1, paciente2);
}

void* _creador_doctores(char** datos_doctor, void* extra) {
    return doctor_crear(datos_doctor[0], datos_doctor[1]);
}

void _doctor_destruir(void* doctor) {
    doctor_destruir(doctor);
}

abb_t* creador_arbol_doctores(const char* ruta_csv_doctores, multicolas_t* colas) {
    lista_t* lista = csv_crear_estructura(ruta_csv_doctores, _creador_doctores, NULL);
    lista_iter_t* iter = lista_iter_crear(lista);
    abb_t* abb = abb_crear(strcmp, _doctor_destruir);
    while (!lista_iter_al_final(iter)) {
        doctor_t* doctor = lista_iter_ver_actual(iter);
        abb_guardar(abb, doctor->nombre, doctor);       
        agregar_cola(colas, doctor->especialidad, _comparar_anios);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    return abb;
}

void* _creador_pacientes(char** paciente, void* extra) {
    return paciente_crear(paciente[0], paciente[1]);
}

void _paciente_destruir(void* paciente) {
    paciente_destruir(paciente);
}

hash_t* creador_hash_pacientes(const char* ruta_csv_pacientes) {
    lista_t* lista_pacientes = csv_crear_estructura(ruta_csv_pacientes, _creador_pacientes, NULL);
    hash_t* hash = hash_crear(_paciente_destruir);
    lista_iter_t* iter = lista_iter_crear(lista_pacientes);
    while (!lista_iter_al_final(iter)) {
        paciente_t* paciente = lista_iter_ver_actual(iter);
        hash_guardar(hash, paciente->nombre, paciente);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    lista_destruir(lista_pacientes, NULL);
    return hash;
}