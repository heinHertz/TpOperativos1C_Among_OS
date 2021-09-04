#ifndef ARCHIVOS_H_
#define ARCHIVOS_H_


#include "stdint.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include "imongo_config.h"
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "filesystem.h"
#include "fsck.h"

/* 
typedef struct{
    uint32_t size;
    uint32_t block_count;
    t_list * blocks;
    char CARACTER_LLENADO;
    char * MD5_ARCHIVO;
 } t_files_config;


t_files_config * OXIGENO_config; //GLOBAL OXIGENO 

t_files_config * BASURA_config;//GLOBAL BASURA

t_files_config * COMIDA_config;//GLOBAL COMIDA
*/
t_config* t_config_files;	

t_dictionary *diccionario;

t_list * lista_Oxigeno;

t_list * lista_Basura;

t_list * lista_Comida;



char * path_oxigeno;

char * path_basura;

char * path_comida;




char * crear_Archivo_Files_ARCHIVO(char * path_punto_montaje , char * NOMBRE_ARCHIVO );

void eliminar_dato(int dato);

char * serializar_t_lista( t_list * lista);


void instanciar_FILES_RECURSO( char * path_archivo_RECURSO , char * nombre );

void t_files_config_loader(char* path_config_file_RECURSO, char * nombre);

void archivos_finally(void);


void files_destroy(t_files_config* files_config);


void file_clave_destroyer(void* elemento);


t_list * files_config_parser(t_config* config, t_files_config* file_config );

void liberarArrays(char **array);


void files_config_data_LOADER(char * path_file, char * nombre);

int CONSUMIR_RECURSO_instanciador_de_archivo( t_files_config * RECURSO_config , int cantidad  ,  char * NOMBRE_ARCHIVO );


char * deserializar_DATA_t_lista( t_list * lista, int size);

#endif