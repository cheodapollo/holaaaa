/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "pdvsa.h"
#include "centro.h"
#include "md5.h"

pthread_mutex_t mutex_inventario = PTHREAD_MUTEX_INITIALIZER;

int *
pedir_tiempos_1_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	result = tiempo;
	printf("Tiempo es: %d \n",result);

	return &result;
}

int *
pedir_gasolina_1_svc(ticket *argp, struct svc_req *rqstp)
{
    static int  result;
    struct ticket aux;
    aux = *argp;
    int r;
    //printf("Valor de hora: %d",argp.hora);
    //PREGUNTAR URGENTE
    if (tiempo_mon - aux.hora <= 60){
      printf("mostrar el tiempo actual y el tiempo del ticket: %d , %d \n",tiempo_mon,aux.hora);
      if(inventario >= 38000){
        pthread_mutex_lock( &mutex_inventario );
        inventario -= 38000;
        pthread_mutex_unlock( &mutex_inventario );
        //pthread_mutex_lock( &mutex_log );
        fprintf(log_centro,"Evento en el tiempo %d:\n\tEl cliente solicita gasolina y el centro puede responder la peticion\n",tiempo_mon);
        fprintf(log_centro,"\tInventario actual del centro: %d\n",inventario);
        //pthread_mutex_unlock( &mutex_log );
        result = -1;
      }  
      else{
        //pthread_mutex_lock( &mutex_log );
        fprintf(log_centro,"Evento en el tiempo %d:\n\tEl cliente solicita gasolina y el centro no puede responder la peticion\n",tiempo_mon);
        fprintf(log_centro,"\tInventario actual del centro: %d\n",inventario);
        //pthread_mutex_unlock( &mutex_log );
        result = -2;    
      }
    }  
    else{
      printf("genero numero rand \n");
      int randNum;
      srand(time(NULL));
      randNum = rand () % (101) + 0; 
      result = randNum;
    }  
    return &result;
}

ticket *
validar_respuesta_1_svc(reto *argp, struct svc_req *rqstp)
{
	static ticket  result;
	char mensaje[80];
	char mensaje2[80];
	char mensaje3[80];
	sprintf( mensaje, "%d", argp->reto );
	unsigned *d = md5(mensaje, strlen(mensaje));
	sprintf( mensaje2, "%u", *(argp->respuesta) );
	sprintf( mensaje3, "%u", *d );
	printf("strcmp es %s %s\n",mensaje3,mensaje2);
	if(strcmp(mensaje3,mensaje2)==0){
	  printf("paso \n");
	  result.numero = 1;
	  result.ip_bomba = nombre;
	  result.hora = tiempo_mon;
	}  

	return &result;
}
