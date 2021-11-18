#define _POSIX_C_SOURCE 200809L //getline
#include "strutil.h"
#include "mensajes.h"
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#define SEPARADOR ','
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#include "multicolas.h"
#include "hash.h"
#include "abb.h"
#include "comandos.h"

// Elimina el fin de linea de un string.
static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

// Devuelve el largo de un arreglo de strings.
int len_arreglo(char** arreglo) {
    int contador = 0;
    for (int i = 0; arreglo[i] != NULL; i ++) {
        contador++;
    }
    return contador;
}

// Procesa el comando recibido por parametro y lo ejecuta.
void procesar_comando(const char* comando, char** parametros, multicolas_t* multicolas, abb_t* arbol_doctores, hash_t* hash_pacientes) {
    int largo = len_arreglo(parametros);
    if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
        if (largo != 3) {
            fprintf(stdout, ENOENT_PARAMS, comando);
            return;
        }
		pedir_turno(parametros[0], parametros[1], parametros[2], hash_pacientes, multicolas);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
        if (largo != 1) {
            fprintf(stdout, ENOENT_PARAMS, comando);
            return;
        }
		atender_paciente(parametros[0], arbol_doctores, multicolas);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
        if (largo != 2) {
            fprintf(stdout, ENOENT_PARAMS, comando);
            return;
        }
		crear_informe(arbol_doctores, parametros[0], parametros[1]);
	} else {
		fprintf(stdout, ENOENT_CMD, comando);
	}
}

void procesar_entrada(multicolas_t* multicolas, abb_t* arbol_doctores, hash_t* hash_pacientes) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, multicolas, arbol_doctores, hash_pacientes);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}