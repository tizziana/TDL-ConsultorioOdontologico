import std.stdio;
import std.algorithm;
import std.conv:to;
import core.stdc.stdlib;
import std.conv;
import std.format;
import std.typecons;
import std.array;
import std.string;
// import std.typecons : No;
// import std.typecons;
// import std.range; 
// import std.parallelism;
// export cola_t;

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

int main(){
    cola_t* cola = cola_crear();
    cola_encolar(cola, "fede");
    cola_encolar(cola, "fLOR");
    writeln(cola_cantidad(cola));

    string desencolar = cola_desencolar(cola);
    writeln(cola_cantidad(cola));
    writeln(desencolar);
    cola_destruir(cola);
    return 0;
}