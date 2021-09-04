#ifndef ADMINISTRAR_TRIPULANTES_H
#define ADMINISTRAR_TRIPULANTES_H

#include "api.h"
#include "discordiador.h"
#include <commons/string.h>
#include <semaphore.h>
#include <commons/string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

static int id_tripulantes = 0;
sem_t mutex_id_tripulantes;
static int id_Patotas = 0;
sem_t mutex_id_Patotas;
typedef enum {
	NEW     = 1,
	READY   = 2,
	BLOCKED = 3,
	EXEC 	= 4,
	EXIT 	= 5,
} estado_code;

typedef struct {
    char* nombre;
    int parametro;
    int posicion_destino_X;
    int posicion_destino_Y;
    int tiempo;
} t_tarea;

typedef struct {
    int id_tripulante; // modulo al cual se conecta el tripulante
    int estado;
    t_tarea* tarea; // tarea que debe realizar
    char* archivo_tareas;
    int posicion_X;
    int posicion_Y;
    int contador_tarea;
    sem_t s_tripulante;
} t_tripulante;

typedef struct {
    t_tripulante* tripulante;
    t_modulo* modulo;
} t_hilo_tripulante;

typedef struct{
    int id_patota;
    t_list* lista_tripulantes;
} t_patota;

void crear_patota(t_modulo* modulo,char* cantidad_tripulantes,char* archivo_tareas,char** ubicaciones);
int asignar_id_tripulante(void);
int asignar_id_patota(void);
int id_patota_de_tripulante(t_tripulante* tripulante);
void listar_tripulantes_por_consola(void);
char* leer_archivo_y_devolver_contenido(char* path_archivo);
int asignar_id_patota(void);
void hilo_tripulante(t_tripulante* tripulante);
void hilo_imprime_texto(void *ubicaciones);
t_tarea* procesar_proxima_tarea(char* tarea_a_procesar);
char* pedir_proxima_tarea(void);
char* proxima_tarea_a_realizar(t_tripulante* tripulante);

#endif