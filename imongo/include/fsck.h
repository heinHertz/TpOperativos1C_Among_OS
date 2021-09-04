#ifndef FSCK_H_
#define FSCK_H_

#include "filesystem.h"
#include "archivos.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/config.h>
#include <dirent.h>


typedef struct {    
    int size;  
    t_list * list_posiciones_sabotaje;
 } t_file_bitmap_config;


t_file_bitmap_config *  file_bitmap_config;


char * path_blocks_ims;

char * path_superbloque;

int fd_blocks_ims;

int fd_superbloque_ims;


uint32_t fsck_Block_size;

uint32_t fsck_Blocks;


struct stat  * estado_superbloque_ims;

struct stat  * estado_blocks_ims;


t_list * lista_bitmap;


char * bitmap_leido;   //bitmap actual leido
int tamanio_bitmap_leido;

char * bitmap_construido;


void REPARAR_Cantidad_de_bloques(void);

void procesoArchivo(char *archivo, char * path);
void paths_setter(void);
int CONTROL_Cantidad_de_bloques_superbloque(void);

int CONTROL_Bitmap(void);
int open_files_blocks_y_superbloque(void);

void fsck_finally(void);

void file_RECURSO_clave_destroyer(void* elemento);


int CONTROL_EN_FILES_SIZE_RECURSO(char * NOMBRE_RECURSO);


int ordenarDeMenorAMayor(int a, int b );

#endif