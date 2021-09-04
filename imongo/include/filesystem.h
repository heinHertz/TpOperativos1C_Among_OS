#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

//#include <archivos.h>
#include <stddef.h>
#include "stdint.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
//#include "archivos.h"

#include <imongo_config.h> 
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <commons/collections/list.h>



typedef struct {
    
    uint32_t block_size;
    uint32_t blocks;
    char* bitmap;

}  t_superbloque;

typedef struct {

    uint32_t size;
    uint32_t block_count;
    char * blocks;
    char caracter_llenado;
    char * md5;   // MD5SUM

} t_metadata;


void * bloques;  //------>GLOBAL binario TODOS LOS  BYTES BLOQUES   -------------

uint32_t blocks_cantidad;  //------>GLOBAL cantidad numero INT no bytes

uint32_t Block_size;  //------>GLOBAL en tamnio en BYTES 

char * bitmap_global;  // GLOBAL POCOS BYTES INDICAN ESCRITURA O NO DE UN BLOCK   

//char * punto_de_montaje_GLOBAL;

int tamanio_bytes_bitmap_global;   //   CANTIDAD DE BYTES DEL BITMAP GLOBAL   SUPERBLOQUE:    4 BYTES ( UINT32)  + 4 BYTES (UINT32)  + tamanio_bytes_bitmap_global(los BYTES con bits de escritura)

uint32_t tamanio_Superbloque_GLOBAL;


t_superbloque * superbloque_global;

t_bitarray * bitarray_global;   //contenedor de Los Bits, para trabajar directamente con bits.

char * bloques_mapper; //BYTES DEL MAPPER BLOCKS.IMS   -------------------------------------
char * superbloque_bits_mapper; //BYTES DEL MAPPER BLOCKS.IMS   -------------------------------------


char * path_del_block_ims;

char * path_del_superbloque;

 


void init_file_system(char *path);
t_superbloque* crear_t_superbloque(t_bitarray* result, uint32_t bytes_por_bloque, uint32_t cantBloques);
void imprimir_bloques_superbloque(t_superbloque* superbloque);
void imprimir_bloques(t_bitarray* bitmap, uint32_t cantBloques);
int tamanio_bytes_bitArray(t_bitarray * bitmap);
uint32_t tamanio_bytes_char_del_bitArray(t_bitarray * bitmap);
t_bitarray* crearBitArray(uint32_t cantBloques);
void  abrirSuperbloque(char * path);
void* serializar_paquete_t_superbloque(t_superbloque* result);
void destroySuperbloque(t_superbloque * super);
void  crearBlocks(t_superbloque * super);


int instanciar_directorios_punto_de_montaje(char * punto_de_montaje);
void instanciar_archivos_punto_de_montaje(char * punto_de_montaje);
void inicializarBlocks(char *path ,  t_superbloque* super);
int verificar_existencia_directorio_PUNTO_DE_MONTAJE(char * punto_de_montaje);
int crear_directorio( char *punto_de_montaje); 
int verificar_existencia_directorio_BITACORAS(char *punto_de_montaje);
int verificar_existencia_directorio_FILES(char *punto_de_montaje);
int crear_directorio_FILES( char * punto_de_montaje);
int verificar_existe_directorio(char * path);
int tamanio_bytes_superbloque(t_superbloque *superbloque);
int tamanio_bytes_char_del_superbloque(t_superbloque *superbloque); 
void guardarBitArray(char * path, t_bitarray* bitarray, uint32_t bytes_por_bloque, uint32_t cantBloques );
int crear_directorio_BITACORAS(char *punto_de_montaje);


void setters_bit_0_al_bitmap_global(int index);
void setters_bit_1_al_bitmap_global(int index);
void imprimir_bits_bitarray_global(void);
void crear_archivos_recursos_files(void);
int verificar_existe_archivo(char * test);
void instanciar_archivos_recursos(void);


void imprimir_lista(t_list * lista);
void print_list(int elemento);
t_list * lista_bloques_vacios(int cantidad_bloques);
void sincronizar_file_system(void);

void persistir_en_bitmap_global_lista(t_list * lista, char * buffer, int tamanio );
char * recuperar_recursos_del_file_system(t_list * lista);
void file_system_finally(void);


t_list * almacenar_en_memoria_del_file_system( t_files_config * RECURSOS_config, char * buffer , int * estado );

void liberar_memoria_del_file_system(t_list * lista);
void liberar_en_bitmap_global_lista(t_list * lista);

#endif