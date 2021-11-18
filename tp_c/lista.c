#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo *prox;
} nodo_t;

nodo_t *lista_nodo_crear(void* dato) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        return NULL;
    }
    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}

struct lista {
    struct nodo* prim;
    struct nodo* ult;
    size_t largo;
};

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (lista == NULL) return NULL;
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nuevo = lista_nodo_crear(dato);
    if (nuevo == NULL) return false;

    if (lista->prim != NULL) {
        nuevo->prox = lista->prim;
    }
    else {
        lista->ult = nuevo;
    }
    lista->prim = nuevo;
    lista->largo ++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nuevo = lista_nodo_crear(dato);
    if (nuevo == NULL) return false;

    if (lista->prim == NULL) {
        lista->prim = nuevo;
    }
    else {
        lista->ult->prox = nuevo;
    }
    lista->ult = nuevo;
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista->prim == NULL) return NULL;
    
    nodo_t *eliminado = lista->prim;
    if (lista->prim == lista->ult) {
        lista->ult = lista->prim->prox;
    } 
    lista->prim = lista->prim->prox;
    lista->largo --;
    void *dato_eliminado = eliminado->dato;
    free(eliminado);
    return dato_eliminado;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista->largo == 0) return NULL;
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (lista->largo == 0) return NULL;
    return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    nodo_t *actual = lista->prim;
    while (actual != NULL) {
        if (destruir_dato != NULL) {
            destruir_dato(actual->dato);
        }
        nodo_t *prox = actual->prox;
        free(actual);
        actual= prox;       
    }
    free(lista);
}

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra) {
    if (lista->prim == NULL) return;
    nodo_t *actual = lista->prim;
    bool estado = true;
    while (actual != NULL && estado == true) {
        void *dato = actual->dato;
        estado = visitar(dato, extra);
        actual = actual->prox;
    }
}

struct lista_iter {
    nodo_t *anterior;
    nodo_t *actual;
    lista_t *lista;
};

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    if (iter == NULL) return NULL;

    iter->anterior = NULL;
    iter->actual = lista->prim;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (iter->actual == NULL) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (iter->actual == NULL) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    if (iter->actual == NULL) return true;
    return false;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nuevo = lista_nodo_crear(dato);
    if (nuevo == NULL) return false;
    if (iter->actual == NULL) {
        iter->lista->ult = nuevo;
    }
    if (iter->actual == iter->lista->prim) {
        iter->lista->prim = nuevo;
    }
    if (iter->anterior != NULL) {
        iter->anterior->prox = nuevo;
    }
    nuevo->prox = iter->actual;
    iter->actual = nuevo;
    iter->lista->largo ++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (iter->actual == NULL) return NULL; 
    nodo_t *proximo = iter->actual->prox;
    if (iter->actual == iter->lista->prim) {
        iter->lista->prim = proximo;
    }
    if (iter->actual == iter->lista->ult) {
        iter->lista->ult = iter->anterior;
    }
    if (iter->anterior != NULL) {
        iter->anterior->prox = proximo;
    }
    void *dato = iter->actual->dato;
    nodo_t *borrar = iter->actual;
    free(borrar);
    iter->actual = proximo;
    iter->lista->largo --;
    return dato;
}