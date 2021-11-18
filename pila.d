import std.stdio;
import std.conv:to;
import core.stdc.stdlib;
import std.conv;
import std.format;
import std.typecons;
import std.array;
enum int CTE_REDIMENSION = 2;
enum int CAPACIDAD_INICIAL = 10;
enum int CTE_ACHICAMIENTO = 4;

struct pila {
    void** datos; // este tipo rompe tambien
    size_t cantidad; 
    size_t capacidad;  
}
alias pila pila_t;

pila_t* pila_crear() {
    //pila_t *pila = malloc(sizeof(pila_t));//???
    pila_t *pila = cast(pila_t*) malloc((pila_t).sizeof);
    if (pila == null) {
        return null;
    }

    //pila.datos = malloc(CAPACIDAD_INICIAL * sizeof(void*)); //????? ESTO ROMPE TODO
    pila.datos = cast(void**) malloc(CAPACIDAD_INICIAL * (void*).sizeof);

    if (pila.datos == null) {
        free(pila);
        return null;
    }
    pila.capacidad = CAPACIDAD_INICIAL;
    pila.cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila.datos);
    free(pila);
}

bool pila_redimensionar(pila_t *pila, size_t redimension) {
    //void **datos_nuevos = realloc(pila.datos, redimension * sizeof(void*)); //???
    void **datos_nuevos = cast(void**) realloc(pila.datos, redimension * (void*).sizeof);
    if (redimension > 0 && datos_nuevos == null) {
        return false; 
    }
    pila.datos = datos_nuevos;
    pila.capacidad = redimension;
    return true;
}

bool pila_esta_vacia(const pila_t *pila) {
    if (pila.cantidad == 0) {
        return true;
    }
    return false;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila.cantidad == pila.capacidad) {
        bool redimension = pila_redimensionar(pila, pila.capacidad * CTE_REDIMENSION);
        if (redimension == false) return false;
    }
    pila.datos[pila.cantidad] = valor; //?? supuestamente pasa de void* a void
    pila.cantidad ++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila.cantidad == 0) {
        return null;
    }
    void* tope = cast(void*)((pila).datos[pila.cantidad - 1]); 
    return tope;
}

void *pila_desapilar(pila_t *pila) {
    if (pila.cantidad == 0) {
        return null;
    }
    void* valor = pila.datos[pila.cantidad - 1]; 
    pila.cantidad --;
    if ((pila.cantidad * CTE_ACHICAMIENTO) <= pila.capacidad && pila.capacidad > CAPACIDAD_INICIAL) {
        pila_redimensionar(pila, pila.capacidad / CTE_REDIMENSION);
    }
    return valor;    
}