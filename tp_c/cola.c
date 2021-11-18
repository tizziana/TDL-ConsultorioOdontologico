#include "cola.h"
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo *sig;
} nodo_t;

nodo_t *nodo_crear(void* dato) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        return NULL;
    }
    nodo->dato = dato;
    nodo->sig = NULL;
    return nodo;
}

struct cola {
    nodo_t *prim;
    nodo_t *ult;
};

cola_t *cola_crear(void) {
    cola_t *cola = malloc(sizeof(cola_t));
    if (cola == NULL) {
        return NULL;
    }
    cola->prim = NULL;
    cola->ult = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    nodo_t *actual = cola->prim;
    while (actual != NULL) {
        if (destruir_dato != NULL) {
            destruir_dato(actual->dato);
        }
        nodo_t *prox = actual->sig;
        free(actual);
        actual = prox;
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t *nuevo = nodo_crear(valor);
    if (nuevo == NULL) {
        return false;
    }
    if (cola->prim == NULL) {
        cola->prim = nuevo;
    }
    else {
        cola->ult->sig = nuevo;
    }
    cola->ult = nuevo;
    return true; 
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola->prim == NULL) {
        return NULL;
    }
    return cola->prim->dato;
}

void *cola_desencolar(cola_t *cola) {
    if (cola->prim == NULL) {
        return NULL;
    }
    nodo_t *sacar = cola->prim;
    cola->prim = cola->prim->sig;
    void* dato = sacar->dato;
    free(sacar);
    return dato;
}