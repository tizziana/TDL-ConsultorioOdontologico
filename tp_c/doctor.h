#include <string.h>
#include <stdlib.h>

// Estructura doctor
typedef struct doctor {
    char* nombre;
    char* especialidad;
    size_t atendidos;
} doctor_t;

// Crea una estructura doctor dinamica
// Post: la estructura fue creada
doctor_t* doctor_crear(char* nombre_doctor, char* nombre_especialidad);

// Destruye la estructura doctor
// Pre: la estructura fue creada
// Post: la estructura fue destruida
void doctor_destruir(doctor_t* doctor);
