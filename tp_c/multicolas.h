#ifndef MULTICOLAS_H
#define MULTICOLAS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "cola_prioridad.h"

typedef struct multicolas multicolas_t;

// typedef bool (*func_priori_t)(void* dato);
// typedef void (*destruir_dato_t)(void* dato);
// typedef int (*cmp_func_t)(const void *a, const void *b);

// Crea una multicola.
// Pre: existen tanto una funcion de destruccion de datos como una funcion para identificar a los elementos
// que se consideran prioritarios sobre los demás.
// Post: Devuelve una multicola vacia.
multicolas_t* multicolas_crear(func_priori_t es_prioritario, destruir_dato_t destruir);

// Destruye la multicola y sus datos (si se pide). 
// Pre: la multicola fue creada.
// Post: se destruye la multicola (y si se pide, sus datos).
void multicolas_destruir(multicolas_t* multicolas);

// Agrega una cola con su clave a la multicola.
// Pre: la multicola fue creada.
// Post: se agrega una nueva cola a la multicola.
bool agregar_cola(multicolas_t* multicolas, const char* clave, cmp_func_t cmp);

// Devuelve true si encuentra una cola con la clave pasada por parametro, false en caso contrario.
// Pre: la multicola fue creada.
// Post: se devolve si existe una cola con la clave 
bool cola_pertenece(multicolas_t* multicolas, const char* clave);

// Encola en la cola adjudicada a la clave pasada por parametro un elemento teniendo 
// en cuenta la prioridad recibida. Devuelve false si ocurrio un error
// o si no existe la cola buscada.
// Pre: la multicola fue creada.
// Post: el elemento fue encolado en la cola correspondiente.
bool multicolas_encolar(multicolas_t* multicolas, const char* clave, void* elemento, void* prioridad);

// Desencola en la cola adjudicada a la clave un elemento y lo devuelve
// Devuelve NULL en caso de que la cola este vacia o que no exista dicha cola.
// Pre: la multicola fue creada.
// Post: se densencola un elemento de una cola.
void* multicolas_desencolar(multicolas_t* multicolas, const char* clave);


// Devuelve la cantidad de colas que existen en las multicolas
// Pre: la multicola fue creada
// Post: se devolvio la cantidad de colas.
size_t multicolas_cantidad(multicolas_t* multicolas);


// Devuelve la cantidad de elementos en una cola determinada. Si la
// cola no existe, devuelve 0.
// Pre: la multicola fue creada
// Post: Se devolvio la cantidad de elementos de una cola
size_t multicolas_cantidad_elementos(multicolas_t* multicolas, const char* clave);

// Devuelve true si la multicola no tiene colas o false en caso contrario.
// Pre: la multicola fue creada.
// Post: devuelve si la multicola no tiene colas.
bool multicolas_esta_vacio(multicolas_t* multicolas);

// Devuelve true si la cola buscada esta vacia o false en el caso contrario.
// Devuelve true si la cola no existe.
// Pre: la multicola fue creada.
// Post: devuelve si la cola buscada esta vacia.
bool multicolas_cola_esta_vacia(multicolas_t* multicolas, const char* clave);

#endif  // MULTICOLA_H
