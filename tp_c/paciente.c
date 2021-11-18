#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "paciente.h"

paciente_t* paciente_crear(char* nombre_paciente, char* anio) {
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if (!paciente) return NULL;
    
    paciente->nombre = strdup(nombre_paciente);
    paciente->anio = strdup(anio);
    return paciente;
} 

void paciente_destruir(paciente_t* paciente) {
    free(paciente->nombre);
    free(paciente->anio);
    free(paciente);
}