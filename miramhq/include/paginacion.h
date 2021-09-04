#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct frame{

    uint32_t posicion_bitarray;
    uint32_t proceso;
    uint32_t pagina;
    void* punteroFrame;
} l_frame;


typedef struct pagina{

    uint32_t numPagina;
    uint32_t bitUso;
    uint32_t bitPresencia;
    uint32_t bitModificado;
    t_bitarray* contenidoPagina;
    l_frame* frame;
    void *swap;
} l_pagina;

typedef struct proceso{

    uint32_t numProceso;
    uint32_t cantidadDeTareas;
    uint32_t cantidadDeTripulantes;
    t_bitarray* tripulantes_Activos;
    t_list* punteroTablaPagina;
} l_proceso;

typedef struct direccionLogica{

    uint32_t numeroPagina;
    uint32_t offset;
    uint32_t dirLogica;
} l_direccionLogica;

typedef struct direccionFisica{ //TODO: Si no lo termino usando Borrar

    uint32_t numeroFrame;
    uint32_t offset;
    uint32_t dirFisica;
} l_direccionFisica;

typedef enum objeto{
	PCB_PID,
	PCB_TAREAS,
	TCB_TID,
	TCB_ESTADO,
	TCB_POSICIONX,
	TCB_POSICIONY,
    TCB_PROXINSTRUCCION,
    TCB_PUNTEROPCB,
    TASK_ID,
    TASK_CANTIDAD,
    TASK_POSICIONX,
    TASK_POSICIONY,
    TASK_DURACION
}id_objeto;

 t_list* tablaFrames;
 t_list* tablaDeProcesos;
 char* punteroBitMap;
 t_bitarray *bitMap;

 //SWAP
 void *puntero_memoria_swap;
 t_list* tablaSwap;
 char* punteroBitMapSwap;
 t_bitarray *bitMapSwap;
 int archivoSwap;

//Algoritmos
 t_list* pilaPaginasAlgoritmos;
 int inicioClockMej;

 char* guardar_patota_en_memoria_Paginacion(char* numero_pcb,t_list* listaTripulantes ,t_list* lista_tareas,int cantidadDePaginas);
 char* actualizar_ubicacion_tripulante_memoria_Paginacion(uint32_t numero_patota, uint32_t numero_tripulante, uint32_t posicionX,uint32_t posicionY);
 char* enviar_proxima_tarea_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante);
 char* actualizar_Estado_Tripulante_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante,char nuevoEstado);
 char* expulsar_tripulante_Paginacion(uint32_t numero_patota,uint32_t numero_tripulante);
 l_direccionLogica* obtenerDireccionLogica(id_objeto tipoObjeto,l_proceso* proceso,uint32_t numero);
 bool memoriaLibre(int cantidadDePaginasNecesarias);
 bool noHayMasTripulantes(t_bitarray* bitArrayTripulantes,uint32_t cantidadDeTripulantes);
 bool noExisteTripulante(l_proceso* proceso,uint32_t numero_tripulante);
 bool laPaginaEstaVacia(l_pagina* pagina);
 void eliminarPagina(l_proceso* proceso,l_pagina* pagina);
 void crearPagina(l_proceso* proceso,t_list* tablaPagina,l_frame* contenidoFrame);
 void limpiarContenidoPagina(t_bitarray* bitArrayContenidoPag,uint32_t posicionInicial,uint32_t longitud);