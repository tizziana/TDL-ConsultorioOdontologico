#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "abb.h"
#include <stdlib.h>
#include "multicolas.h"
#include "mensajes.h"
#include "paciente.h"
#include "doctor.h"
#include "lista.h"
#define URGENTE "URGENTE"
#define REGULAR "REGULAR"

void pedir_turno(char* nombre_paciente, char* especialidad, char* urgencia, hash_t* pacientes, multicolas_t* multicolas) {
    if (!hash_pertenece(pacientes, nombre_paciente)) {
        fprintf(stdout, ENOENT_PACIENTE, nombre_paciente);
        return;
    }
    if (!cola_pertenece(multicolas, especialidad)) {
        fprintf(stdout, ENOENT_ESPECIALIDAD, especialidad);
        return;
    }
    if (strcmp(urgencia, URGENTE) != 0 && strcmp(urgencia, REGULAR) != 0) {
        fprintf(stdout, ENOENT_URGENCIA, urgencia);
        return;
    }
    paciente_t* paciente = hash_obtener(pacientes, nombre_paciente);
    multicolas_encolar(multicolas, especialidad, paciente, urgencia);
    fprintf(stdout, PACIENTE_ENCOLADO, nombre_paciente);
    size_t en_espera = multicolas_cantidad_elementos(multicolas, especialidad);
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, en_espera, especialidad);
}

void atender_paciente(const char* nombre_doctor, abb_t* doctores, multicolas_t* colas) {
    if (!abb_pertenece(doctores, nombre_doctor)) {
        fprintf(stdout, ENOENT_DOCTOR, nombre_doctor);
        return;
    }
    doctor_t* doctor = abb_obtener(doctores, nombre_doctor);
    char* especialidad = doctor->especialidad;
    if (multicolas_cola_esta_vacia(colas, especialidad)) {
        fprintf(stdout, SIN_PACIENTES);
        return;
    }
    paciente_t* paciente = multicolas_desencolar(colas, especialidad);
    fprintf(stdout, PACIENTE_ATENDIDO, paciente->nombre);
    size_t en_espera = multicolas_cantidad_elementos(colas, especialidad);
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, en_espera, especialidad);
    doctor->atendidos++;
}

bool enlistar_doctores(const char* clave, doctor_t* doctor, void** arreglo) {
    if (strcmp((char*)arreglo[2], "") != 0 && strcmp((char*)arreglo[2], doctor->nombre) < 0) {
        return false;
    } 
    lista_t* lista = arreglo[0];
    lista_insertar_ultimo(lista, doctor);
    return true;
}

bool _enlistar_doctores(const char* clave, void* doctor, void* arreglo) {
    return enlistar_doctores(clave, doctor, arreglo);
}

void crear_informe(abb_t* doctores, char* inicio, char* final) {
    void** arreglo = malloc(sizeof(void*) * 3);
    lista_t* lista_doctores = lista_crear();
    arreglo[0] = lista_doctores;
    arreglo[1] = inicio;
    arreglo[2] = final;
    if (strcmp(inicio, "") == 0) {
        abb_in_order(doctores, _enlistar_doctores, arreglo);
    } else {
        abb_in_order_inicio(doctores, _enlistar_doctores, arreglo, inicio);
    }
    fprintf(stdout, DOCTORES_SISTEMA, lista_largo(lista_doctores));
    lista_iter_t* iter = lista_iter_crear(lista_doctores);
    size_t num = 1;
    while (!lista_iter_al_final(iter)) {
        doctor_t* doctor = lista_iter_ver_actual(iter);
        fprintf(stdout, INFORME_DOCTOR, num, doctor->nombre, doctor->especialidad, doctor->atendidos);
        num++;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    lista_destruir(lista_doctores, NULL);
    free(arreglo);
} 