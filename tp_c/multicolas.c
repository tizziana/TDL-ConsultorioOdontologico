#include "hash.h"
#include "cola_prioridad.h"
#include "stdbool.h"
#include "multicolas.h"

void _cola_prioridad_destruir(void* cola) {
    cola_prioridad_destruir((cola_prioridad_t*) cola);
}

struct multicolas {
    hash_t* colas;
    size_t cantidad; //cantidad de colas
    func_priori_t es_priori;
    destruir_dato_t destruir;
};

multicolas_t* multicolas_crear(func_priori_t es_prioritario, destruir_dato_t destruir) {
    multicolas_t* multicolas = malloc(sizeof(multicolas_t));
    multicolas->colas = hash_crear(_cola_prioridad_destruir);
    multicolas->cantidad = 0;
    multicolas->es_priori = es_prioritario;
    multicolas->destruir = destruir;
    return multicolas;
}

void multicolas_destruir(multicolas_t* multicolas) {
    hash_destruir(multicolas->colas);
    free(multicolas);
}

bool agregar_cola(multicolas_t* multicolas, const char* clave, cmp_func_t cmp) {
    if (hash_pertenece(multicolas->colas, clave)) return false;
    multicolas->cantidad++;
    return hash_guardar(multicolas->colas, clave, cola_prioridad_crear(cmp, multicolas->es_priori, multicolas->destruir));
}

bool cola_pertenece(multicolas_t* multicolas, const char* clave) {
    return hash_pertenece(multicolas->colas, clave);
}

bool multicolas_encolar(multicolas_t* multicolas, const char* clave, void* elemento, void* prioridad) {
    cola_prioridad_t* cola = hash_obtener(multicolas->colas, clave);
    if (!cola) return false;

    return cola_prioridad_encolar(cola, elemento, prioridad);
}

void* multicolas_desencolar(multicolas_t* multicolas, const char* clave) {
    cola_prioridad_t* cola = hash_obtener(multicolas->colas, clave);
    if (!cola) return NULL;

    return cola_prioridad_desencolar(cola);
}

size_t multicolas_cantidad(multicolas_t* multicolas) {
    return multicolas->cantidad;
}

size_t multicolas_cantidad_elementos(multicolas_t* multicolas, const char* clave) {
    cola_prioridad_t* cola = hash_obtener(multicolas->colas, clave);
    if (!cola) return 0;

    return cola_prioridad_cantidad(cola);
}

bool multicolas_esta_vacio(multicolas_t* multicolas) {
    return multicolas_cantidad(multicolas) == 0;
}

bool multicolas_cola_esta_vacia(multicolas_t* multicolas, const char* clave) {
	cola_prioridad_t* cola = hash_obtener(multicolas->colas, clave);
    if (!cola) return true;
    
	return cola_prioridad_esta_vacia(cola);
}