#include "MemoriaPrincipal.h"


void iniciarMemoriaSwap(){
    
	/*
	//TODO: no tengo permiso para crear en esat ruta 
	char comando [100 + 1] = "cat /dev/null > ";
	strcat(comando,pathSwap);
    archivoSwap = open(pathSwap,O_CREAT, O_RDWR, S_IRUSR | S_IWUSR);
    system(comando);
	*/

    archivoSwap = open("./swap.txt",O_CREAT, O_RDWR, S_IRUSR | S_IWUSR);
    system("cat /dev/null > ./swap.txt");

    void* puntero = NULL;

    lseek(archivoSwap, tamanioSwap - 1, SEEK_SET);

    write(archivoSwap, "", 1);

    puntero_memoria_swap = mmap(NULL, tamanioSwap, PROT_READ | PROT_WRITE, MAP_SHARED, archivoSwap, 0);

	puntero = puntero_memoria_swap;

    for(int i=0; tamanioBitMapSwap > i; i++){

		bitarray_clean_bit(bitMapSwap, i);

	}

    while(puntero + tamanioSwap > puntero_memoria_swap){
		
		list_add(tablaSwap, puntero_memoria_swap);

        puntero_memoria_swap += tamanioPagina;

	}

    puntero_memoria_swap = puntero;


}
/* 
void *ejecutarAlgoritmo(){

	void* framelibre = frameLibre();

	if(framelibre != NULL) 
		return framelibre;

	char string_log[100];
    sprintf(string_log, "Ejecutando algoritmo: %s", algoritmo);
    log_info(logger, string_log);
	
	if(!strcmp(algoritmo, "LRU")){
		void* direccion = ejecutarLRU();
		return direccion;
	}
	if(!strcmp(algoritmo, "CLOCK_MEJ")){
		void* direccion = ejecutarClockMej();
		return direccion;
	}
	
	printf("ALGORITMO INVALIDO\n");

	return NULL;
}


void* ejecutarLRU(){
	l_pagina* pagina = popfrontlist(pilaPaginasAlgoritmos);
	l_frame* frame = pagina->frame;
	char string_log[200];
    sprintf(string_log, "Victima seleccionada: %s | Direccion principal: %p | Direccion virtual: %p ", frame->punteroFrame, pagina->frame, pagina->swap);
    log_info(logger, string_log);
	return desalojarDePrincipal(pagina);

}

 
void *ejecutarClockMej(){
	
	l_pagina* pagina = NULL;
	//IteratorList iterador = NULL;
	int gradoDeBusqueda = 0; //0 = bitUso en 0 + bitModificado en 0 | 1 = bitUso en 0 + bitModificado en 1

	if(inicioClockMej == 1){
		iteradorClockMej = beginlist(pilaPaginasAlgoritmos);
		inicioClockMej = 0;
	}

	iterador = iteradorClockMej;

	while(iterador != NULL){
	
		pagina = (l_pagina*)dataiterlist(iterador);
		l_frame* frame = pagina->frame;

		if(gradoDeBusqueda){
			if(!pagina->bitUso){
				if(nextlist(iterador) == NULL){
					iteradorClockMej = beginlist(pilaPaginasAlgoritmos);
				} else iteradorClockMej = nextlist(iterador);
				popiterlist(&pilaPaginasAlgoritmos, iterador);
				char string_log[200];
    			sprintf(string_log, "Victima seleccionada: %s | Direccion principal: %p | Direccion virtual: %p ", frame->punteroFrame, pagina->frame, pagina->swap);
    			log_info(logger, string_log);
				return desalojarDePrincipal(pagina);
			} else {
				pagina->bitUso = 0;
			}
		}

		if(!gradoDeBusqueda){
			if(!pagina->bitUso){
				if(!pagina->bitModificado){
					if(nextlist(iterador) == NULL){
						iteradorClockMej = beginlist(pilaPaginasAlgoritmos);
					} else iteradorClockMej = nextlist(iterador);
					popiterlist(&pilaPaginasAlgoritmos, iterador);
					char string_log[200];
    				sprintf(string_log, "Victima seleccionada: %s | Direccion principal: %p | Direccion virtual: %p ", frame->punteroFrame, pagina->frame, pagina->swap);
    				log_info(logger, string_log);
					return desalojarDePrincipal(pagina);
				}
			}
		}

		iterador = nextlist(iterador);

		if(iterador == NULL){
			iterador = beginlist(pilaPaginasAlgoritmos);
			gradoDeBusqueda = !gradoDeBusqueda;
		}
	
	}
	printf("\nError ejecutando el algoritmo!!");
	return NULL;
}
*/