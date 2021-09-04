#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <commons/bitarray.h>
#include "server.h"
#include <semaphore.h>
#include "paginacion.h"
#include "segmentacion.h"

t_log * logger;

typedef struct {
    uint32_t PID; //Identificador de la Patota
    uint32_t tareas; //Dirección lógica del inicio de las Tareas
} t_pcb;

typedef struct {
    uint32_t TID; //Identificador del tripulante
    char estado; //Estado del tripulante (N/R/E/B)
    uint32_t posicion_X; //Posición del tripulante en el Eje X
    uint32_t posicion_Y; //Posición del tripulante en el Eje Y
    uint32_t proxima_instruccion; //Identificador de la próxima instrucción a ejecutar
    uint32_t puntero_PCB; //Dirección lógica del PCB del tripulante
} t_tcb;

typedef struct{
	uint32_t id;
	uint32_t cantidad;
	uint32_t posicionX;
	uint32_t posicionY;
	uint32_t duracion;
}t_tarea;

//Semaforos
sem_t* sem_dibujar_mapa;

// MEMORIA PRINCIPAL

 void iniciarMemoria(char* esquemaMemoria);
 void iniciarMemoriaPaginacion();
 void iniciarMemoriaSegmentacion();
 void iniciarMemoriaSwap();
 int calcularTamanioProceso(t_list* listaTareas,t_list* listaTripulantes);

 l_frame* frameLibre();

 void* puntero_memoria_principal;

 char* algoritmo;
 char* pathSwap;
 char* criterioSeleccion;
 char* puerto;
 int tamanioSwap;
 int tamanioBitMapSwap;
 int tamanioBitMapPrincipal;
 int tamanioMemoria;
 int tamanioPagina;
 char* esquemaMemoria;

int max (int x, int y);