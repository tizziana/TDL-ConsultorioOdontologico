#include "multicolas.h"
#include "hash.h"
#include "abb.h"

// Encola al paciente en su respectiva especialidad, si se encuentra en el sistema.
// Pre: la multicola y el hash fueron previamente creados.
// Post: el paciente fue encolado.
void pedir_turno(char* nombre_paciente, char* especialidad, char* urgencia, hash_t* pacientes, multicolas_t* multicolas);

// Desencola un paciente de la especialidad del doctor, si este esta en el sistema, y lo atiende segun prioridad.
// Pre: el arbol y la multicola fueron previamente creados.
// Post: se desencolo un paciente de la especialidad del doctor.
void atender_paciente(const char* nombre_doctor, abb_t* doctores, multicolas_t* colas);

// Imprime un informe con los doctores cuyos nombes se encuentren entre los caracteres recibidos por parametro (NULL para que no se marque limite).
// Pre: el arbol de doctores fue previamente creado.
// Post: Imprime en la consola el informe de los doctores. 
void crear_informe(abb_t* doctores, char* inicio, char* final);