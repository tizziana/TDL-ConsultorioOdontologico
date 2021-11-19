import std.stdio;
import std.algorithm;
import std.conv:to;
import core.stdc.stdlib;
import std.conv;
import std.format;
import std.typecons;
import std.array;
import std.string;
import std.typecons : No;
import std.typecons;
import std.range; 
import std.parallelism;
import cola;

// class Multicola {
//     cola_t* cola_prioritaria;
//     cola_t* cola_regular;

//     //unnitests

//     this() {
//         this.cola_prioritaria = cola_crear();
//         this.cola_regular = cola_crear();
//     }

    // void multicola_encolar_prioritario(string paciente) {
    //     cola_encolar(this.cola_prioritaria, paciente);
    // }

    // void multicola_encolar_regular(string paciente) {
    //     cola_encolar(this.cola_regular, paciente);
    // }

    // string multicola_desencolar() {
    //     if (multicola_esta_vacia()) {
    //         return null;
    //     }
    //     if (!cola_esta_vacia(this.cola_prioritaria)) {
    //         return cola_desencolar(this.cola_prioritaria);
    //     }
    //     return cola_desencolar(this.cola_regular);
    // }

    // int multicola_cantidad() {
    //     return (cola_cantidad(this.cola_prioritaria) + cola_cantidad(this.cola_regular));
    // }

    // bool multicola_esta_vacia() {
    //     return !multicola_cantidad();
    // }

    // string multicola_ver_primero() {
    //     if (multicola_esta_vacia()) {
    //         return null;
    //     }
    //     if (cola_esta_vacia(this.cola_prioritaria) == false) {
    //         return cola_ver_primero(this.cola_prioritaria);
    //     }
    //     return cola_ver_primero(this.cola_regular);
    // }
// }


int main() {
    // Multicola multicola = new Multicola();
    string num1 = "fede";
    string num2 = "flor";
    cola_t* cola = cola_crear();
    cola_encolar(cola, num1);
    // multicola.multicola_encolar_prioritario(num1);
    // multicola.multicola_encolar_prioritario(num2);
    // writeln(multicola.multicola_cantidad());

    // string desencolar = multicola.multicola_desencolar();
    // writeln(desencolar);

    return 0;
}