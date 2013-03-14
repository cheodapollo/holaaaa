#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#define PULL_HILOS 5

/*Archivo para escribir el log del centro*/
FILE* log_centro;

/*Datos sobre el servidor*/
char* nombre;
int capacidad;
int inventario;
int tiempo;
int suministro;
int tiempo_mon;
//int puerto;
int ocupado[PULL_HILOS];


/*Declaracion de las funciones del centro /
char* leer_flags(int, char**);
void imprimir_datos();
*/

/*Declaracion de los mutex sobre la variables globales*/
// pthread_mutex_t mutex_inventario = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

