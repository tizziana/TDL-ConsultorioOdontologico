#ifndef COLA_PRIORIDAD_H
#define COLA_PRIORIDAD_H

#include <stdbool.h>
#include <stddef.h>
#include "cola.h"
#include "heap.h"

// La cola de prioridad funciona con una prioridad binaria, es decir,
// se centra en si algo es prioritario o no, desencolando
// primero a los prioritarios y luego a los regulares. El conjunto de
// regulares, además, se encuentra ordenado según una función de
// comparación, mientras que el de prioritarios respeta FIFO.

// Cabe destacar que, a diferencia de la cola comun, permite
// acceder a la cantidad de elementos.

typedef bool (*func_priori_t)(void* dato);
typedef void (*destruir_dato_t)(void* dato);
//typedef int (*cmp_func_t)(const void *a, const void *b);

typedef struct cola_prioridad cola_prioridad_t;

// Crea una cola de prioridad.
// Pre: existen tanto una funcion de destruccion como
// una funcion que determine prioridad.
// Post: devuelve una nueva cola vacía.
cola_prioridad_t* cola_prioridad_crear(cmp_func_t cmp, func_priori_t es_priori, destruir_dato_t destruir_dato);

// Destruye la cola de prioridad y sus datos (si se pide)
// Pre: la cola de prioridad fue creada
// Post: se destruye la cola de prioridad (y si se pide, sus datos)
void cola_prioridad_destruir(cola_prioridad_t* cola_prioridad);

// Agrega un nuevo elemento en la cola, devuelve falso en caso de error
// Respeta el orden fifo dentro de los elementos prioritarios y a la funcion de comparacion
// dentro de los regulares, saliendo primero todos los prioritarios.
// Pre: la cola de prioridad fue creada
// Post: se agrego un nuevo elemento a la cola.
bool cola_prioridad_encolar(cola_prioridad_t* cola_prioridad, void* dato, void* prioridad);


// Saca el primer elemento de la cola de prioridad. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola de prioridad fue creada.
// Post: se devolvió el valor del primer elemento, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_prioridad_desencolar(cola_prioridad_t* cola_prioridad);


// Devuelve la cantidad de elementos encolados.
// Pre: la cola de prioridad fue creada.
// Post: devolvió la cantidad de elementos encolados.
size_t cola_prioridad_cantidad(cola_prioridad_t* cola_prioridad);

// Devuelve verdadero si la cola de prioridad no tiene elementos encolados, false en caso contrario.
// Pre: la cola de prioridad fue creada.
// Post: devolvio si la cola estaba vacia.
bool cola_prioridad_esta_vacia(cola_prioridad_t* cola_prioridad);


// Devuelve el primer elemento de la cola, o NULL si esta vacia.
// Pre: la cola de prioridad fue creada.
// Post: se devolvio el primer elemento.
void* cola_prioridad_ver_primero(cola_prioridad_t* cola_prioritaria);

#endif  // COLA_PRIORIDAD_H
