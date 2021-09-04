#include "miramhq.h"

int main(){
    logger = log_create("./cfg/miramhq.log", "MIRAMHQ", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el miramhq ! %s", mi_funcion_compartida());

    miramhq_init(&miramhq_config);    
     
    tamanioMemoria = miramhq_config->tamanio_memoria;
    esquemaMemoria = miramhq_config->esquema_memoria;
    tamanioPagina = miramhq_config->tamanio_pagina;
    tamanioSwap = miramhq_config->tamanio_swap;
    pathSwap = miramhq_config->path_swap;
    algoritmo = miramhq_config->algoritmo_reemplazo;
    criterioSeleccion = miramhq_config->criterio_seleccion;
    puerto = miramhq_config->puerto_escucha;

 
    puntero_memoria_principal = malloc(miramhq_config->tamanio_memoria); //Reserva espacio de memoria principal

    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        tablaFrames = list_create();
        tablaDeProcesos = list_create(); 
        tablaSwap = list_create();
        //Creo birarray -> Indica frames libres 2048bytes/64bytes= 32Paginas
        tamanioBitMapPrincipal = (tamanioMemoria/tamanioPagina);
	    punteroBitMap = malloc(max(1, tamanioBitMapPrincipal));
        bitMap = bitarray_create_with_mode(punteroBitMap, max(1,tamanioBitMapPrincipal/8), MSB_FIRST);

        tamanioBitMapSwap = (tamanioSwap/tamanioPagina);
        punteroBitMapSwap = malloc(max(1,tamanioBitMapSwap));
        bitMapSwap = bitarray_create_with_mode(punteroBitMapSwap, max(1,tamanioBitMapSwap/8), MSB_FIRST);

        inicioClockMej = 1;
        
        iniciarMemoriaSwap();
    }   
    else
    {
        tabla_segmentos = list_create();
		id_segmento = 1;
    }

    iniciarMemoria(esquemaMemoria);

    sem_dibujar_mapa = malloc(sizeof(sem_t));
    sem_init(sem_dibujar_mapa, 0, 0);

    pthread_t thread;
	pthread_create(&thread,NULL,(void*)dibujado_mapa_miramhq, NULL);
	pthread_detach(thread);
    
    // TODO: Eliminar todos los printf de api.c y server.c porque molestan al dibujar el mapa
    iniciar_servidor(IP,miramhq_config->puerto_escucha, handle_client);

    miramhq_finally(miramhq_config, logger);
    return 0;
}


void miramhq_init(t_miramhq_config** miramhq_config){
    *miramhq_config = miramhq_config_loader("./cfg/miramhq.config");
}

t_miramhq_config* miramhq_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_miramhq_config* miramhq_config = malloc(sizeof(t_miramhq_config));

    miramhq_config_parser(config, miramhq_config);
    config_destroy(config);

    return miramhq_config;
}

void miramhq_config_parser(t_config* config, t_miramhq_config* miramhq_config) {
    miramhq_config->tamanio_memoria = config_get_int_value(config, "TAMANIO_MEMORIA");
    miramhq_config->esquema_memoria = strdup(config_get_string_value(config, "ESQUEMA_MEMORIA"));
    miramhq_config->tamanio_pagina = config_get_int_value(config, "TAMANIO_PAGINA");
    miramhq_config->tamanio_swap = config_get_int_value(config, "TAMANIO_SWAP");
    miramhq_config->path_swap = strdup(config_get_string_value(config, "PATH_SWAP"));
    miramhq_config->algoritmo_reemplazo = strdup(config_get_string_value(config, "ALGORITMO_REEMPLAZO"));
    miramhq_config->criterio_seleccion = strdup(config_get_string_value(config, "CRITERIO_SELECCION"));;
    miramhq_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO"));
}

void miramhq_destroy(t_miramhq_config* miramhq_config) {
    free(miramhq_config->esquema_memoria);
    free(miramhq_config->path_swap);
    free(miramhq_config->algoritmo_reemplazo);
    free(miramhq_config->criterio_seleccion);
    free(miramhq_config->puerto_escucha);
    free(miramhq_config);
}

void miramhq_finally(t_miramhq_config* miramhq_config, t_log* logger) {

    miramhq_destroy(miramhq_config);
    log_destroy(logger);
}

void test_llenar_Memoria(t_list* tablaFrames){

    int cantidadDeFrames = list_size(tablaFrames);
    uint32_t posicion = 0;
    while(posicion < cantidadDeFrames )
    {
        void* contenidoFrame = list_get(tablaFrames,posicion);
        memcpy(contenidoFrame,&posicion,sizeof(uint32_t));
        int *p;
        p = contenidoFrame;
        printf( "El frame [%d] contiene : %d \n",posicion,*p);
        posicion++;
    }

}
void test_mostrar_Memoria( t_list* tablaFrames){

    int cantidadDeFrames = list_size(tablaFrames);
    int posicion = 0;
    while(posicion < cantidadDeFrames )
    {
        void* contenidoFrame = list_get(tablaFrames,posicion);
        int *p;
        p = contenidoFrame;
        printf( "El frame [%d] contiene : %d \n",posicion,*p);
        posicion++;
    }

}
