#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TAM_INIT 10

struct heap {
    void** datos;
    size_t cant;
    size_t cap;
    cmp_func_t cmp;
};

//func auxiliar
void arrcpy(void* arr_original[], void* arr_cpy[], size_t n) {
    if (!n) return;
    for (int i = 0; i < n; i ++) {
        arr_cpy[i] = arr_original[i];
    }
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    
    heap->datos = malloc(TAM_INIT * sizeof(void*));
	if (heap->datos == NULL) {
		free(heap);
		return NULL;
	}
	heap->cant = 0;
	heap->cap = TAM_INIT;
	heap->cmp = cmp;
	return heap;
}

size_t calc_pos_padre(size_t hijo) {
    return (hijo - 1) / 2;
}

size_t calc_pos_hijo(size_t padre, bool izq) {
    if (izq) return 2 * padre + 1;
    return 2 * padre + 2;
}

void swap(void** arreglo, size_t pos1, size_t pos2) {
    void* aux = arreglo[pos2];
    arreglo[pos2] = arreglo[pos1];
    arreglo[pos1] = aux; 
}

void upheap(void **arreglo, size_t hijo, cmp_func_t cmp) {
    if (hijo == 0) return;
    size_t padre = calc_pos_padre(hijo);
    if (cmp(arreglo[padre], arreglo[hijo]) < 0) {
        swap(arreglo, padre, hijo);
        upheap(arreglo, padre, cmp);
    }
}

size_t maximo(void **arreglo, cmp_func_t cmp, size_t padre, size_t h_izq, size_t h_der) {
    if (cmp(arreglo[padre], arreglo[h_izq]) >= 0 && cmp(arreglo[padre], arreglo[h_der]) >= 0) return padre;
    if (cmp(arreglo[h_izq], arreglo[h_der]) > 0) {
        return h_izq;
    }
    else {
        return h_der;
    }
}

void downheap(void **arreglo, size_t tam, size_t padre, cmp_func_t cmp) {
    if (padre == tam) return;
    if (padre == tam - 1) return;
    size_t h_izq = calc_pos_hijo(padre, true);
    size_t h_der = calc_pos_hijo(padre, false);
    size_t max;
    if (h_izq < tam && h_der < tam) {
        max = maximo(arreglo, cmp, padre, h_izq, h_der);
    } else if (h_izq < tam) {
        max = (cmp(arreglo[padre], arreglo[h_izq]) >= 0) ? padre : h_izq; 
    } else {
        max = padre;
    }
    if (max != padre) {
        swap(arreglo, padre, max);
        downheap(arreglo, tam, max, cmp);
    }
}

void heapify(void *elementos[], size_t cant, cmp_func_t cmp) {
    for (int i = (int)cant - 1; i >= 0; i --) {
        downheap(elementos, cant, (size_t)i, cmp);
    }    
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    if (n == 0) {
        return heap_crear(cmp);
    }
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    void *nuevo_arreglo = malloc(2 * n * sizeof(void*));
    if (!nuevo_arreglo) {
        free(heap);
        return NULL;
    }
	heap->cmp = cmp;
    heap->cap = 2 * n;
    heap->cant = n;
    arrcpy(arreglo, nuevo_arreglo, n);
    heapify(nuevo_arreglo, n, cmp);
    heap->datos = nuevo_arreglo;
    return heap;
}

bool heap_redim(heap_t *heap, size_t nueva_cap) {
	void* datos_nuevo = realloc(heap->datos, nueva_cap * sizeof(void*));
	if (datos_nuevo == NULL) return false;

	heap->datos = datos_nuevo;
	heap->cap = nueva_cap;
	return true;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    for (int i = 0; i < heap->cant; i ++) {
        if (destruir_elemento) destruir_elemento(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {
    return !heap_cantidad(heap);
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

#define CTE_REDIMENSION 2
#define CTE_ACHICAMIENTO 4

bool heap_encolar(heap_t *heap, void *elem){
    if (heap->cant == heap->cap) {
        bool redimension = heap_redim(heap, heap->cap * CTE_REDIMENSION);
        if (!redimension) return false;
    }
    heap->datos[heap->cant] = elem;
    upheap(heap->datos, heap->cant, heap->cmp);
	heap->cant++;
	return true;
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    void *dato = heap->datos[0];
    swap(heap->datos, 0, heap->cant - 1);
    downheap(heap->datos, heap->cant - 1, 0, heap->cmp);
    heap->cant --;
    if ((heap->cant * CTE_ACHICAMIENTO) <= heap->cap && heap->cap > TAM_INIT) {
        heap_redim(heap, heap->cap / CTE_REDIMENSION);
    }
    return dato;
}

void _heap_sort(void* elementos[], size_t cant, cmp_func_t cmp) {
    if (!cant) return;
    size_t max = 0;
    swap(elementos, max, cant - 1);
    downheap(elementos, cant - 1, 0, cmp);
    _heap_sort(elementos, cant - 1, cmp);
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    _heap_sort(elementos, cant, cmp);
}