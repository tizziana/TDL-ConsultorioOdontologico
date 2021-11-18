#include "pila.h"

#include <stdlib.h>

#define CTE_REDIMENSION 2
#define CAPACIDAD_INICIAL 10
#define CTE_ACHICAMIENTO 4

struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }

    pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));  

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_redimensionar(pila_t *pila, size_t redimension) {
    void **datos_nuevos = realloc(pila->datos, redimension * sizeof(void*));
    if (redimension > 0 && datos_nuevos == NULL) {
        return false; 
    }
    pila->datos = datos_nuevos;
    pila->capacidad = redimension;
    return true;
}

bool pila_esta_vacia(const pila_t *pila) {
    if (pila->cantidad == 0) {
        return true;
    }
    return false;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        bool redimension = pila_redimensionar(pila, pila->capacidad * CTE_REDIMENSION);
        if (redimension == false) return false;
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad ++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila->cantidad == 0) {
        return NULL;
    }
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila->cantidad == 0) {
        return NULL;
    }
    void* valor = pila->datos[pila->cantidad - 1];
    pila->cantidad --;
    if ((pila->cantidad * CTE_ACHICAMIENTO) <= pila->capacidad && pila->capacidad > CAPACIDAD_INICIAL) {
        pila_redimensionar(pila, pila->capacidad / CTE_REDIMENSION);
    }
    return valor;    
}