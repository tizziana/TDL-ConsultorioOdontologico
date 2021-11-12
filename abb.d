//#define _POSIX_C_SOURCE 200809L;
import abb;
import std.stdio;
import std.conv;
import std.math;
import std.random;
import std.datetime;
import std.algorithm;
import std.array;
import std.string;
import std.format;
import std.typecons : No;
import std.typecons;
import std.range : chain;
import std.file;
import core.thread;
import std.parallelism;
import std.concurrency;
import std.range;
//import std.typecons;
//import std.stdio;
// #include <stdlib.h>
//import std.string;
//import pila;
// #include <stdio.h>
///
struct abb_nodo {
    string clave;
    void dato;
}abb_nodo_t;
typedef struct abb_nodo abb_nodo_t;

abb_nodo_t* abb_nodo_crear(const char *clave, void *dato){
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;

	char *clave_nodo = strdup(clave);
    if (!clave_nodo) {
		free(nodo);
		return NULL;
	}
	nodo.clave = clave_nodo;

	nodo.dato = dato;
	nodo.izq = NULL;
	nodo.der = NULL;
	return nodo;
}

void abb_nodo_destruir(abb_nodo_t *nodo, abb_destruir_dato_t destruir_dato) {
	if (destruir_dato) destruir_dato(nodo.dato);
	free(nodo.clave);
	free(nodo);
}

void _abb_nodos_destruir(abb_nodo_t *nodo, abb_destruir_dato_t destruir_dato) {
	if (!nodo) return;
	if (nodo.izq) _abb_nodos_destruir(nodo.izq, destruir_dato);
	if (nodo.der) _abb_nodos_destruir(nodo.der, destruir_dato);
	abb_nodo_destruir(nodo, destruir_dato);
}

struct abb {
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant;
}abb_t;
typedef struct abb abb_t;


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;
	abb.raiz = NULL;
	abb.cmp = cmp;
	abb.destruir_dato = destruir_dato;
	abb.cant = 0;
	return abb;
}

abb_nodo_t *nodo_hallar(const abb_t *arbol, abb_nodo_t* nodo, const char *clave) {
	if (!nodo) return NULL;
	if (arbol.cmp(clave, nodo.clave) == 0) return nodo;
	if (arbol.cmp(clave, nodo.clave) < 0) {
		return nodo_hallar(arbol, nodo.izq, clave);
	} else {
		return nodo_hallar(arbol, nodo.der, clave);
	}
}

void _abb_guardar(abb_t *arbol, abb_nodo_t* nodo_act, abb_nodo_t* nodo_guar){
	if (arbol.cmp(nodo_guar.clave, nodo_act.clave) < 0) {
		if (!nodo_act.izq) {
			nodo_act.izq = nodo_guar;
			return;
		} 
		_abb_guardar(arbol, nodo_act.izq, nodo_guar);
	} else {
		if (!nodo_act.der) {
			nodo_act.der = nodo_guar;
			return;
		} 
		_abb_guardar(arbol, nodo_act.der, nodo_guar);
	}
} 

void _abb_cambiar_dato(abb_t *arbol, const char *clave, void *dato) {
	abb_nodo_t *nodo = nodo_hallar(arbol, arbol.raiz, clave);
	if (arbol.destruir_dato) arbol.destruir_dato(nodo.dato); 
	nodo.dato = dato;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	if (abb_pertenece(arbol, clave)) {
		_abb_cambiar_dato(arbol, clave, dato);
		return true;
	}
	abb_nodo_t* nodo = abb_nodo_crear(clave, dato);
	if (!nodo) return false;
	
    if (!arbol.raiz) {
        arbol.raiz = nodo;
    } else {
        _abb_guardar(arbol, arbol.raiz, nodo);
    }
    
	arbol.cant++;
	return true;
}

void nodo_swap(abb_nodo_t* nodo_1,abb_nodo_t* nodo_2) {
	char* clave_aux = nodo_1.clave;
	void* dato_aux = nodo_1.dato;
	nodo_1.clave = nodo_2.clave;
	nodo_1.dato = nodo_2.dato;
	nodo_2.clave = clave_aux;
	nodo_2.dato = dato_aux;
}

abb_nodo_t *_abb_hallar_padre(abb_t *arbol, abb_nodo_t* nodo, const char *clave) {
	if ((nodo.izq && arbol.cmp(clave, nodo.izq.clave) == 0) || (nodo.der && arbol.cmp(clave, nodo.der.clave) == 0)) return nodo;
	if (arbol.cmp(clave, nodo.clave) < 0) {
		return _abb_hallar_padre(arbol, nodo.izq, clave);
	} else {
		return _abb_hallar_padre(arbol, nodo.der, clave);
	}
}

abb_nodo_t *_abb_sig_in_padre(abb_t *arbol, abb_nodo_t* nodo_act) {
	if (!nodo_act) return NULL;
	if (!nodo_act.izq) return nodo_act;
	if (!nodo_act.izq.izq) return nodo_act;
	return _abb_sig_in_padre(arbol, nodo_act.izq);
}

void _nodo_borrar(abb_t *arbol, abb_nodo_t *nodo, abb_nodo_t *nodo_padre, abb_nodo_t *reemplazo, bool izq){
	if (!nodo_padre) {
		arbol.raiz = reemplazo; 
		abb_nodo_destruir(nodo, NULL);
		return;
	}
	if (izq) {
		nodo_padre.izq = reemplazo;
	} else {
		nodo_padre.der = reemplazo;
	}
	
	abb_nodo_destruir(nodo, NULL);
	return;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	if (!abb_pertenece(arbol, clave)) return NULL;
    abb_nodo_t *nodo_padre;
    abb_nodo_t *nodo;
    bool izq = true;
    if (arbol.cmp(arbol.raiz.clave, clave) == 0) {
        nodo_padre = NULL;
        nodo = arbol.raiz;
    } else {
        nodo_padre = _abb_hallar_padre(arbol, arbol.raiz, clave);
        izq = (arbol.cmp(clave, nodo_padre.clave) < 0);
        nodo = izq? nodo_padre.izq : nodo_padre.der;
    }
	void* dato = nodo.dato; 
	if (!nodo.izq && !nodo.der) {
        _nodo_borrar(arbol, nodo, nodo_padre, NULL, izq);
	} else if (!nodo.der) {
		_nodo_borrar(arbol, nodo, nodo_padre, nodo.izq, izq);
	} else if (!nodo.izq) {
		_nodo_borrar(arbol, nodo, nodo_padre, nodo.der, izq);
	} else {
		abb_nodo_t *nodo_intercambiar = nodo.der;
		abb_nodo_t *nodo_padre_swap = _abb_sig_in_padre(arbol, nodo_intercambiar);
		if (nodo_intercambiar.izq) {
			nodo_intercambiar = nodo_padre_swap.izq;
			nodo_swap(nodo_intercambiar, nodo);
			_nodo_borrar(arbol, nodo_intercambiar, nodo_padre_swap, nodo_intercambiar.der, true);
		} else {
			nodo_padre_swap = nodo;
			nodo_swap(nodo_intercambiar, nodo);
			_nodo_borrar(arbol, nodo_intercambiar, nodo_padre_swap, nodo_intercambiar.der, false);
		}
	}
    arbol.cant--;
	return dato;
}


void *abb_obtener(const abb_t *arbol, const char *clave) {
	if (!abb_pertenece(arbol, clave)) return NULL;
	abb_nodo_t *nodo = nodo_hallar(arbol, arbol.raiz, clave);
	return nodo.dato;
}


bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return (nodo_hallar(arbol, arbol.raiz, clave)) ? true : false;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol.cant;
}

void abb_destruir(abb_t *arbol) {
	_abb_nodos_destruir(arbol.raiz, arbol.destruir_dato);
	free(arbol);
}


// ITERADOR INTERNO //
bool function(const char *,void *,void *) visitar_t;

void _abb_in_order(abb_nodo_t *nodo, visitar_t visitar, void* extra, bool *seguir_iterando) {
    if (!nodo) return;
    _abb_in_order(nodo.izq, visitar, extra, seguir_iterando);
	if (!(*seguir_iterando)) return;
	*seguir_iterando = visitar(nodo.clave, nodo.dato, extra);
    _abb_in_order(nodo.der, visitar, extra, seguir_iterando);
}

void abb_in_order(abb_t *arbol, visitar_t visitar, void *extra) { 
	bool seguir_iterando = true;
	_abb_in_order(arbol.raiz, visitar, extra, &seguir_iterando);
}

/*
// ITERADOR EXTERNO //
struct abb_iter {
	const abb_t* arbol;
	pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;
	pila_t* pila = pila_crear();
	if (!pila) {
		free(iter);
		return NULL;
	}
	if (arbol.raiz) {
		abb_nodo_t* actual = arbol.raiz;
		int contador = 0;
		while (actual) {
			pila_apilar(pila, actual);
			contador ++;
			actual = actual.izq;
		}
	}
	iter.arbol = arbol;
	iter.pila = pila; 
	return iter;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter.pila);
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter)) return NULL;
    abb_nodo_t* tope = cast(abb_nodo_t*)pila_ver_tope(iter.pila);
	return tope.clave;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;
	abb_nodo_t* nodo = cast(abb_nodo_t*)pila_desapilar(iter.pila);
	if (nodo.der) {
        abb_nodo_t *actual = nodo.der;
        int cont = 0;
		while (actual) {
            pila_apilar(iter.pila, actual);
			cont ++;
			actual = actual.izq;
        }
    }
	return true;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter.pila);
	free(iter);
}

void _abb_in_order_inicio(abb_nodo_t *nodo, visitar_t visitar, void* extra, bool *seguir_iterando, const char* ini) {
    if (!nodo) return;
	if (strcmp(nodo.clave, ini) > 0) {
		_abb_in_order_inicio(nodo.izq, visitar, extra, seguir_iterando, ini);
	}
	if (!(*seguir_iterando)) return;
	if (strcmp(nodo.clave, ini) >= 0) {
		*seguir_iterando = visitar(nodo.clave, nodo.dato, extra);
	}
    _abb_in_order_inicio(nodo.der, visitar, extra, seguir_iterando, ini);
}

abb_nodo_t* buscar_padre(abb_t* arbol, abb_nodo_t* nodo, const char* ini) {
	if (!nodo) return NULL;
	if (strcmp(nodo.clave, ini) >= 0) return nodo;
	return buscar_padre(arbol, nodo.der, ini);
}

void abb_in_order_inicio(abb_t* arbol, visitar_t visitar, void* extra, const char* inicio) {
	if (!arbol) return;
	abb_nodo_t* mayor = buscar_padre(arbol, arbol.raiz, inicio);
	if (!mayor) return;
	bool seguir_iterando = true;
	_abb_in_order_inicio(mayor, visitar, extra, &seguir_iterando, inicio);
}*/