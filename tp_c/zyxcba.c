#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "abb.h"
#include <stdlib.h>
#include "multicolas.h"
#include "procesar_entrada.h"
#include "mensajes.h"
#include "funciones_tp2.h"
#include "doctor.h"

bool _asignar_prioridad(void* prioridad) {
    return asignar_prioridad(prioridad);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stdout, ENOENT_CANT_PARAMS);
        return 1;
    }
    
    const char* ruta_csv_doctores = argv[1];
    const char* ruta_csv_pacientes = argv[2];
    
    multicolas_t* multicolas = multicolas_crear(_asignar_prioridad, NULL);
    abb_t* arbol_doctores = creador_arbol_doctores(ruta_csv_doctores, multicolas);
    hash_t* hash_pacientes = creador_hash_pacientes(ruta_csv_pacientes);
    
    procesar_entrada(multicolas, arbol_doctores, hash_pacientes); 

    abb_destruir(arbol_doctores);
    hash_destruir(hash_pacientes);
    multicolas_destruir(multicolas);
    return 0;
}