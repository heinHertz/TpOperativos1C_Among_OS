#ifndef MIRAMHQ_H
#define MIRAMHQ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>
#include <curses.h>

#include "atender_mensajes.h" //Por handle de memoria

// VARIABLES Y ESTRUCTURAS
#define IP "127.0.0.1"

typedef struct {
    int tamanio_memoria;
    char* esquema_memoria;
    int tamanio_pagina;
    int tamanio_swap;
    char* path_swap;
    char* algoritmo_reemplazo;
    char* criterio_seleccion;
    char* puerto_escucha;
} t_miramhq_config;

t_miramhq_config* miramhq_config;

// FUNCIONES
void miramhq_init(t_miramhq_config** miramhq_config);
void miramhq_finally(t_miramhq_config* miramhq_config, t_log* logger);
t_miramhq_config* miramhq_config_loader(char* path_config_file);
void miramhq_config_parser(t_config* config, t_miramhq_config* miramhq_config);
void miramhq_destroy(t_miramhq_config* miramhq_config);

void test_llenar_Memoria(t_list* tablaFrames);
void test_mostrar_Memoria(t_list* tablaFrames);

#endif