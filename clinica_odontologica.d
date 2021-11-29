import std.stdio;
import std.algorithm;
import std.conv:to;
import core.stdc.stdlib;
import std.conv;
import std.format;
import std.array : array;
import std.string;
import std.typecons : Yes;
import std.range; 
import std.parallelism;
import std.exception;
import std.traits;
import std.ascii : isAlpha;



enum string URGENTE = "URGENTE";
enum string REGULAR = "REGULAR";
enum string ATENDER_SIGUIETE = "ATENDER_SIGUIETE";
enum string PEDIR_TURNO = "PEDIR_TURNO";

//alias stdin = makeGlobal!"core.stdc.stdio.stdin".makeGlobal; 
//alias stdin = makeGlobal!(StdFileHandle.stdin);

void pedir_turno(Multicola multicola, string nombre, string prioridad) { 
    prioridad == URGENTE ? multicola.multicola_encolar_prioritario(nombre) : multicola.multicola_encolar_regular(nombre); 
	writeln("Se le asigno un turno con prioridad ", prioridad, " a ", nombre);
}

void atender_paciente(Multicola multicola) {
    string pacienteAtendido;
    try {
        pacienteAtendido = multicola.multicola_desencolar();
    } catch (Error err) {
        writeln(err.msg);
        return;
    }
    writeln("Se atendio al paciente ", pacienteAtendido);
}

void procesar_entrada(Multicola multicola) { //PEDIR_TURNO:Flor,PRIORITARIO o ATENDER_SIGUIETE
    string[] linea;
    string[] turno;
    string comando;
    string nombre;
    string prioridad;
    string paciente;

	string input;
	string[] lista = stdin.byLineCopy(Yes.keepTerminator).array();

    for (int i = 0; i < lista.length; i++) {
		input = lista[i];
		input = strip(input);
        linea = split(input, ":");
        comando = linea[0];

        if (comando == ATENDER_SIGUIETE) {
            atender_paciente(multicola);
        }
		else if (comando == PEDIR_TURNO) {
			paciente = linea[1];

			turno = split(paciente, ",");
    
            try {
			    nombre = turno[0];
			    prioridad = turno[1];
            } catch (Error err) {
                writeln("La cantidad de parametros no es suficiente");
                continue;
            }

			pedir_turno(multicola, nombre, prioridad);
		} 
        else {
            writeln("El comando es invalido");
        }
    }
}

int main () {
    Multicola multicola = new Multicola;
    
    procesar_entrada(multicola); 

    return 0;
}


// Multicola y cola
struct cola {
    string[] lista_cola;
}
alias cola cola_t;

cola_t *cola_crear() {
    cola_t *cola = cast(cola_t*) malloc((cola_t).sizeof); 
    if (cola == null) {
        return null;
    }
    string[] lista;
    cola.lista_cola = lista;
    return cola;
}

void cola_destruir(cola_t *cola){
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola.lista_cola.length == 0;
}

void cola_encolar(cola_t *cola, string valor) {
    cola.lista_cola ~= valor; 
}

string cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        throw new Error("La cola esta vacia");
    }
    return cola.lista_cola[0];
}

string cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        throw new Error("La cola esta vacia");
    }
    string dato = cola.lista_cola[0];
    cola.lista_cola = cola.lista_cola.remove(0);
    return dato;
}

int cola_cantidad(cola_t *cola) {
    return cast(int) cola.lista_cola.length;
}

class Multicola {
    cola_t* cola_prioritaria;
    cola_t* cola_regular;

    this() {
        this.cola_prioritaria = cola_crear();
        this.cola_regular = cola_crear();
    }

    void multicola_encolar_prioritario(string paciente)
    in {
        assert(isSomeString!(typeof(paciente)));
    } 
    body {
        cola_encolar(this.cola_prioritaria, paciente);    
    }

    void multicola_encolar_regular(string paciente)
    in {
        assert(isSomeString!(typeof(paciente)));
        assert(!isNumeric(paciente));
        assert(all!isAlpha(paciente));
    } 
    body {
        cola_encolar(this.cola_regular, paciente);
    }

    void multicola_encolar(string paciente, string urgencia) {
        if (urgencia == URGENTE) {
            multicola_encolar_prioritario(paciente);
        } else {
            multicola_encolar_regular(paciente);
        }
    }

    string multicola_desencolar() {
        if (!cola_esta_vacia(this.cola_prioritaria)) {
            return cola_desencolar(this.cola_prioritaria);
        }
        return cola_desencolar(this.cola_regular);
    }

    int multicola_cantidad() {
        return (cola_cantidad(this.cola_prioritaria) + cola_cantidad(this.cola_regular));
    }

    bool multicola_esta_vacia() {
        return !multicola_cantidad();
    }

    string multicola_ver_primero() {
        if (cola_esta_vacia(this.cola_prioritaria) == false) {
            return cola_ver_primero(this.cola_prioritaria);
        }
        return cola_ver_primero(this.cola_regular);
    }

    unittest {
        Multicola multicola = new Multicola;
        multicola.multicola_encolar_prioritario("Flor");
        multicola.multicola_encolar_regular("Fede");
        multicola.multicola_encolar_regular("Fran");
        multicola.multicola_encolar_prioritario("Tizziana");
        assert(multicola.multicola_cantidad() == 4);
        assert(multicola.multicola_desencolar() == "Flor");
        assert(multicola.multicola_desencolar() == "Tizziana");
        assert(multicola.multicola_desencolar() == "Fede");
        assert(multicola.multicola_desencolar() == "Fran");
        assert(multicola.multicola_cantidad() == 0);
        multicola.multicola_encolar("Flor", "URGENTE");
        assert(multicola.multicola_cantidad() == 1);
    }
}