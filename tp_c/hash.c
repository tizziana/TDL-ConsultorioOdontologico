#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // para el strdup() 

#define CAP 13
#define CTE_REDIMENSION 2
#define CARGA_MAX 0.7
#define CARGA_MIN 0.1


// Funcion hash utilizada. Fuente:
// https://stackoverflow.com/questions/21001659/crc32-algorithm-implementation-in-c-without-a-look-up-table-and-with-a-public-li
// Puede no ser la mas optima, pero funciona por ahora
unsigned int crc32(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

// Representa el estado de cada elemento
typedef enum status {
	VACIO,
	OCUPADO,
	BORRADO
} status_t;

// El struct donde se guardaran las claves y su dato.
typedef struct elemento {
    char *clave;
    void *dato;
    status_t estado;
} elemento_t;

// Crea y devuelve un elemento
elemento_t elemento_crear() {
	elemento_t elemento;
	elemento.clave = NULL;
	elemento.dato = NULL;
	elemento.estado = VACIO;
	return elemento;
}


// Modifica los valores de un elemento
void elemento_mod(elemento_t *elemento, const char *clave, void *dato) {
    // char *clave_buff = malloc((strlen(clave) + 1) * sizeof(char));
    // if (!clave_buff) {
    //     free(elemento);
    //     return;
    // }
    // strcpy(clave_buff, clave);
    char *clave_buff = strdup(clave);
    if (!clave_buff) return;
    elemento->clave = clave_buff;
    elemento->dato = dato;
    elemento->estado = OCUPADO;
}

struct hash {
    size_t cap;
    size_t cant;
    elemento_t *arreglo;
    hash_destruir_dato_t destruccion;
};


// Recibe un arreglo de Elementos y una funcion de destruccion (o NULL)
// y los destruye
// Pre: existe un arreglo de elementos
// Post: el arreglo fue destruido, y sus datos si hubo funcion de destruccion 
void arreglo_destruir(elemento_t *arreglo, size_t tam, hash_destruir_dato_t destruir) {
    for (size_t i = 0; i < tam; i ++) {
		if (destruir && arreglo[i].estado == OCUPADO) destruir(arreglo[i].dato);   
        free(arreglo[i].clave);
    }
    free(arreglo);
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    elemento_t *vector = malloc(CAP * sizeof(elemento_t));
    if (!vector) {
        free(hash);
        return NULL;
    }
    for (int i = 0; i < CAP; i ++) {
		vector[i] = elemento_crear();
	}
    hash->arreglo = vector;
    hash->cap = CAP;
    hash->cant = 0;
    hash->destruccion = destruir_dato;
    return hash;
}


// Busca una posicion valida para guardar en la tabla hash y la devuelve
// Pre: existe una tabla hash y una clave por guardar
// Post: se devuelve un size_t con la posicion adecuada
size_t hash_posicion_valid_guardar(const hash_t *hash, const char *clave) {
    size_t pos = (size_t) crc32((unsigned char*)clave) % hash->cap;
    while (hash->arreglo[pos].estado == OCUPADO && strcmp(hash->arreglo[pos].clave, clave) != 0) {
        pos ++;
        if (pos >= hash->cap) pos = 0;
    }
    return pos;
}

// Cambia la capacidad de un hash a la recibida por parametro
bool hash_redimensionar(hash_t *hash, size_t cant_redimension) {
    elemento_t *nuevo_arreglo = malloc(cant_redimension * sizeof(elemento_t));
    if (!nuevo_arreglo) return false;
    for (int j = 0; j < cant_redimension; j ++) {
		nuevo_arreglo[j] = elemento_crear();
	}
    size_t cap_anterior = hash->cap;
    hash->cap = cant_redimension;
    elemento_t *arreglo_anterior = hash->arreglo;
    hash->arreglo = nuevo_arreglo;
    for (size_t i = 0; i < cap_anterior; i ++) {	
        if (arreglo_anterior[i].estado == OCUPADO) {
            size_t nueva_pos = hash_posicion_valid_guardar(hash, arreglo_anterior[i].clave);
            elemento_mod(&nuevo_arreglo[nueva_pos], arreglo_anterior[i].clave, arreglo_anterior[i].dato);
        }
    }
    arreglo_destruir(arreglo_anterior, cap_anterior, NULL);
    return true;
}

// Busca la posicion en la que se encuentra una clave en una tabla hash y la devuelve
// Pre: existe una tabla hash y una clave
// Post: se devuelve un size_t con la posicion adecuada
size_t hash_posicion_valid(const hash_t *hash, const char *clave) {
    size_t pos = (size_t) crc32((unsigned char*)clave) % hash->cap;
    while (hash->arreglo[pos].estado != VACIO && (hash->arreglo[pos].estado == BORRADO || strcmp(hash->arreglo[pos].clave, clave) != 0)) {
        pos ++;
        if (pos >= hash->cap) pos = 0;
    }
    return pos;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    size_t pos = hash_posicion_valid_guardar(hash, clave);
    if (hash_pertenece(hash, clave)) {
        if (hash->destruccion != NULL) hash->destruccion(hash->arreglo[pos].dato);
        hash->arreglo[pos].dato = dato;
        return true;
    }
    elemento_mod(&hash->arreglo[pos], clave, dato);
    hash->cant ++;
    float factor_carga = (float)hash->cant / (float)hash->cap;
    if (factor_carga >= CARGA_MAX) hash_redimensionar(hash, hash->cap * CTE_REDIMENSION);
    return true;
}


size_t hash_cantidad(const hash_t *hash){
    return hash->cant;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    if (!hash_pertenece(hash, clave)) return NULL;
    void *dato_actual = hash_obtener(hash, clave);
    size_t pos = hash_posicion_valid(hash, clave);
    hash->arreglo[pos].estado = BORRADO;
    hash->cant --;
    float factor_carga = (float)hash->cant / (float)hash->cap;
    if (factor_carga <= CARGA_MIN && hash->cap >= CAP) hash_redimensionar(hash, hash->cap / CTE_REDIMENSION);
    return dato_actual;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    if (!hash_pertenece(hash, clave)) return NULL;
    size_t pos = hash_posicion_valid(hash, clave);
    return hash->arreglo[pos].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t pos = hash_posicion_valid(hash, clave);
    return hash->arreglo[pos].estado == OCUPADO;
}
    
void hash_destruir(hash_t *hash) {
    arreglo_destruir(hash->arreglo, hash->cap, hash->destruccion);
    free(hash);
}

struct hash_iter {
    const hash_t *hash;    
    size_t actual;
};

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;
    iter->hash = hash;
    iter->actual = 0;
    while (iter->hash->arreglo[iter->actual].estado != OCUPADO) {
        iter->actual ++;
        if (hash_iter_al_final(iter)) return iter; 
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) return false; 
    iter->actual ++;
    if (hash_iter_al_final(iter)) return false; 
    while (iter->hash->arreglo[iter->actual].estado != OCUPADO) {
        iter->actual ++;
        if (hash_iter_al_final(iter)) return false; 
    }
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    return (!hash_iter_al_final(iter)) ? iter->hash->arreglo[iter->actual].clave : NULL;    
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->actual >= iter->hash->cap; 
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}
