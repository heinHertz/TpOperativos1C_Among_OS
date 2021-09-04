#ifndef IMONGO_CONFIG_H
#define IMONGO_CONFIG_H

#include "imongo_commons.h"
#include  <string.h>
#include "stdint.h"


typedef struct{
    uint32_t size;
    uint32_t block_count;
    t_list * blocks;
    char CARACTER_LLENADO;
    unsigned int MD5_ARCHIVO;
 } t_files_config;


t_files_config * TRIPULANTE_config;  //TRIPULANTE ACTUAL



t_files_config * OXIGENO_config; //GLOBAL OXIGENO 

t_files_config * BASURA_config;//GLOBAL BASURA

t_files_config * COMIDA_config;//GLOBAL COMIDA

t_config* t_config_files;	


char * punto_de_montaje_GLOBAL;

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* puerto_escucha;
    char* punto_montaje;
    int tiempo_sincronizacion;
    t_list * list_posiciones_sabotaje;
 //   char* block_size;
  //  char* blocks;
  //  char* magic_number;
 //   char* ip;
} t_imongo_config;

//t_list *lista; //GLOBAL

t_imongo_config* imongo_config;

t_log* logger_imongo;



void imongo_init( void);
void imongo_finally(t_imongo_config* imongo_config, t_log* logger_imongo);
void imongo_config_loader(char* path_config_file);
void imongo_config_parser(t_config* config, t_imongo_config* imongo_config);
void imongo_destroy(t_imongo_config* imongo_config);



void clave_destroyer(void* elemento);

#endif