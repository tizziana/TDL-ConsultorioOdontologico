#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>
typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//Crea un arbol binario.
//Recibe una función de comparación entre strings y una función para destruir datos.
//Devuelve un abb o NULL en caso de error.
//Pre: existe la función de comparación.
//Post: crea el árbol.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda una clave en el abb con su dato en caso de no pertenecer, o modifica el dato en caso de que la clave pertenezca.
//Recibe un arbol, un string como clave y un puntero a void como dato.
//Devuelve true si lo pudo guardar, false en caso contrario.
//Pre: el arbol existe.
//Post: guarda la clave y el dato en el árbol.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra un elemento del arbol si este pertenecía.
//Recibe un arbol y una clave en formato string.
//Devuelve el dato en caso de que la clave pertenezca o NULL si la clave no pertenecía.
//Pre: el arbol existe.
//Post: El elemento fue borrado de la estructura y se lo devolvió, en el caso de que estuviera guardado.
void *abb_borrar(abb_t *arbol, const char *clave);

//Obtiene el dato asignado a la clave si la clave pertenece al árbol.
//Pre: el árbol existe.
//Post: devuelve el dato asignado a la clave si pertenecía, NULL en caso contrario.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Devuelve false si la clave no pertenece al árbol, true en caso contrario.
//Pre: el árbol existe.
//Post: Devolvió si la clave pertenecía al árbol.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad de elementos almacenados.
//Pre: el árbol existe.
//Post: Devolvió la cantidad.
size_t abb_cantidad(abb_t *arbol);

//Destruye el árbol y sus datos en caso de existir la función destruir dato.
//Pre: el árbol existe.
//Post: el árbol fue destruído (y sus datos si corresponde).
void abb_destruir(abb_t *arbol);

//Recorre el árbol in order aplicando a los datos la función visitar hasta false.
//Pre: el árbol existe.
//Post: el árbol fue recorrido y la función visitar aplicada.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

typedef struct abb_iter abb_iter_t;

// Crea un iterador externo inorder. Recibe el árbol al que apuntará.
// Pre: el árbol fue creado
// Post: devuelve una nuevo iterador apuntando al primer elemento del árbol inorder.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza el iterador. Devuelve false si ya no puede avanzar más, true en el caso contrario.
// Pre: el iterador y su árbol fueron creados
// Post: el iterador apunta al siguiente elemento del árbol, o
// sigue apuntando al final de él, si ya estaba en el final.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Obtiene el dato del elemento al que apunta el iterador
// Pre: el iterador y su árbol fueron creados
// Post: se devolvió el dato del árbol al que apunta el iterador
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador apunta al final del árbol (NULL),
// false en el caso contrario.
// Pre: el iterador y su árbol fueron creados
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador
// Pre: el iterador fue creado
void abb_iter_in_destruir(abb_iter_t* iter);

// Recorre el árbol in order aplicando a los datos la función visitar hasta false desde el inicio establecido. Emplea el algoritmo de busqueda
// binaria para hallar el primer iterado.
// Pre: el árbol existe.
// Post: el árbol fue recorrido desde el inicio establecido y la función visitar aplicada.
void abb_in_order_inicio(abb_t* arbol, bool visitar(const char *, void *, void *), void* extra, const char* inicio);

void pruebas_abb_estudiante(void);
#endif  // ABB_H
