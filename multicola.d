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
    return cola.lista_cola[0];
}

string cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return null;
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

    void multicola_encolar_prioritario(string paciente) {
        cola_encolar(this.cola_prioritaria, paciente);
    }

    void multicola_encolar_regular(string paciente) {
        cola_encolar(this.cola_regular, paciente);
    }

    string multicola_desencolar() {
        if (multicola_esta_vacia()) {
            return null;
        }
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
        if (multicola_esta_vacia()) {
            return null;
        }
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
    }
}


int main() {
    Multicola multicola = new Multicola();
    string num1 = "fede";
    string num2 = "flor";
    multicola.multicola_encolar_prioritario(num2);
    multicola.multicola_encolar_regular(num1);
    writeln(multicola.multicola_cantidad());

    string desencolar = multicola.multicola_desencolar();
    writeln(desencolar);

    return 0;
}