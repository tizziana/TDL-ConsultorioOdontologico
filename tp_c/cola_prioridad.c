#include "cola.h"
#include "stdbool.h"
#include <stddef.h>
#include <stdlib.h>
#include "cola_prioridad.h"
#include "heap.h"

struct cola_prioridad {
    cola_t* cola_prior;
    heap_t* cola_regular;
    size_t cantidad;
    func_priori_t es_priori;
    destruir_dato_t destruir;
};

cola_prioridad_t* cola_prioridad_crear(cmp_func_t cmp, func_priori_t es_priori, destruir_dato_t destruir_dato) {
    cola_prioridad_t* cola_prioridad = malloc(sizeof(cola_prioridad_t));
    if (!cola_prioridad) return NULL;
    cola_prioridad->es_priori = es_priori;
    cola_prioridad->destruir = destruir_dato;
    cola_prioridad->cola_prior = cola_crear();
    cola_prioridad->cola_regular = heap_crear(cmp);
    cola_prioridad->cantidad = 0;
    return cola_prioridad;
}

void cola_prioridad_destruir(cola_prioridad_t* cola_prioridad) {
    cola_destruir(cola_prioridad->cola_prior, cola_prioridad->destruir);
    heap_destruir(cola_prioridad->cola_regular, cola_prioridad->destruir);
    free(cola_prioridad);
}

bool cola_prioridad_encolar(cola_prioridad_t* cola_prioridad, void* dato, void* prioridad) {
    bool val;
    if (cola_prioridad->es_priori(prioridad)) {
        val = cola_encolar(cola_prioridad->cola_prior, dato);
    } else {
        val = heap_encolar(cola_prioridad->cola_regular, dato);
    }
    cola_prioridad->cantidad++;
    return val;
}

void* cola_prioridad_desencolar(cola_prioridad_t* cola_prioridad) {
    if (cola_esta_vacia(cola_prioridad->cola_prior) && heap_esta_vacio(cola_prioridad->cola_regular)) return NULL;
    cola_prioridad->cantidad--;
    if (!cola_esta_vacia(cola_prioridad->cola_prior)) {
        return cola_desencolar(cola_prioridad->cola_prior);
    } else {
        return heap_desencolar(cola_prioridad->cola_regular);
    }
}

size_t cola_prioridad_cantidad(cola_prioridad_t* cola_prioridad) {
    return cola_prioridad->cantidad;
}

bool cola_prioridad_esta_vacia(cola_prioridad_t* cola_prioridad) {
    return cola_prioridad_cantidad(cola_prioridad) == 0;
}

void* cola_prioridad_ver_primero(cola_prioridad_t* cola_prioridad) {
    if (cola_esta_vacia(cola_prioridad->cola_prior) && heap_esta_vacio(cola_prioridad->cola_regular)) {
        return NULL;
    } else if (!cola_esta_vacia(cola_prioridad->cola_prior)) {
        return cola_ver_primero(cola_prioridad->cola_prior);
    } else {
        return heap_ver_max(cola_prioridad->cola_regular);
    }
}