/*		pdvsa.x			*/

/* OJO Se podria poner el tipo time de C */
struct ticket {
        int numero;
        char* ip_bomba ;
	int hora; 
}
;
struct reto {
        unsigned* respuesta;
        int reto;
}
;

program PDVSA_PROG {
	version PDVSA_VERS {
		int PEDIR_TIEMPOS(int)=1;
		int PEDIR_GASOLINA(ticket)=2;
		ticket VALIDAR_RESPUESTA(reto)=3;
	}=1;
}= 0x30810272;