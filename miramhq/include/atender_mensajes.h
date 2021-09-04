#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "shared_utils.h"
#include "server.h"
#include "mapa.h" 
#include "MemoriaPrincipal.h"

#define FALSE 0
#define TRUE 1

typedef enum tipo_mensaje{
    iniciar_Patota = 0,
    recibir_Ubicacion_Tripulante = 1,
    enviar_Proxima_Tarea = 2,
    expulsar_Tripulante = 3,
	actualizar_estado = 4
} tipo_mensaje_t;

typedef enum tarea{
	GENERAR_OXIGENO,
	CONSUMIR_OXIGENO,
	GENERAR_COMIDA,
	CONSUMIR_COMIDA,
	GENERAR_BASURA,
	DESCARTAR_BASURA
}id_tarea;

char string_log[1024*10]; 

void handle_client(t_result* result);
void handle_Recibir_Tarea_Patota(t_result*);
void handle_Recibir_Ubicacion_Tripulante(t_result *);
void handle_Enviar_Proxima_Tarea(t_result*);
void handle_Expulsar_Tripulante(t_result *);
void handle_Actualizar_Estado_Tripulante(t_result *);
void handle_Iniciar_Discordiador(t_result *);
void handle_Iniciar_Patota(t_result*);

void iniciar_patota(t_result*);
bool eliminar_tripulante(char*);

t_tcb* separarTripulante(t_tcb* tripulante, char* tripulante_sin_separar);
t_tarea* separarTarea(t_tarea* tarea, char* tarea_sin_separar);
uint32_t asignarIdTarea (char* tipo_tarea);
void* agruparProceso(t_list* lista_tareas,t_pcb* nueva_pcb,t_list* listaTripulantes,int tamanioTotalProceso);
void* generarTareaAuxiliar (int tamanioRestante,int tamanioOcupado,t_tarea* tarea);
void generar_lista_de(t_list* lista,char* char_sin_separar,char* tipo);
char* convertirTareaChar(t_tarea* proxima_tarea);
void liberar_lista(void* elemento);

//Segmentacion

t_tarea obtener_tarea(char*);
t_tarea buscar_proxima_tarea(uint32_t );