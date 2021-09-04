#ifndef BITACORAS_H_
#define BITACORAS_H_

#include "filesystem.h"
#include "archivos.h"
#include "recursos.h"

#include <commons/collections/list.h>
#include <commons/config.h>
/*
typedef struct {
    int  size;
    t_list* blocks_tripulante;
} t_tripulante_config;

t_tripulante_config * TRIPULANTE_config;   //seria tripulante actual?
*/

char * path_tripulante_actual;

void lista_destroyer_bitacora(void* elemento);
void tripu_clave_destroyer(void* elemento);
void lista_bitacora_destroyer(void* elemento);
char * OBTENER_BITACORA(int tripulante);


void crear_Tripulante_en_File_System(int numeroTripulante, t_files_config * tripulante_config );

char * crear_Archivo_Files_TRIPULANTE(char * path_punto_montaje , char * NOMBRE_ARCHIVO, t_files_config* tripulante_config  );


t_list * tripulantes_config_parser(t_config* config, t_files_config * tripulante_config);

#endif