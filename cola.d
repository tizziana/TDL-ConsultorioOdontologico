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
enum int CTE_REDIMENSION = 2;
enum int CAPACIDAD_INICIAL = 10;
enum int CTE_ACHICAMIENTO = 4;

struct cola {
    void*[] lista_cola;
}
alias cola cola_t;

cola_t *cola_crear() {
    cola_t *cola = cast(cola_t*) malloc((cola_t).sizeof); 
    if (cola == null) {
        return null;
    }
    void*[] lista;
    cola.lista_cola = lista;
    return cola;
}

void cola_destruir(cola_t *cola){
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola.lista_cola[0] == null;
}

bool cola_encolar(cola_t *cola, void *valor) {
    cola.lista_cola ~= valor; 
    return true;
}

void *cola_ver_primero(const cola_t *cola) {
    return cast(void*) cola.lista_cola[0];
}

void *cola_desencolar(cola_t *cola) {
    if (cola.lista_cola[0] == null) {
        return null;
    }
    void* dato = cola.lista_cola[0];
    //void*[] lista = cola.lista_cola;
    cola.lista_cola.remove(0);
    return dato;
}

int main(){
    cola_t* cola = cola_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    cola_encolar(cola, &num1);
    cola_encolar(cola, &num2);
    cola_encolar(cola, &num3);
    cola_encolar(cola, &num4);
    void*[] lista = cast(void*[]) cola.lista_cola;

    int* desencolar = cast(int*) cola_desencolar(cola);
    writeln(cast(int) *desencolar);
    cola_destruir(cola);
    return 0;
}