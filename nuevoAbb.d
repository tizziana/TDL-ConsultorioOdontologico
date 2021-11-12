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

class Nodo{
    public Nodo der;
    public Nodo izq;
    public string clave;
    public int valor;

    this(string clave, int valor){
        this.clave = clave;
        this.valor = valor;
        this.der = null;
        this.izq = null;
    }
}

class Abb{
    public Nodo raiz;
    public int cant;
    this(){
       this.raiz = null;
       this.cant = 0;
    }

    private void _agregarElemento(Nodo actual, Nodo nuevo){
        if(nuevo.clave < actual.clave){
            if(!actual.izq){
                actual.izq = nuevo;
                return;
            }
            _agregarElemento(actual.izq, nuevo);
        }else{
            if(!actual.der){
                actual.der = nuevo;
                return;
            }
            _agregarElemento(actual.der, nuevo);
        }

    }

    private Nodo _buscarNodo(Nodo actual, string clave){
        if(!actual) return null;
        if(clave == actual.clave) return actual;
        if(clave < actual.clave) return _buscarNodo(actual.izq, clave);
        else return _buscarNodo(actual.der, clave);
    }

    private Nodo _buscarReemplazante(Nodo actual){
        //if(!actual.izq) return actual;
        //return _buscarReemplazante(actual.izq);

        return (!actual.izq) ? actual : _buscarReemplazante(actual.izq);
    }

    private int _borrarElemento(Nodo padre, Nodo hijo, string clave){
        if(!hijo) return -1;

        if(clave == hijo.clave){

            if(!hijo.izq && !hijo.der){

                if(padre && padre.izq && padre.izq.clave == hijo.clave) padre.izq = null;
                else if(padre && padre.der && padre.der.clave == hijo.clave) padre.der = null;
                else if(!padre) this.raiz = null;

                return hijo.valor;
            }

            else if((!hijo.izq && hijo.der) || (!hijo.der && hijo.izq)){
                Nodo seleccionado;

                if(hijo.izq) seleccionado = hijo.izq;
                else seleccionado = hijo.der;

                if(padre && padre.izq && padre.izq.clave == hijo.clave) padre.izq = seleccionado;
                else if(padre && padre.der && padre.der.clave == hijo.clave) padre.der = seleccionado;
                else if(!padre) this.raiz = seleccionado;

                return hijo.valor;
            }

            else if(hijo.izq && hijo.der){
                Nodo reemplazante = _buscarReemplazante(hijo.der);
                _borrarElemento(null, this.raiz, reemplazante.clave);

                int devolver = hijo.valor;
                hijo.clave = reemplazante.clave;
                hijo.valor = reemplazante.valor;
                return devolver;
            }
        }
        if(clave < hijo.clave) return _borrarElemento(hijo, hijo.izq, clave);
        else return _borrarElemento(hijo, hijo.der, clave);
    }

    public bool agregarElemento(string clave, int valor){
        // FALTA VER SI YA EXISTE
        Nodo nuevo = new Nodo(clave, valor);

        if(!this.raiz) this.raiz = nuevo;
        else _agregarElemento(this.raiz, nuevo);
        
        this.cant++;
        return true;
    }

    public bool pertenece(string clave){
        return _buscarNodo(this.raiz, clave) ? true : false;
    }



    public int borrarElemento(string clave){
        this.cant--;
        return _borrarElemento(null, this.raiz, clave);
    }
    
}

void main(string[] args)
{
    Abb unAbb = new Abb();
    unAbb.agregarElemento("B", 1);
    unAbb.agregarElemento("A", 2);
    unAbb.agregarElemento("C", 3);

    writeln("Pertenece B ",unAbb.pertenece("B"));
    writeln("Pertenece A ",unAbb.pertenece("A"));
    writeln("Pertenece C ",unAbb.pertenece("C"));

    writeln("Borrar C ",unAbb.borrarElemento("C"));

    writeln("Pertenece B ",unAbb.pertenece("B"));
    writeln("Pertenece A ",unAbb.pertenece("A"));
    writeln("Pertenece C ",unAbb.pertenece("C"));
}