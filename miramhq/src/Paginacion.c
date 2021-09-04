#include "atender_mensajes.h"

char* guardar_patota_en_memoria_Paginacion(char* numero_pcb, t_list* lista_tareas,t_list* listaTripulantes ,int cantidadDePaginas){
    
    
    if(!memoriaLibre(cantidadDePaginas))
    {
        log_error(logger,"No hay suficiente espacio en memoria (Memoria Principal + Memoria SWAP).");
        return "FAIL";
    }
        int tamanioDelProceso = calcularTamanioProceso(lista_tareas,listaTripulantes);
        int cantidadDeTareas = list_size(lista_tareas);
        int cantidadTripulantes = list_size(listaTripulantes);

        t_list* tablaPagina = list_create();
        l_proceso* proceso = malloc(sizeof(l_proceso));
        proceso->numProceso = (uint32_t)atoi(numero_pcb);
        proceso->cantidadDeTareas = cantidadDeTareas;
        proceso->cantidadDeTripulantes = cantidadTripulantes;
        void* punteroTripulantes = malloc(max(1,cantidadTripulantes/8));
        proceso->tripulantes_Activos = bitarray_create_with_mode(punteroTripulantes,max(1,cantidadTripulantes/8), MSB_FIRST);
        proceso->punteroTablaPagina = tablaPagina;

        t_pcb* nueva_pcb = malloc(sizeof(t_pcb));
        nueva_pcb->PID = (uint32_t)atoi(numero_pcb);
        l_direccionLogica* direccionLogicaTarea = obtenerDireccionLogica(TASK_ID,proceso,0);
        nueva_pcb->tareas = direccionLogicaTarea->dirLogica;

        for(int posicion = 0; posicion < cantidadTripulantes; posicion++)
        {
            t_tcb* tripulante = list_get(listaTripulantes,posicion);
            crearPersonajeEnMapa(tripulante->TID,tripulante->posicion_X,tripulante->posicion_Y);
            l_direccionLogica* direccionLogicaPCB = obtenerDireccionLogica(PCB_PID,proceso,posicion);
            tripulante->puntero_PCB = direccionLogicaPCB->dirLogica; //TODO: No se modifica en la lista
            bitarray_set_bit(proceso->tripulantes_Activos,tripulante->TID);
        }

        void* procesoAgrupado = agruparProceso(lista_tareas,nueva_pcb,listaTripulantes,tamanioDelProceso);
        for(int desplazamiento = 0;desplazamiento < tamanioDelProceso;desplazamiento = desplazamiento + tamanioPagina)
        {
            l_frame* contenidoFrame = frameLibre();
            crearPagina(proceso,tablaPagina,contenidoFrame);
            memcpy(contenidoFrame->punteroFrame,procesoAgrupado + desplazamiento,tamanioPagina);
            bitarray_set_bit(bitMap, contenidoFrame->posicion_bitarray);
        }
        list_add(tablaDeProcesos, proceso);

        list_destroy_and_destroy_elements(lista_tareas,liberar_lista);
        list_destroy_and_destroy_elements(listaTripulantes,liberar_lista);
        free(nueva_pcb);
        //free(proceso)->bitarray - No liberarlo por ahora

        return "OK";
} 

char* actualizar_ubicacion_tripulante_memoria_Paginacion(uint32_t numero_patota, uint32_t numero_tripulante, uint32_t posicionX,uint32_t posicionY){
    l_proceso* proceso = list_get(tablaDeProcesos,numero_patota);
    if(noExisteTripulante(proceso,numero_tripulante) )
    {
        log_error(logger, "No existe el tripulnate[%d], no es posible actualizar su ubicacion \n",numero_tripulante);
        return "FAIL";
    }
    l_direccionLogica* direccionLogicaTCB_PosX = obtenerDireccionLogica(TCB_POSICIONX,proceso,numero_tripulante);
    l_pagina* pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB_PosX->numeroPagina);
    void* stream = pagina->frame;
    stream += direccionLogicaTCB_PosX->offset;
    uint32_t posicionX_anterior;
    memcpy(&posicionX_anterior,stream,sizeof(uint32_t));

    l_direccionLogica* direccionLogicaTCB_PosY = obtenerDireccionLogica(TCB_POSICIONY,proceso,numero_tripulante);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB_PosY->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTCB_PosY->offset;
    uint32_t posicionY_anterior;
    memcpy(&posicionY_anterior,stream,sizeof(uint32_t));


    uint32_t offset = direccionLogicaTCB_PosX->offset;
    memcpy( ( (pagina->frame) + offset),&posicionX,sizeof(uint32_t));

    offset = direccionLogicaTCB_PosY->offset;
    memcpy( ( (pagina->frame) + offset),&posicionY,sizeof(uint32_t));

    char string_log[100] = "";
    sprintf(string_log, "Tripulante %d: Posicion anterior %d|%d - Posicion actual %d|%d \n",numero_tripulante,posicionX_anterior,posicionY_anterior,posicionX,posicionY);
    log_info(logger, string_log);

    moverPersonajeEnMapa(numero_tripulante,posicionX,posicionY);
    free(direccionLogicaTCB_PosX);
    free(direccionLogicaTCB_PosY);
    return "OK";
}

char* enviar_proxima_tarea_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante){
    
    l_proceso* proceso = list_get(tablaDeProcesos,numero_patota);
    if(noExisteTripulante(proceso,numero_tripulante) )
    {
        log_error(logger, "No existe el tripulnate[%d], no es posible enviar su proxima tarea \n",numero_tripulante);
        return "FAIL";
    }
    l_direccionLogica* direccionLogicaTCB = obtenerDireccionLogica(TCB_PROXINSTRUCCION,proceso,numero_tripulante);
    l_pagina* pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
    void* stream = pagina->frame;
    stream += direccionLogicaTCB->offset;
    uint32_t proximaInstruccion;
    memcpy(&proximaInstruccion,stream,sizeof(uint32_t));

    uint32_t idproximaTarea;
    if(proximaInstruccion + 1 > list_size(proceso->punteroTablaPagina)){
        idproximaTarea = proximaInstruccion;
    }
    else
    {
        idproximaTarea = proximaInstruccion + 1;
    }
    memcpy( ((pagina->frame) + direccionLogicaTCB->offset),&idproximaTarea,sizeof(uint32_t));

    t_tarea* proxima_tarea = malloc(sizeof(t_tarea));

    l_direccionLogica* direccionLogicaTarea = obtenerDireccionLogica(TASK_ID,proceso,idproximaTarea);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTarea->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTarea->offset;
    memcpy(&(proxima_tarea->id),stream,sizeof(uint32_t));

    direccionLogicaTarea = obtenerDireccionLogica(TASK_CANTIDAD,proceso,idproximaTarea);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTarea->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTarea->offset;
    memcpy(&(proxima_tarea->cantidad),stream,sizeof(uint32_t));

    direccionLogicaTarea = obtenerDireccionLogica(TASK_POSICIONX,proceso,idproximaTarea);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTarea->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTarea->offset;
    memcpy(&(proxima_tarea->posicionX),stream,sizeof(uint32_t));

    direccionLogicaTarea = obtenerDireccionLogica(TASK_POSICIONY,proceso,idproximaTarea);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTarea->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTarea->offset;
    memcpy(&(proxima_tarea->posicionY),stream,sizeof(uint32_t));

    direccionLogicaTarea = obtenerDireccionLogica(TASK_DURACION,proceso,idproximaTarea);
    pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTarea->numeroPagina);
    stream = pagina->frame;
    stream += direccionLogicaTarea->offset;
    memcpy(&(proxima_tarea->duracion),stream,sizeof(uint32_t));

    char string_log[100] = "";
    char* proximaTarea = "Ya finalizo todas sus tareas.";
    if(idproximaTarea == proximaInstruccion){
        sprintf(string_log, "El tripulante %d ya finalizo todas sus tareas. Su ultima tarea fue %s \n",numero_tripulante,proximaTarea);
    }
    else
    {
        proximaTarea = convertirTareaChar(proxima_tarea);
        sprintf(string_log, "La proxima tarea del tripulante %d es: %s \n",numero_tripulante,proximaTarea);
    }
    log_info(logger, string_log);

    free(proxima_tarea);
    free(direccionLogicaTCB);
    free(direccionLogicaTarea);
    return proximaTarea;

}

char* actualizar_Estado_Tripulante_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante,char nuevoEstado){
    
    l_proceso* proceso = list_get(tablaDeProcesos,numero_patota);
    if(noExisteTripulante(proceso,numero_tripulante) )
    {
        log_error(logger, "No existe el tripulnate[%d], no es posible actualizar su estado \n",numero_tripulante);
        return "FAIL";
    }
    l_direccionLogica* direccionLogicaTCB = obtenerDireccionLogica(TCB_ESTADO,proceso,numero_tripulante);
    l_pagina* pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
    void* stream = pagina->frame;
    stream += direccionLogicaTCB->offset;
    char estadoAnterior;
    memcpy(&estadoAnterior,stream,sizeof(char));

    uint32_t offset = direccionLogicaTCB->offset;
    memcpy( ((pagina->frame) + offset),&nuevoEstado,sizeof(char));

    char string_log[100] = "";
    sprintf(string_log, "Tripulante %d: Estado anterior %c - Estado actual %c \n",numero_tripulante,estadoAnterior,nuevoEstado);
    log_info(logger, string_log);

    free(direccionLogicaTCB);
    return "OK";

}

char* expulsar_tripulante_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante){
    
    l_proceso* proceso = list_get(tablaDeProcesos,numero_patota);
    if(noExisteTripulante(proceso,numero_tripulante) )
    {
        log_error(logger, "No existe el tripulnate[%d], no es posible actualizar su ubicacion \n",numero_tripulante);
        return "FAIL";
    }

    bitarray_clean_bit(proceso->tripulantes_Activos,numero_tripulante);
    if (noHayMasTripulantes(proceso->tripulantes_Activos,proceso->cantidadDeTripulantes) )
    {
        for(int i=0; list_size(proceso->punteroTablaPagina) > i; i++)
        {
            l_pagina* paginaABorrar = list_get(proceso->punteroTablaPagina,i);
            eliminarPagina(proceso,paginaABorrar);
	    }
        bitarray_destroy(proceso->tripulantes_Activos);
        free(proceso->punteroTablaPagina);
        free(proceso);
    }
    else
    {
        //Borrar Tripulante 
        l_direccionLogica* direccionLogicaTCB = obtenerDireccionLogica(TCB_TID,proceso,numero_tripulante);
        l_pagina* pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(uint32_t));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);

        direccionLogicaTCB = obtenerDireccionLogica(TCB_ESTADO,proceso,numero_tripulante);
        pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(char));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);
        
        direccionLogicaTCB = obtenerDireccionLogica(TCB_POSICIONX,proceso,numero_tripulante); //TODO: Segfault
        pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(uint32_t));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);

        direccionLogicaTCB = obtenerDireccionLogica(TCB_POSICIONY,proceso,numero_tripulante);
        pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(uint32_t));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);

        direccionLogicaTCB = obtenerDireccionLogica(TCB_PROXINSTRUCCION,proceso,numero_tripulante);
        pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(uint32_t));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);

        direccionLogicaTCB = obtenerDireccionLogica(TCB_PUNTEROPCB,proceso,numero_tripulante);
        pagina = list_get(proceso->punteroTablaPagina,direccionLogicaTCB->numeroPagina);
        limpiarContenidoPagina(pagina->contenidoPagina,direccionLogicaTCB->offset,sizeof(uint32_t));
        if(laPaginaEstaVacia(pagina)) eliminarPagina(proceso,pagina);
    }
    eliminarPersonajeDelMapa(numero_tripulante);
    return "OK";

}

bool memoriaLibre(int cantidadDePaginasNecesarias){

	int cantidadPaginasLibres = 0;
	int i=0;
	while( tamanioBitMapPrincipal > i && cantidadDePaginasNecesarias > cantidadPaginasLibres)
	{
		int bitOcupado = bitarray_test_bit(bitMap, i);
		if(!bitOcupado){
			cantidadPaginasLibres ++;
		}
		i++;
	}

    i=0;
    while( tamanioBitMapSwap > i && cantidadDePaginasNecesarias > cantidadPaginasLibres)
	{
		int bitOcupado = bitarray_test_bit(bitMapSwap, i);
		if(!bitOcupado){
			cantidadPaginasLibres ++;
		}
		i++;
	}

	if(cantidadPaginasLibres < cantidadDePaginasNecesarias)
        return false;
	else 
        return true;

}

void crearPagina(l_proceso* proceso, t_list* tablaPagina,l_frame* contenidoFrame){
        l_pagina* pagina = malloc(sizeof(l_pagina));
        pagina->numPagina = 0;
        pagina->bitUso = 1;
        pagina->bitPresencia = 1;
        pagina->bitModificado = 0;
        pagina->frame = contenidoFrame;
		pagina->swap = NULL;
        void* punteroContenidoPagina = malloc(max(1,tamanioPagina/8));
        pagina->contenidoPagina = bitarray_create_with_mode(punteroContenidoPagina,max(1,tamanioPagina/8), MSB_FIRST);

        for(int i=0;  tamanioPagina > i; i++){
		bitarray_clean_bit(pagina->contenidoPagina, i);
	    }
		list_add(tablaPagina, pagina);

		proceso->punteroTablaPagina = tablaPagina;
}

void* agruparProceso(t_list* lista_tareas,t_pcb* nueva_pcb,t_list* listaTripulantes,int tamanioTotalProceso){

    void* proceso = malloc(tamanioTotalProceso);
    int posicion = 0;
    int desplazamiento = 0;

    //PCB
    memcpy(proceso + desplazamiento,&nueva_pcb->PID,sizeof(uint32_t));
    desplazamiento = desplazamiento + sizeof(uint32_t);
    memcpy(proceso + desplazamiento,&nueva_pcb->tareas,sizeof(uint32_t));
    desplazamiento = desplazamiento + sizeof(uint32_t);

    //TAREAS
    while( posicion < list_size(lista_tareas))
    {
        t_tarea* tarea = list_get(lista_tareas,posicion);
        memcpy(proceso + desplazamiento,&tarea->id,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tarea->cantidad,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tarea->posicionX,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tarea->posicionY,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tarea->duracion,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        posicion++;
    }
    posicion = 0;
    
    //TRIPULANTES
    while( posicion < list_size(listaTripulantes))
    {
        t_tcb* tripulante = list_get(listaTripulantes,posicion);
        memcpy(proceso + desplazamiento,&tripulante->TID,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tripulante->estado,sizeof(char));
        desplazamiento = desplazamiento + sizeof(char);
        memcpy(proceso + desplazamiento,&tripulante->posicion_X,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tripulante->posicion_Y,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tripulante->proxima_instruccion,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        memcpy(proceso + desplazamiento,&tripulante->puntero_PCB,sizeof(uint32_t));
        desplazamiento = desplazamiento + sizeof(uint32_t);
        posicion++;
    }
    return proceso;
}

l_direccionLogica* obtenerDireccionLogica(id_objeto tipoObjeto,l_proceso* proceso,uint32_t numero){

l_direccionLogica* direccionLogica = malloc(sizeof(l_direccionLogica));
int numero_pagina = 0;
int offset = 0;
int posicionInicialTarea = sizeof(t_pcb) + (numero * sizeof(t_tarea));
int posicionInicialTripulante = sizeof(t_pcb) + (proceso->cantidadDeTareas * sizeof(t_tarea))+ (numero * 21);

switch (tipoObjeto)
{
case PCB_PID:
    numero_pagina = 0 / tamanioPagina ; 
    offset = 0 - (numero_pagina * tamanioPagina);
    break;
case PCB_TAREAS:
    numero_pagina = (0 + sizeof(uint32_t))  / tamanioPagina ; 
    offset = (0 + sizeof(uint32_t)) - (numero_pagina * tamanioPagina);
    break;
case TASK_ID:
    numero_pagina = posicionInicialTarea / tamanioPagina ; 
    offset = posicionInicialTarea - (numero_pagina * tamanioPagina);
    break;
case TASK_CANTIDAD:
    numero_pagina = (posicionInicialTarea + sizeof(uint32_t))  / tamanioPagina ; 
    offset = (posicionInicialTarea + sizeof(uint32_t)) - (numero_pagina * tamanioPagina);
    break;
case TASK_POSICIONX:
    numero_pagina = (posicionInicialTarea + (sizeof(uint32_t)*2))  / tamanioPagina ; 
    offset = (posicionInicialTarea + (sizeof(uint32_t)*2)) - (numero_pagina * tamanioPagina);
    break;
case TASK_POSICIONY:
    numero_pagina = (posicionInicialTarea + (sizeof(uint32_t)*3))  / tamanioPagina ; 
    offset = (posicionInicialTarea + (sizeof(uint32_t)*3)) - (numero_pagina * tamanioPagina);
    break;
case TASK_DURACION:
    numero_pagina = (posicionInicialTarea + (sizeof(uint32_t)*4))  / tamanioPagina ; 
    offset = (posicionInicialTarea + (sizeof(uint32_t)*4)) - (numero_pagina * tamanioPagina);
    break;
case TCB_TID:
    numero_pagina = posicionInicialTripulante / tamanioPagina ; 
    offset = posicionInicialTripulante - (numero_pagina * tamanioPagina);
    break;
case TCB_ESTADO:
    numero_pagina = (posicionInicialTripulante + sizeof(uint32_t))  / tamanioPagina ; 
    offset = (posicionInicialTripulante + sizeof(uint32_t)) - (numero_pagina * tamanioPagina);
    break;
case TCB_POSICIONX:
    numero_pagina = (posicionInicialTripulante + sizeof(uint32_t) + sizeof(char))  / tamanioPagina ; 
    offset = (posicionInicialTripulante + sizeof(uint32_t) + sizeof(char)) - (numero_pagina * tamanioPagina);
    break;
case TCB_POSICIONY:
    numero_pagina = (posicionInicialTripulante + (sizeof(uint32_t)*2) + sizeof(char))  / tamanioPagina ; 
    offset = (posicionInicialTripulante + (sizeof(uint32_t)*2) + sizeof(char)) - (numero_pagina * tamanioPagina);
    break;
case TCB_PROXINSTRUCCION:
    numero_pagina = (posicionInicialTripulante + (sizeof(uint32_t)*3) + sizeof(char))  / tamanioPagina ; 
    offset = (posicionInicialTripulante + (sizeof(uint32_t)*3)  + sizeof(char)) - (numero_pagina * tamanioPagina);
    break;
case TCB_PUNTEROPCB:
    numero_pagina = (posicionInicialTripulante + (sizeof(uint32_t)*4) + sizeof(char))  / tamanioPagina ; 
    offset = (posicionInicialTripulante + (sizeof(uint32_t)*4)  + sizeof(char)) - (numero_pagina * tamanioPagina);
    break;
default:
    log_info(logger, "Error al obtener direccion Logica. [%d] \n",tipoObjeto);
    break;
}

direccionLogica->numeroPagina = numero_pagina;
direccionLogica->offset = offset;
//TODO: 5 Caracteres para N° pagina y 5 caracteres para offset. Padear a izquierda con 0
char aux [10 + 1] = "";
strcat(aux,string_itoa(direccionLogica->numeroPagina) ); // concateno lo q hay en n1 (convertido en char) a aux
strcat(aux,string_itoa(direccionLogica->offset) ); // concateno lo q hay en n2 (convertido en char) a aux
direccionLogica->dirLogica= (uint32_t)atoi(aux); // saco lo que tenia el aux

return direccionLogica;
}
void liberar_lista(void* elemento)
{
    free(elemento);
}

bool noHayMasTripulantes(t_bitarray* bitArrayTripulantes,uint32_t cantidadDeTripulantes){

    bool noHayTripulantes = true;
    
    for(int i=0; cantidadDeTripulantes > i; i++){

        if(bitarray_test_bit(bitArrayTripulantes,i))
        {
            noHayTripulantes = false;
        }
	}

    return noHayTripulantes;

}

bool noExisteTripulante(l_proceso* proceso,uint32_t numero_tripulante){
    return !bitarray_test_bit(proceso->tripulantes_Activos,numero_tripulante);
}

bool laPaginaEstaVacia(l_pagina* pagina){

    bool estaVacia = true;
    for(int i=0; tamanioPagina > i; i++)
    {
        if(bitarray_test_bit(pagina->contenidoPagina,i))
        {
            estaVacia = false;
        }
    }
    
    return estaVacia;
}

void eliminarPagina(l_proceso* proceso,l_pagina* pagina) {
    if(pagina->bitUso == 1)
    {
        bitarray_clean_bit(bitMap,pagina->frame->posicion_bitarray);
    }
    else
    {
        //Eliminar frame? de memoria virtual
    }       
    bitarray_destroy(pagina->contenidoPagina);
    list_remove(proceso->punteroTablaPagina,pagina->numPagina); //Lo elimino de la tabla de pagina
    free(pagina);
}

void limpiarContenidoPagina(t_bitarray* bitArrayContenidoPag,uint32_t posicionInicial,uint32_t longitud){

        for(int i=posicionInicial; longitud > i; i++)
    {
        bitarray_clean_bit(bitArrayContenidoPag,i);
    }
}