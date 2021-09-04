#ifndef RECURSOS_H_
#define RECURSOS_H_

#include <stdlib.h>
#include <stdio.h>
#include "imongo_config.h"

#include "archivos.h"
#include <commons/collections/list.h>

#include <commons/string.h>
#include <string.h>
#include "filesystem.h"
#include "bitacoras.h"
#include "fsck.h"


typedef enum tipo_tarea{
    desplazamiento = 1,
    ejecucion_tarea = 2,
    finalizar_tarea = 3,
    panico_ubicacion_sabotaje = 4,
    resolver_sabotaje = 5      
} t_tipo_tarea;



char *  formatear_tarea_desplazamiento(char * data);

char * formatear_tarea_ejecucion_tarea(char * data);

char * formatear_tarea_finalizar_tarea(char * data);

char * formatear_tarea_panico_ubicacion_sabotaje(char * data);

char* formatear_tarea_resolver_sabotaje(char * data);


t_list * string_de_array_a_t_lista(char *string);


void almacenar_en_archivo_oxigeno_ims(char * buffer, t_list * lista);
void almacenar_en_archivo_comida_ims(char * buffer, t_list * lista);

void actualizar_archivos_oxigeno_ims(t_files_config  * RECURSOS_config, int cantidad , char * path_recurso);

void lista_destroyer(void* elemento);

void GENERAR_OXIGENO(int);
void GENERAR_COMIDA(int);
void GENERAR_BASURA(int);


void GENERAR_STRING( int tripulante , char * buffer);


void CONSUMIR_OXIGENO(int cantidad);
void CONSUMIR_BASURA(int cantidad);
void CONSUMIR_COMIDA(int cantidad);


int CONSUMIR_RECURSO_instanciador_de_archivo(t_files_config *  RECURSO_config, int cantidad, char * nombre);


t_list * almacenar_en_memoria_del_file_systemAUXILIAR( t_files_config * RECURSOS_config, char * buffer, int * correcto  );



#endif