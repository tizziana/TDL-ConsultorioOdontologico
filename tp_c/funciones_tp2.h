#include "multicolas.h"
#include "abb.h"
#include "hash.h"

// Devuelve si el string recibido denota prioridad en la clinica.
// Pre: existe un string que determina la prioridad.
// Post: devuelve true si lo que recibe denota prioridad, false en caso contrario.
bool asignar_prioridad(char* prioridad);

// Procesa el csv, creando un arbol binario con los doctores y sus datos, y agrega las especialidades a la multicola.
// Pre: la multicola fue creada y se recibio un csv con los doctores.
// Post: Proceso el archivo csv, creo el arbol binario y agrego las especialidades.
abb_t* creador_arbol_doctores(const char* ruta_csv_doctores, multicolas_t* colas);

// Procesa el csv, creando un hash con los pacientes y sus datos.
// Pre: recibio una ruta a un archivo csv.
// Post: proceso el archivo y creo un hash de pacientes.
hash_t* creador_hash_pacientes(const char* ruta_csv_pacientes);