#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "doctor.h"

doctor_t* doctor_crear(char* nombre_doctor, char* nombre_especialidad) {
    doctor_t* doctor = malloc(sizeof(doctor_t));
    if (!doctor) return NULL;
    
    doctor->nombre = strdup(nombre_doctor);
    doctor->especialidad = strdup(nombre_especialidad);
    doctor->atendidos = 0;
    return doctor;
} 

void doctor_destruir(doctor_t* doctor) {
    free(doctor->nombre);
    free(doctor->especialidad);
    free(doctor);
}
