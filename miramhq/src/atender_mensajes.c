#include "atender_mensajes.h"

void handle_client(t_result* result){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        switch(tipo_mensaje){
                    case iniciar_Patota:
                    handle_Iniciar_Patota(result);
                    break;
                    case recibir_Ubicacion_Tripulante:
        			handle_Recibir_Ubicacion_Tripulante(result);
        			break;
                    case enviar_Proxima_Tarea:
        			handle_Enviar_Proxima_Tarea(result);
        			break;
                    case expulsar_Tripulante:
        			handle_Expulsar_Tripulante(result);
        			break;
                    case actualizar_estado:
        			handle_Actualizar_Estado_Tripulante(result);
        			break;
        		    default:
        			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
        			break;
        }
    return;
}

void handle_Iniciar_Patota(t_result* result){

    iniciar_patota(result);
    liberar_conexion(result->socket);
}


void iniciar_patota(t_result* result)
{
    char* numero_patota = result->mensajes->mensajes[1];
    char* tareas_sin_separar = result->mensajes->mensajes[2];
    //char* tripulantes_sin_separar = result->mensajes->mensajes[3];
    char* tripulantes_sin_separar = "0;N;1;1;0;0\n1;N;8;7;0;0\n2;N;0;0;0;1\n3;N;3;1;0;0";

    t_list* lista_tareas = list_create();
    generar_lista_de(lista_tareas,tareas_sin_separar,"TAREAS");

    t_list* lista_tripulantes = list_create();
    generar_lista_de(lista_tripulantes,tripulantes_sin_separar,"TRIPULANTES");

    char* respuesta[1];
    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        int tamanioDelProceso = calcularTamanioProceso(lista_tareas,lista_tripulantes);
        int cantidadDePaginas = (int)ceil( ((double)tamanioDelProceso / (double)tamanioPagina) );
        respuesta[0] = guardar_patota_en_memoria_Paginacion(numero_patota,lista_tareas,lista_tripulantes,cantidadDePaginas);
        //respuesta[0] = actualizar_ubicacion_tripulante_memoria_Paginacion(0,1,9,9);
        //respuesta[0] = enviar_proxima_tarea_Paginacion(0,1);
        //respuesta[0] = actualizar_Estado_Tripulante_Paginacion(0,1,'R');
        respuesta[0] = expulsar_tripulante_Paginacion(0,1);
    }   
    else
    {
        t_tareas_patota* tareas_de_patota;
        tareas_de_patota->patota = (uint32_t)atoi(numero_patota);
        tareas_de_patota->tareas = lista_tareas;
        tareas_de_patota->tripulantes = lista_tripulantes;
        crear_PCB(tareas_de_patota);
        respuesta[0]= "OK";
    }

    send_messages_socket(result->socket, respuesta, 1);
    
}

void handle_Recibir_Ubicacion_Tripulante(t_result* result){

    //uint32_t numero_patota = (uint32_t)atoi(result->mensajes->mensajes[1]);
    //uint32_t numero_tripulante = (uint32_t)atoi(result->mensajes->mensajes[2]);
    //uint32_t posicionX = (uint32_t)atoi(result->mensajes->mensajes[3]);
    //uint32_t posicionY = (uint32_t)atoi(result->mensajes->mensajes[4]);

    uint32_t numero_patota = 0;
    uint32_t numero_tripulante = 1;
    uint32_t posicionX = 9;
    uint32_t posicionY = 9;

    char* respuesta[1];

    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        respuesta[0] = actualizar_ubicacion_tripulante_memoria_Paginacion(numero_patota,numero_tripulante,posicionX,posicionY);
    }   
    else
    {
        //t_movimiento posicion;
        //posicion->id = numero_tripulante;
        //posicion->X = posicionX;
        //posicionY->Y=posicionY;
        respuesta[0]= "OK";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}


void handle_Enviar_Proxima_Tarea(t_result* result){

    //uint32_t numero_patota = (uint32_t)atoi(result->mensajes->mensajes[1]);
    //uint32_t numero_tripulante = (uint32_t)atoi(result->mensajes->mensajes[2]);

    uint32_t numero_patota = 0;
    uint32_t numero_tripulante = 1;
    
    char* respuesta[1];

    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        respuesta[0] = enviar_proxima_tarea_Paginacion(numero_patota,numero_tripulante);
    }   
    else
    {
        respuesta[0]= "OK";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void handle_Expulsar_Tripulante(t_result* result){
    
    //uint32_t numero_patota = (uint32_t)atoi(result->mensajes->mensajes[1]);
    //uint32_t numero_tripulante = (uint32_t)atoi(result->mensajes->mensajes[2]);
    uint32_t numero_patota = 0;
    uint32_t numero_tripulante = 1;

    char* respuesta[1];

    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        respuesta[0] = expulsar_tripulante_Paginacion(numero_patota,numero_tripulante);
    }   
    else
    {
        respuesta[0]= "OK";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void handle_Actualizar_Estado_Tripulante(t_result* result){
    
    //uint32_t numero_patota = (uint32_t)atoi(result->mensajes->mensajes[1]);
    //uint32_t numero_tripulante = (uint32_t)atoi(result->mensajes->mensajes[2]);
    //char nuevo_estado = result->mensajes->mensajes[3];

    uint32_t numero_patota = 0;
    uint32_t numero_tripulante = 1;
    char nuevo_estado = 'R'; //(N/R/E/B)

    char* respuesta[1];

    if( (strcmp(esquemaMemoria,"PAGINACION") == 0) )
    {
        respuesta[0] = actualizar_Estado_Tripulante_Paginacion(numero_patota,numero_tripulante,nuevo_estado);
    }   
    else
    {
        respuesta[0]= "OK";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void generar_lista_de(t_list* lista,char* char_sin_separar,char* tipo){    
    
    char** array = string_split(char_sin_separar,"\n");
    int cantidad = 0;
    int error = 0;
    while(array[cantidad] != NULL && error != 1)
    {   
        if (strcmp(tipo,"TAREAS") == 0)
        {
            t_tarea* tarea = malloc(sizeof(t_tarea));
            tarea = separarTarea(tarea,array[cantidad]);
            list_add(lista,tarea);
        }
        else if (strcmp(tipo,"TRIPULANTES") == 0)
        {
            t_tcb* tripulante =  malloc(21);
            tripulante = separarTripulante(tripulante,array[cantidad]);
            list_add(lista,tripulante);
        }
        else{
            log_info(logger, "Error al separar Tareas y TRipulantes, el siguiente parametro es invalido: [%s]",tipo);
            error = 1;
        }
        cantidad ++; 
    }
}

t_tcb* separarTripulante(t_tcb* tripulante, char* tripulante_sin_separar){
        char** tripulantes_separados  = string_split(tripulante_sin_separar,";"); 
        tripulante->TID = (uint32_t)atoi(tripulantes_separados[0]);
        memcpy(&tripulante->estado,tripulantes_separados[1],sizeof(char));
        tripulante->posicion_X = (uint32_t)atoi(tripulantes_separados[2]);
        tripulante->posicion_Y = (uint32_t)atoi(tripulantes_separados[3]);
        tripulante->proxima_instruccion = (uint32_t)atoi(tripulantes_separados[4]);
        tripulante->puntero_PCB = (uint32_t)atoi(tripulantes_separados[5]);
        return tripulante;
}

t_tarea* separarTarea(t_tarea* tarea, char* tarea_sin_separar){
        //"GENERAR_OXIGENO 12;2;3;5"
        char** tarea_separada  = string_split(tarea_sin_separar," "); // ["GENERAR_OXIGENO","12;2;3;5"]
        char** tarea_separada2 = string_split(tarea_separada[1],";"); // ["12","2","3","5"]
        tarea->id = asignarIdTarea(tarea_separada[0]);
        tarea->cantidad = (uint32_t)atoi(tarea_separada2[0]);
        tarea->posicionX = (uint32_t)atoi(tarea_separada2[1]);
        tarea->posicionY = (uint32_t)atoi(tarea_separada2[2]);
        tarea->duracion = (uint32_t)atoi(tarea_separada2[3]);
        return tarea;
}

uint32_t asignarIdTarea (char* tipo_tarea){
    if (strcmp(tipo_tarea, "GENERAR_OXIGENO") == 0) { return GENERAR_OXIGENO; }
    if (strcmp(tipo_tarea, "CONSUMIR_OXIGENO") == 0) { return CONSUMIR_OXIGENO; }
    if (strcmp(tipo_tarea, "GENERAR_COMIDA") == 0) { return GENERAR_COMIDA; }
    if (strcmp(tipo_tarea, "CONSUMIR_COMIDA") == 0) { return CONSUMIR_COMIDA; }
    if (strcmp(tipo_tarea, "GENERAR_BASURA") == 0) { return GENERAR_BASURA; }
    if (strcmp(tipo_tarea, "DESCARTAR_BASURA") == 0) { return DESCARTAR_BASURA; }
    
    return 6; //Tareas que no tienen operacion en archivo recurso
}

char* convertirTareaChar(t_tarea* proxima_tarea){
    char * proximaTarea = string_new();
    string_append(&proximaTarea,string_itoa(proxima_tarea->id) );
    string_append(&proximaTarea,";");   
    string_append(&proximaTarea,string_itoa(proxima_tarea->cantidad) );
    string_append(&proximaTarea,";" );
    string_append(&proximaTarea,string_itoa(proxima_tarea->posicionX) );
    string_append(&proximaTarea,";" );
    string_append(&proximaTarea,string_itoa(proxima_tarea->posicionY) );
    string_append(&proximaTarea,";" );
    string_append(&proximaTarea,string_itoa(proxima_tarea->duracion) );
    return proximaTarea;
}