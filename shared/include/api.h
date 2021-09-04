#ifndef API_H
#define API_H

#include "server.h"
#include "list.h"
#include <commons/string.h>

typedef enum tipo_mensaje{ // mensajes que entienden otros modulos
    iniciar_patota = 0,
    iniciar_tripulante = 1,
    ubicacion_tripulante = 2,
    expulsar_tripulante = 3,
    tareas_patota = 4,
    tarea_tripulante = 5,
    listar_tripulantes = 6,
    iniciar_planificacion = 7,
    bitacora_tripulante = 8
} tipo_mensaje_t;


typedef struct {
    char* ip;
    char* puerto;
    int socket;
    char* identificacion;
} t_modulo;

int enviar_mensaje_modulo(t_modulo* modulo, char** mensaje, int cantidadMensajes);
char ** separar_por_comillas(char** string_separado_por_espacios);
char ** list_a_char(List lista);
List* separar_por_comillas_lista(char** string_separado_por_espacios);
int obtener_numero_mensaje(char* mensaje_tipo);
char** obtener_array_mensajes(char* array_mensaje);
List* obtener_list_mensajes(char* array_mensaje);

int obtener_numero_mensaje(char* mensaje_tipo);
char* enviar_mensaje_iniciar_patota(t_modulo* modulo,char* id_patota ,char* archivo_tareas);
char* enviar_mensaje_iniciar_tripulante(t_modulo* modulo,char* patota,char* id_tripulante,char* posicion_X,char* posicion_Y);
char* enviar_mensaje_ubicacion_tripulante(t_modulo* modulo, char* tripulante, char* posicion_X, char* posicion_Y);
char* enviar_mensaje_bitacora_tripulante(t_modulo* modulo, char* tripulante, char* bitacora);
char** enviar_mensaje_tareas_patota(t_modulo* modulo);
char* enviar_mensaje_expulsar_tripulante(t_modulo* modulo, char* tripulante);
char** enviar_mensaje_tarea_tripulante(t_modulo* modulo);


#endif