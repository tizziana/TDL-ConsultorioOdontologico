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
import multicola;
import comandos;

enum char SEPARADOR = ',';
enum string COMANDO_PEDIR_TURNO = "PEDIR_TURNO";
enum string COMANDO_ATENDER = "ATENDER_SIGUIENTE";

static void eliminar_fin_linea(string linea) {
	size_t len = linea.length;
	if (linea[len - 1] == '\n') {
		// linea.remove(len-1);
		// linea[len - 1] = '\0';
		strip(linea);
	}
}

int len_arreglo(string* arreglo) {
    int contador = 0;
    for (int i = 0; arreglo[i] != null; i ++) {
        contador++;
    }
    return contador;
}

void procesar_comando(const string comando, string* parametros, Multicola multicola) {
    int largo = len_arreglo(parametros);
    if (comando == COMANDO_PEDIR_TURNO) {
        if (largo != 3) {
            //fprintf(stdout, ENOENT_PARAMS, comando); // HACER MANEJO DE ERRORES
            return;
        }
		pedir_turno(parametros[0], parametros[1], multicola);
	} else if (comando == COMANDO_ATENDER) {
        if (largo != 1) {
            //fprintf(stdout, ENOENT_PARAMS, comando); // HACER MANEJO DE ERRORES
            return;
        }
		atender_paciente(multicola); 
	} else {
		//fprintf(stdout, ENOENT_CMD, comando); // HACER MANEJO DE ERRORES
	}
}

void procesar_entrada(Multicola multicola) { //PEDIR_TURNO:Flor,PRIORITARIO o ATENDER_SIGUIETE
	char* linea = null;
	size_t c = 0;
	string[] parametros;
	while (getline(&linea, &c, stdin) > 0) {
		string linea_s = to!string(linea);
		eliminar_fin_linea(linea_s);
		string[] campos = linea_s.split(':');
		if (campos[1] == null) {
			// printf(ENOENT_FORMATO, linea);
			// free_strv(campos);
            //MANEJO DE ERRORES
			continue;	
		}
        if (campos[1]) {
            parametros = split(campos[1], ',');
        } else {
            parametros = null;
        }
		
		procesar_comando(campos[0], parametros, multicola);
		// free_strv(parametros);
		// free_strv(campos); // LO HACE EL GARBAGE COLLECTOR?
	}
	free(linea);
}



int main() {
    Multicola multicola = new Multicola;
    
    procesar_entrada(multicola); 

    // cola del multicola destruir
    return 0;
}