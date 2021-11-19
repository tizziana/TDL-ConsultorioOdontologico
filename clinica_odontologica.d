import std.stdio;
import std.algorithm;
import std.conv:to;
import core.stdc.stdlib;
import std.conv;
import std.format;
import std.typecons;
import std.array;
import std.string;
import std.typecons : No;
import std.typecons;
import std.range; 
import std.parallelism;

#define SEPARADOR ',' //hacer cte
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"


static void eliminar_fin_linea(string linea) {
	size_t len = linea.length;
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_comando(const char* comando, char** parametros, Multicola multicolas) {
    int largo = len_arreglo(parametros);
    if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
        if (largo != 3) {
            fprintf(stdout, ENOENT_PARAMS, comando); // HACER MANEJO DE ERRORES
            return;
        }
		pedir_turno(parametros[0], parametros[1], parametros[2], multicola);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
        if (largo != 1) {
            fprintf(stdout, ENOENT_PARAMS, comando); // HACER MANEJO DE ERRORES
            return;
        }
		atender_paciente(multicola); 
	} else {
		fprintf(stdout, ENOENT_CMD, comando); // HACER MANEJO DE ERRORES
	}
}

void procesar_entrada(Multicola multicola) { //PEDIR_TURNO:Flor,PRIORITARIO o ATENDER_SIGUIETE
	string linea = null;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == null) {
			// printf(ENOENT_FORMATO, linea);
			// free_strv(campos);
            //MANEJO DE ERRORES
			continue;	
		}
        if (campos[1]) {
            char** parametros = split(campos[1], ',');
        } else {
            char** parametros = null;
        }
		
		procesar_comando(campos[0], parametros, multicola);
		// free_strv(parametros);
		// free_strv(campos); // LO HACE EL GARBAGE COLLECTOR?
	}
	free(linea);
}



int main(int argc, char** argv) {
    Multicola multicola = new Multicola;
    
    procesar_entrada(multicolas); 

    // cola del multicola destruir
    return 0;
}