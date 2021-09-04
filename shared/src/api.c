#include "api.h"
#include "server.h"


int enviar_mensaje_modulo(t_modulo* modulo, char** mensaje, int cantidadMensajes){

    int socketReturn;

    if(modulo->socket == 0){
        socketReturn = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, mensaje, cantidadMensajes);
    } else {
        socketReturn = modulo->socket;
        send_message_socket(modulo->socket, modulo->identificacion);
        send_messages_socket(modulo->socket, mensaje, cantidadMensajes);
    }

    return socketReturn;
}
    
char ** separar_por_comillas(char** string_separado_por_espacios){
    List lista_separado_por_comillas;
    initlist(&lista_separado_por_comillas);

    for (int i = 0; string_separado_por_espacios[i] != NULL; i++){

        if (string_starts_with(string_separado_por_espacios[i], "\"")){
            if (string_ends_with(string_separado_por_espacios[i], "\"")){
                char* string_sin_comillas = string_substring(string_separado_por_espacios[i], 1, strlen(string_separado_por_espacios[i]) - 2);
                pushbacklist(&lista_separado_por_comillas, string_sin_comillas);
            } else {
                char* string_concatenado = string_new();
                string_append(&string_concatenado, string_separado_por_espacios[i]);
                i++;
                int finalize_correctamente = 0;
                while(string_separado_por_espacios[i] != NULL){
                    string_append(&string_concatenado, " ");
                    string_append(&string_concatenado, string_separado_por_espacios[i]);
                    if (string_ends_with(string_separado_por_espacios[i], "\"")){
                        finalize_correctamente = 1;
                        break;
                    }
                    i++;
                }
                if (finalize_correctamente == 1){
                    char* string_sin_comillas = string_substring(string_concatenado, 1, strlen(string_concatenado) - 2);
                    pushbacklist(&lista_separado_por_comillas, string_sin_comillas);
                } else {
                    return NULL;
                }
            }
        } else {
            pushbacklist(&lista_separado_por_comillas, string_separado_por_espacios[i]);
        }

}


char ** separado_por_comillas = list_a_char(lista_separado_por_comillas);

    return separado_por_comillas;

}

char ** list_a_char(List lista)
{
    int size = sizelist(lista);
    int i = 0;
    char ** resultado = malloc(sizeof(char*) * size);

    for(IteratorList iterator_a = beginlist(lista); iterator_a != NULL; iterator_a = nextlist(iterator_a))
    {
        resultado[i] = iterator_a->data;
        i++;
    }

    return resultado;
}

List* separar_por_comillas_lista(char** string_separado_por_espacios){
    
    List* lista_separado_por_comillas = malloc(sizeof(List));
    initlist(lista_separado_por_comillas);

    for (int i = 0; string_separado_por_espacios[i] != NULL; i++){

        if (string_starts_with(string_separado_por_espacios[i], "\"")){
            if (string_ends_with(string_separado_por_espacios[i], "\"")){
                char* string_sin_comillas = string_substring(string_separado_por_espacios[i], 1, strlen(string_separado_por_espacios[i]) - 2);
                pushbacklist(lista_separado_por_comillas, string_sin_comillas);
            } else {
                char* string_concatenado = string_new();
                string_append(&string_concatenado, string_separado_por_espacios[i]);
                i++;
                int finalize_correctamente = 0;
                while(string_separado_por_espacios[i] != NULL){
                    string_append(&string_concatenado, " ");
                    string_append(&string_concatenado, string_separado_por_espacios[i]);
                    if (string_ends_with(string_separado_por_espacios[i], "\"")){
                        finalize_correctamente = 1;
                        break;
                    }
                    i++;
                }
                if (finalize_correctamente == 1){
                    char* string_sin_comillas = string_substring(string_concatenado, 1, strlen(string_concatenado) - 2);
                    pushbacklist(lista_separado_por_comillas, string_sin_comillas);
                } else {
                    return NULL;
                }
            }
        } else {
            pushbacklist(lista_separado_por_comillas, string_separado_por_espacios[i]);
        }

    }

    return lista_separado_por_comillas;

}

int obtener_numero_mensaje(char* mensaje_tipo){	
    if(strcmp(mensaje_tipo, "iniciar_patota") == 0){	
    return iniciar_patota;}

    if(strcmp(mensaje_tipo, "ubicacion_tripulante") == 0){	
    return ubicacion_tripulante;}

    if(strcmp(mensaje_tipo, "iniciar_planificacion") == 0){	
    return iniciar_planificacion;}

    if(strcmp(mensaje_tipo, "iniciar_tripulante") == 0){	
    return iniciar_tripulante; }
    
    else if(strcmp(mensaje_tipo, "expulsar_tripulante") == 0)	
    return expulsar_tripulante;

    else if(strcmp(mensaje_tipo, "tareas_patota") == 0){	
    return tareas_patota;}
    
    else if(strcmp(mensaje_tipo, "tareas_tripulante") == 0){	
    return tarea_tripulante;}

    else if(strcmp(mensaje_tipo, "listar_tripulantes") == 0){	
    return listar_tripulantes;}

    else return -1;	
};

char** obtener_array_mensajes(char* array_mensaje){

    char** array_string = string_split(array_mensaje, ",");
    // array_string =separar_por_comillas(array_string);
 
    return array_string;

}

List* obtener_list_mensajes(char* array_mensaje){

    char** array_string = string_split(array_mensaje, ",");
    List* resultado = separar_por_comillas_lista(array_string);

    return resultado;
}


char* enviar_mensaje_iniciar_patota(t_modulo* modulo,char* id_patota ,char* archivo_tareas){
    
   char* tipo_mensaje = string_itoa(iniciar_patota);
   int socket;

   char* iniciar_Patota[3] ={tipo_mensaje,id_patota,archivo_tareas};
   socket = enviar_mensaje_modulo(modulo,iniciar_Patota,3);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];
}


char* enviar_mensaje_iniciar_tripulante(t_modulo* modulo,char* patota,char* id_tripulante,char* posicion_X,char* posicion_Y){
    
   char* tipo_mensaje = string_itoa(iniciar_tripulante);
   int socket;

   char* iniciar_Tripulante[5] ={tipo_mensaje,patota,id_tripulante,posicion_X,posicion_Y};
   socket = enviar_mensaje_modulo(modulo,iniciar_Tripulante,5);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];

}

char* enviar_mensaje_bitacora_tripulante(t_modulo* modulo, char* tripulante, char* bitacora){

    if( tripulante == NULL || bitacora == NULL ){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(bitacora_tripulante);
    int socket;

    char* ubicacion_Tripulante[3] ={tipo_mensaje, tripulante, bitacora };
    socket = enviar_mensaje_modulo(modulo, ubicacion_Tripulante, 3);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];
};

char* enviar_mensaje_ubicacion_tripulante(t_modulo* modulo, char* tripulante, char* posicion_X, char* posicion_Y){

    if( tripulante == NULL || posicion_X == NULL || posicion_Y == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(ubicacion_tripulante);
    int socket;

    char* ubicacion_Tripulante[4] ={tipo_mensaje,tripulante, posicion_X, posicion_Y};
    socket = enviar_mensaje_modulo(modulo, ubicacion_Tripulante, 4);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];
};

char* enviar_mensaje_expulsar_tripulante(t_modulo* modulo, char* tripulante){

    if( tripulante == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(ubicacion_tripulante);
    int socket;

    char* ubicacion_Tripulante[2] ={tipo_mensaje,tripulante};
    socket = enviar_mensaje_modulo(modulo, ubicacion_Tripulante, 2);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];
};



char* enviar_mensaje_tarea(t_modulo* modulo,char* nombre,char* parametro){

    if( parametro == NULL || nombre == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(tareas_patota);
    int socket;

    char* tarea[3] ={tipo_mensaje,nombre,parametro};
    socket = enviar_mensaje_modulo(modulo, tarea, 3);
    
         if(socket == -1){
         return "FAIL"; }

   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);

    if(modulo->socket <= 0){
        liberar_conexion(socket);
    }

    return respuesta->mensajes[0];
};
