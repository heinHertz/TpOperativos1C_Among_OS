#ifndef DISCORDIADOR_H
#define DISCORDIADOR_H

#include "shared_utils.h"
#include "server.h"
#include "api.h"
#include "administrar_tripulantes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/log.h>
#include <stdbool.h>
#include <commons/config.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include "mensajes_consola.h"

typedef struct {
    char* ip_mi_ram_hq;
    char* puerto_mi_ram_hq;
    char* ip_i_mongo_store;
    char* puerto_i_mongo_store;
    int grado_multitarea;
    char* algoritmo;
    int quantum;
    int duracion_sabotaje;
    int retardo_ciclo_cpu;
} t_discordiador_config;

typedef enum {
	RR    = 1,
	FIFO  = 2
} algoritmo_code;

typedef enum {
    TAREA_IO = 0,
    TAREA_ESPECIAL= 1,
} tareas_code;

typedef struct {
	int32_t 	algoritmo_code;
	char* 	algoritmo_string;
	int32_t 	quantum;
	int32_t 	retardo;
	double alpha;
}t_algoritmo;

t_algoritmo algoritmo;
t_modulo* modulo_file_system;
t_modulo* modulo_memoria;
t_log* logger;
t_discordiador_config* discordiador_config;

sem_t s_cola_ready_con_items;
sem_t s_cola_bloqueados_con_items;
sem_t s_procesos_en_exec;
pthread_mutex_t mutex_cola_ready;
pthread_mutex_t mutex_cola_bloqueados;
sem_t s_iniciar_planificacion;

t_list* lista_patotas;
t_list* cola_ready;
t_list* cola_blocked;

void discordiador_init(t_discordiador_config** discordiador_config, t_log** logger);
t_discordiador_config* discordiador_config_loader(char* path_config_file, t_log** logger);
void discordiador_config_parser(t_config* config, t_discordiador_config* discordiador_config);
void escuchar_mensajes_socket(t_parameter* parametro);
void handle_client(t_result* result);
int handshake(t_modulo* modulo);
void finally(t_discordiador_config* cliente_config, t_log* logger);
void discordiador_destroy(t_discordiador_config* cliente_config);
void hilo_leer_consola(void);
void hilo_planificador(void);
void planificar_fifo(void);
void planificar();
t_algoritmo get_algoritmo(t_discordiador_config* config);
t_modulo * crear_modulo_imongo(char* ip, char* puerto);
t_modulo * crear_modulo_miram(char* ip, char* puerto);
void hilo_bloqueados(void);
void administrar_cola_io(void);

#endif