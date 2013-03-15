/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "pdvsa.h"
#include "centro.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#define PULL_HILOS 5

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

/*Declaracion de los mutex sobre la variables globales*/
//pthread_mutex_t mutex_inventario = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

static void
pdvsa_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		int pedir_tiempos_1_arg;
		ticket pedir_gasolina_1_arg;
		reto validar_respuesta_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case PEDIR_TIEMPOS:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) pedir_tiempos_1_svc;
		break;

	case PEDIR_GASOLINA:
		_xdr_argument = (xdrproc_t) xdr_ticket;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) pedir_gasolina_1_svc;
		break;

	case VALIDAR_RESPUESTA:
		_xdr_argument = (xdrproc_t) xdr_reto;
		_xdr_result = (xdrproc_t) xdr_ticket;
		local = (char *(*)(char *, struct svc_req *)) validar_respuesta_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

/*Funcion que se encarga de guardar la informacion del centro */
char* leer_flags(int argc, char** argv){
  int c;
  char* fichero;
  while (1) {
    int aux = 0;
    static struct option long_options[] = {
      {"cp", 1, 0, 'q'},
      {0, 0, 0, 0}
    };
    c = getopt_long_only(argc, argv, "t:s:n:i:",long_options, &aux);
    if (c == -1)
      break;
    /* Casos del tipo de flags */
    switch (c) {
    case 'n':
      nombre =  optarg;
      break;
    case 's':
      suministro =  atoi(optarg);
      break;
    case 'q':
      capacidad = atoi(optarg);
      break;
    case 'i':
      inventario = atoi(optarg);
      break;
    case 't':
      tiempo = atoi(optarg);
      break;
    }
  }
  return fichero;
}

/* Funcion que se encarga de leer la linea de codigo del terminal */
void leer_entrada(int argc, char** argv){
  if (argc != 11) {
    fprintf(stderr, "Entrada invalida, faltan flags\n");
    exit(0);
  }
  else{
    char* fichero = leer_flags(argc,argv);   
  }
}

/*Procediomiento que ejecuta el hilo que se encarga de monitorear el tiempo que
  ha transcurrido para terminar la simulacion cuando corresponda*/
void * monitor_tiempo(void *param){
  tiempo_mon = 0;
  int unsleep;
  while (tiempo_mon <= 600) {
    if ((inventario + suministro) < capacidad){
      //pthread_mutex_lock( &mutex_inventario );
      inventario += suministro;  
      //pthread_mutex_unlock( &mutex_inventario );
      pthread_mutex_lock( &mutex_log );
      fprintf(log_centro,"Evento en el tiempo %d:\n\tEl centro recibe suministro de la refineria\n",tiempo_mon);
      fprintf(log_centro,"\tInventario actual del centro: %d\n",inventario);
      pthread_mutex_unlock( &mutex_log);
    }
    else{
      inventario = capacidad;
    }  

    usleep (100000);
    ++tiempo_mon;
    printf("tiempo es: %d \n",tiempo_mon);
    //printf("El inventario es : %d \n",inventario);
  }
  printf("Se ha termiando la simulacion del centro\n");
  exit(0);
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;
	leer_entrada(argc, argv);
	char documento [50];
	strcpy(documento, "log_");
	strcat(documento, nombre);
	strcat(documento, ".txt");
	log_centro = fopen(documento,"w+");
	printf("--------------INICIO DE SIMULACION-------------\n" ); 
	printf("* Nombre del centro: %s\n",nombre);
	printf("* Capacidad maxima del centro: %d\n",capacidad);
	printf("* Inventario del centro: %d\n",inventario);
	printf("* Tiempo del centro: %d\n",tiempo);
	printf("* Suministro del centro: %d\n",suministro);
	
	pthread_t monitor;
		
	int arg[1];
	if ((pthread_create(&monitor, NULL, &monitor_tiempo, (void *) arg)) != 0) 
		perror("Error!, fallo creacion del hilo monitor del tiempo de simulacion.\n");
	//aceptar_conexiones();

	pmap_unset (PDVSA_PROG, PDVSA_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, PDVSA_PROG, PDVSA_VERS, pdvsa_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (PDVSA_PROG, PDVSA_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, PDVSA_PROG, PDVSA_VERS, pdvsa_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (PDVSA_PROG, PDVSA_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
