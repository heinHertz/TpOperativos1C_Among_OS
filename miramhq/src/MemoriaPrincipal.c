#include "MemoriaPrincipal.h"

void iniciarMemoria(char* esquemaMemoria){

    if (strcmp(esquemaMemoria,"SEGMENTACION") == 0)
    {
        iniciarMemoriaSegmentacion();
    }
    else if (strcmp(esquemaMemoria,"PAGINACION") == 0)
    {
        iniciarMemoriaPaginacion();
    }
    else
    {
        log_info(logger, "Esquema de memoria Invalido %s",esquemaMemoria);
    }

}

void iniciarMemoriaPaginacion(){
    
    void* puntero = NULL;

	puntero = puntero_memoria_principal;

	for(double i=0;  tamanioBitMapPrincipal> i; i++){

		bitarray_clean_bit(bitMap, i);

	}

    while(puntero + tamanioMemoria > puntero_memoria_principal){
		
		list_add(tablaFrames, puntero_memoria_principal);

		puntero_memoria_principal += tamanioPagina;

	}

	puntero_memoria_principal = puntero;

}
 
void iniciarMemoriaSegmentacion(){ //TODO: Inicializar memoria en segmentos
    

}

l_frame *frameLibre(){

    for(int i=0; tamanioBitMapPrincipal > i; i++){

		int bitOcupado = bitarray_test_bit(bitMap, i);
		void *punteroFrame = list_get(tablaFrames, i);

		if(!bitOcupado){
            l_frame* frameLibre = malloc(sizeof(l_frame));
            frameLibre->posicion_bitarray = i;
            frameLibre->punteroFrame = punteroFrame;
			return frameLibre;
		}

	}
	return NULL;

}

int calcularTamanioProceso(t_list* listaTareas,t_list* listaTripulantes){
    int tamanioListaTareas = list_size(listaTareas);
    int cantidad_tripulantes = list_size(listaTripulantes);
    int tamanioPCB = sizeof(t_pcb);
    int tamanioTCB = 21; // sizeof(t_tcb); TODO: Me devuelve 24 pero tiene que dar 21
    int tamanioTarea = sizeof(t_tarea);
    int tamanioProceso = tamanioPCB + (tamanioTCB * cantidad_tripulantes) + (tamanioListaTareas * tamanioTarea);
    return tamanioProceso;
}

int max (int x, int y)
{
if (x<y) {
return (y);
}  else  {
return (x);
}
}
