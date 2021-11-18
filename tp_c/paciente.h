#include <string.h>
#include <stdlib.h>

// Estructura paciente
typedef struct paciente {
    char* nombre;
    char* anio;
} paciente_t;

// Crea una estructura paciente dinamica
// Post: la estructura fue creada
paciente_t* paciente_crear(char* nombre_paciente, char* anio);

// Destruye la estructura paciente
// Pre: la estructura fue creada
// Post: la estructura fue destruida
void paciente_destruir(paciente_t* paciente);

