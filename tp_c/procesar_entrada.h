#include "multicolas.h"
#include "hash.h"
#include "abb.h"

// Procesa la entrada del stdin, verifica que sea correcta y la ejecuta.
// Pre: la multicola, el arbol y el hash fueron creados previamente.
// Post: proceso la entrada, la ejecuto. Deja de leer stdin.
void procesar_entrada(multicolas_t* multicolas, abb_t* arbol_doctores, hash_t* hash_pacientes);