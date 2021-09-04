#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "api.h"
#include "server.h"
#include "bitacoras.h"

#define FALSE 0
#define TRUE 1

typedef enum {
    ID_GENERAR_OXIGENO  = 0,
    ID_CONSUMIR_OXIGENO = 1,
    ID_GENERAR_COMIDA   = 2,
    ID_CONSUMIR_COMIDA  = 3,
    ID_GENERAR_BASURA   = 4,
    ID_DESCARTAR_BASURA = 5
} tareas_code;

/* 
typedef enum tipo_mensaje{
    iniciar_Servidor = 1,
    recibir_Bitacora_Almacenar = 2,
    recibir_Pedido_Bitacora = 3,
    invocar_FSCK = 4,
    PEDIDO_FILE_SYSTEM = 5      
} tipo_mensaje_t;
*/

t_log * logger_imongo; //------>GLOBAL

void handle_server(t_result* result);

/* 
void handle_Iniciar_Servidor(t_result*);
void handle_Recibir_Bitacora_Almacenar(t_result*);
void handle_Recibir_Pedido_Bitacora(t_result *);
void handle_Invocar_FSCK(t_result*);
void handle_PEDIDO_FILE_SYSTEM(t_result*);
*/


void handle_handshake_memoria(t_result*);
void handle_iniciar_tripulante(t_result*);
void handle_expulsar_tripulante(t_result *);
void handle_tareas_patota(t_result*);
void handle_tareas_tripulante(t_result*);
void handle_ubicacion_tripulante(t_result*);

void handle_mensaje_simples(t_result*);