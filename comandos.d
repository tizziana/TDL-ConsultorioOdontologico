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
import multicola; 

enum string URGENTE = "URGENTE";
enum string REGULAR = "REGULAR";

void pedir_turno(string nombre_paciente, string urgencia, Multicola multicola) {
    if ((urgencia != URGENTE) && (urgencia != REGULAR)) {
        //fprintf(stdout, ENOENT_URGENCIA, urgencia); // MANEJO DE ERRORES
        return;
    }
    multicola.multicola_encolar(nombre_paciente, urgencia);
    fprintf(stdout, PACIENTE_ENCOLADO, nombre_paciente); //hace falta?
    size_t en_espera = multicola.multicola_cantidad();
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, en_espera); //hace falta?
}

void atender_paciente(Multicola multicola) {
    if (multicola.multicola_esta_vacia()) {
        fprintf(stdout, SIN_PACIENTES); //MANEJO DE ERRORES
        return;
    }
    string nombre_paciente = multicola.multicola_desencolar();
    fprintf(stdout, PACIENTE_ATENDIDO, nombre_paciente); //hace falta?
    size_t en_espera = multicola.multicola_cantidad(); 
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, en_espera); //hace falta?
}