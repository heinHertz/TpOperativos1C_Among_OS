#include "imongo_mensajes.h"


void handle_server(t_result* result){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        
        printf("Recibido:  %d \n", tipo_mensaje);

     
        switch(tipo_mensaje){
                   /* case mensaje_simple:        		
                    recibir_mensaje(result->socket);   REFACTORING CUANDO ESTEN LOS MENSAJES
                    handle_mensaje_simples(result);
        			break;    */            
                    case iniciar_tripulante:
        			handle_iniciar_tripulante(result);
        			break;
                    case ubicacion_tripulante:
        			handle_ubicacion_tripulante(result);
        			break;
                    case expulsar_tripulante:
        			handle_expulsar_tripulante(result);
        			break;
                    case tareas_patota:
        			handle_tareas_patota(result);
        			break;                       
                    case iniciar_patota:
                   { log_info( logger_imongo, "iniciar patota log" ); }
                    break;
                    case bitacora_tripulante:
                    handle_bitacora_tripulante(result);
                    break;        
        		    default:
        			log_warning(logger_imongo, "Operacion desconocida. No quieras meter la pata");
        			break;
        }
    return;
}
 void handle_mensaje_simples(t_result* result){
    
    char* respuesta[1];
    char* string = "TeEnvioMensajeTextoCamelCase";
    if (TRUE){ 
   
    respuesta[0] = malloc(strlen(string) );
    strcpy(respuesta[0], string);
        
    } else {
        respuesta[0] = "FAIL";
    }
       
        enviar_mensaje(respuesta[0],result->socket);  //  RECEPTOR RESPONDEDOR
  //  send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
    log_info( logger_imongo, "mensaje simple recibido" );
    
    free(respuesta[0]);
 }
 
void handle_bitacora_tripulante(t_result* result){
    
    char* respuesta[1];
    char* bitacora = result->mensajes->mensajes[1];
    char* parametro = result->mensajes->mensajes[2];

        printf("Recibí la bitacora %s del tripulante:  %s\n", bitacora, parametro);


        TRIPULANTE_GENERAR_TAREA( atoi( bitacora) , parametro  ); 

        respuesta[0] = "OK";

  //  int32_t id_tarea = proxima_tarea_code(nombre_tarea);
  //  int parametro_de_tareas = atoi(parametro);

  

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}


void handle_handshake_memoria(t_result* result){
    
    char* respuesta[1];
 char* string = "Mensaje:TeEnvioMensajeTexto";
    if (TRUE){ //HAcer lo que tenga que hacer
   
    respuesta[0] = malloc(strlen(string) );
    strcpy(respuesta[0], string);
     //   respuesta[0] ="OKKKKK";//malloc(strlen(string));
        
    } else {
        respuesta[0] = "FAIL";
    }
       
    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

    free(respuesta[0]);
}

void handle_iniciar_tripulante(t_result* result){
    
    char* respuesta[10];

    if (TRUE){ //HAcer lo que tenga que hacer
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void handle_expulsar_tripulante(t_result* result){
    
    char* respuesta[1];

    if (TRUE){ //HAcer lo que tenga que hacer
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void handle_ubicacion_tripulante(t_result* result){
    
    char* respuesta[1];

    if (TRUE){ //HAcer lo que tenga que hacer
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

void handle_tareas_patota(t_result* result){
    
    char* respuesta[1];
    char* nombre_tarea = result->mensajes->mensajes[1];
    char* parametro = result->mensajes->mensajes[2];
    printf("Recibí la tarea %s con el parametro %s\n", nombre_tarea,parametro);
    
    int32_t id_tarea = proxima_tarea_code(nombre_tarea);
    int parametro_de_tareas = atoi(parametro);

    switch(id_tarea)
    {
        case ID_GENERAR_OXIGENO: GENERAR_OXIGENO(parametro_de_tareas);
        respuesta[0] = "OK";
        break;
        
        case ID_CONSUMIR_OXIGENO: CONSUMIR_OXIGENO(parametro_de_tareas);
        respuesta[0] = "OK";
        break;

        case ID_GENERAR_COMIDA: GENERAR_COMIDA(parametro_de_tareas);
        respuesta[0] = "OK";
        break;

        case ID_CONSUMIR_COMIDA: CONSUMIR_COMIDA(parametro_de_tareas);
        respuesta[0] = "OK";
        break;

        case ID_GENERAR_BASURA: GENERAR_BASURA(parametro_de_tareas);
        respuesta[0] = "OK";
        break;

        case ID_DESCARTAR_BASURA: DESCARTAR_BASURA(parametro_de_tareas);
        respuesta[0] = "OK";
        break;

        default: printf("No es una tarea del anexo dos\n");
        respuesta[0] = "FAIL";
        break;
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}

int32_t proxima_tarea_code(char* tarea){

	if(string_equals_ignore_case(tarea, "GENERAR_OXIGENO")){
		
    return ID_GENERAR_OXIGENO;
	} else if (string_equals_ignore_case(tarea, "CONSUMIR_OXIGENO")){
		
    return ID_CONSUMIR_OXIGENO;
	} else if (string_equals_ignore_case(tarea, "GENERAR_COMIDA")){
	
    return ID_GENERAR_COMIDA;
    } else if (string_equals_ignore_case(tarea, "CONSUMIR_COMIDA")){
    
    return ID_CONSUMIR_COMIDA;
    } else if (string_equals_ignore_case(tarea, "GENERAR_BASURA")){
		
    return ID_GENERAR_BASURA;
    } else if (string_equals_ignore_case(tarea, "DESCARTAR_BASURA")){
		
    return ID_DESCARTAR_BASURA;
    }
  return -1;
}