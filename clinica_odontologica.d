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
import std.file;
import std.concurrency;
import core.thread;


/***********************************************/
/**           CLÍNICA ODONTOLOGICA            **/
/***********************************************/

enum string URGENTE = "URGENTE";
enum string REGULAR = "REGULAR";
enum string ATENDER_SIGUIETE = "ATENDER_SIGUIETE";
enum string PEDIR_TURNO = "PEDIR_TURNO";
enum string INFORME = "INFORME";

string escribir_resumen(string[][int] pacientes, int desde, ulong hasta) {
    auto dni_pacientes = pacientes.keys.reverse()[desde - 1 .. hasta];
    auto lista_pacientes = pacientes.values.reverse()[desde - 1 .. hasta];
    File archivo = File("resumen_clinica.txt", "a");
    for (int i = 0; i <= dni_pacientes.length - 1; i ++) {
        archivo.writeln(format("dni %d, nombre %s, cantidad de veces atendido %s", dni_pacientes[i], lista_pacientes[i][0], lista_pacientes[i][1]));
        continue;
    }
    archivo.close();
    return "Resumen escrito correctamente";
}

void escribir_reporte(string mensaje) {
    File archivo = File("reporte_clinica.txt", "a");
    archivo.writeln(mensaje);
    archivo.close();
    writeln(mensaje);
}

string pedir_turno(Multicola multicola, paciente_t* Paciente) { 
    Paciente.prioridad == URGENTE ? multicola.multicola_encolar_prioritario(Paciente) : multicola.multicola_encolar_regular(Paciente); 
    string mensaje = format("Se le asigno un turno con prioridad %s a %s", Paciente.prioridad, Paciente.nombre);
    return mensaje;
}

string[][int] incrementar_atendido(string[][int] pacientes, paciente_t* pacienteAtendido) {
    if (0 in pacientes) {pacientes.remove(0);} // porque sino no funciona
    
    pacientes[pacienteAtendido.dni] = pacientes.get(pacienteAtendido.dni, [pacienteAtendido.nombre, "0"]);
    auto lista = pacientes[pacienteAtendido.dni];
    string snum = lista[1];
    int num = to!int(snum);
    num++;
    string snum2 = to!string(num);
    lista[1] = snum2;
    return pacientes;
}

string atender_paciente(Multicola multicola, string[][int] pacientes) {
    paciente_t* pacienteAtendido;
    try {
        pacienteAtendido = multicola.multicola_desencolar();
    } catch (Error err) {
        return err.msg;
    }
    string mensaje = format("Se atendio al paciente %s", pacienteAtendido.nombre);
    pacientes = incrementar_atendido(pacientes, pacienteAtendido);
    return mensaje;
}

void procesar_entrada(Multicola multicola) { //PEDIR_TURNO:Flor,PRIORITARIO o ATENDER_SIGUIETE
    string[] linea;
    string[] turno;
    string comando;
    string[][int] pacientes;
    pacientes[0] = ["Nombre", "cantidad_veces_atendido"]; // se inicializa porque sino no funciona
	
	string[] lista = stdin.byLineCopy(Yes.keepTerminator).array();

    for (int i = 0; i < lista.length; i++) {
        string mensaje;
        linea = split(strip(lista[i]), ":");
        comando = linea[0];

        if (comando == ATENDER_SIGUIETE) {
            mensaje = atender_paciente(multicola, pacientes);
        }
		else if (comando == PEDIR_TURNO) {
			turno = split(linea[1], ",");
    
            try {
                paciente_t* Paciente = crear_paciente(turno[0], turno[1], to!int(turno[2]));
                mensaje = pedir_turno(multicola, Paciente);
            } catch (Error err) {
                mensaje = "La cantidad de parametros no es suficiente";
            }
		} 
        else if (comando == INFORME) {
            string[] limites = split(linea[1], ",");

            ulong hasta = to!ulong(limites[1]);

            hasta > pacientes.length ? hasta = pacientes.length : true;

            try {
                mensaje = escribir_resumen(pacientes, to!int(limites[0]), hasta); 
            } catch (Error err) {
                mensaje = "La cantidad de parametros no es suficiente AHhHHHHHHHHh";
            }
        }
        else {
            mensaje = "El comando es invalido";
        }
        new Thread({
            escribir_reporte(mensaje);
        }).start().join(); // une hilo secundario con el principal    
    }
}

int main () {
    // if (argc != 3) {
    //     // fprintf(stdout, ENOENT_CANT_PARAMS);
    //     return 1;
    // }
    // int desde = cast(int) argv[1];
    // int hasta = cast(int) argv[2];

    Multicola multicola = new Multicola;

    procesar_entrada(multicola); 
    return 0;
}


/***********************************************/
/**                PACIENTE                   **/
/***********************************************/

struct Paciente{
    string nombre;
    string prioridad;
    int dni;
}
alias Paciente paciente_t;

paciente_t* crear_paciente(string nombre, string prioridad, int dni){
    paciente_t* Paciente = cast(paciente_t*) malloc((paciente_t).sizeof);
    if(!Paciente) return null;
    Paciente.nombre = nombre;
    Paciente.prioridad = prioridad;
    Paciente.dni = dni;
    return Paciente;
}



/***********************************************/
/**                   COLA                    **/
/***********************************************/

struct cola {
    paciente_t*[] lista_cola;
}
alias cola cola_t;

cola_t *cola_crear() {
    cola_t *cola = cast(cola_t*) malloc((cola_t).sizeof); 
    if (cola == null) {
        return null;
    }
    paciente_t*[] lista;
    cola.lista_cola = lista;
    return cola;
}

void cola_destruir(cola_t *cola){
    free(cola);
}

bool cola_esta_vacia(cola_t *cola) {
    return cola.lista_cola.length == 0;
}

void cola_encolar(cola_t *cola, paciente_t* valor) {
    cola.lista_cola ~= valor; 
}

paciente_t* cola_ver_primero(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        throw new Error("La cola esta vacia");
    }
    return cola.lista_cola[0];
}

paciente_t* cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        throw new Error("La cola esta vacia");
    }
    paciente_t* dato = cola.lista_cola[0];
    cola.lista_cola = cola.lista_cola.remove(0);
    return dato;
}

int cola_cantidad(cola_t *cola) {
    return cast(int) cola.lista_cola.length;
}


/***********************************************/
/**               MULTICOLA                   **/
/***********************************************/

class Multicola {
    cola_t* cola_prioritaria;
    cola_t* cola_regular;

    this() {
        this.cola_prioritaria = cola_crear();
        this.cola_regular = cola_crear();
    }

    void multicola_encolar_prioritario(paciente_t* paciente)
    in {
        // assert(isSomeString!(typeof(nombrePaciente)));
    } 
    body {
        cola_encolar(this.cola_prioritaria, paciente);    
    }

    void multicola_encolar_regular(paciente_t* paciente)
    in {
        // assert(isSomeString!(typeof(nombrePaciente)));
        // assert(!isNumeric(nombrePaciente));
        // assert(all!isAlpha(nombrePaciente));
    } 
    body {
        cola_encolar(this.cola_regular, paciente);
    }

    paciente_t* multicola_desencolar() {
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

    paciente_t* multicola_ver_primero() {
        if (cola_esta_vacia(this.cola_prioritaria) == false) {
            return cola_ver_primero(this.cola_prioritaria);
        }
        return cola_ver_primero(this.cola_regular);
    }

    unittest {
        Multicola multicola = new Multicola;
        paciente_t* flor = Paciente("Flor", "URGENTE", 42933535);
        multicola.multicola_encolar_prioritario(flor);
        // multicola.multicola_encolar_regular("Fede");
        // multicola.multicola_encolar_regular("Fran");
        // multicola.multicola_encolar_prioritario("Tizziana");
        // assert(multicola.multicola_cantidad() == 4);
        assert(multicola.multicola_desencolar() == flor);
        // assert(multicola.multicola_desencolar() == "Tizziana");
        // assert(multicola.multicola_desencolar() == "Fede");
        // assert(multicola.multicola_desencolar() == "Fran");
        // assert(multicola.multicola_cantidad() == 0);
        // multicola.multicola_encolar("Flor", "URGENTE");
        // assert(multicola.multicola_cantidad() == 1);
    }
}