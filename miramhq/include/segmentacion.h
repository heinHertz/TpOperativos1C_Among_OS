#ifndef MIRAMHQ_AUXILIARES_H_
#define MIRAMHQ_AUXILIARES_H_


typedef struct{
	uint32_t id; //tiene que ser unico por cada segmento
	uint32_t base;
	uint32_t limite;
	uint32_t id_patota;//estos 3 ultimos para buscar en la tabla de segmentos;
	uint32_t id_tripulante;
	uint32_t tareas_patota;
}t_segmento;

typedef struct{
	uint32_t id;
	uint32_t patota;
	char estado;
	uint32_t posicionX;
	uint32_t posicionY;
	uint32_t proximaTarea;
}t_tripulante;

typedef struct{
	uint32_t id;
	uint32_t segmentoTareas;
}t_patota;

typedef struct{
	uint32_t X;
	uint32_t Y;
	uint32_t id;
}t_movimiento;

typedef struct{
	uint32_t patota;
	t_list* tareas;
	t_list* tripulantes;
}t_tareas_patota;

typedef enum{

	NUEVO_PCB,
	NUEVO_TCB,
	NUEVAS_TAREAS,
	TAMANIO_TAREAS,
	BORRAR,
	BUSCAR_TRIPULANTE,
	BUSCAR_PATOTA

}admin_segmentacion;

t_list* tabla_segmentos;
uint32_t id_segmento;


uint32_t administrador_tabla_segmento(uint32_t , uint32_t , admin_segmentacion );
void inicializar_memoria_principal();
void crear_PCB(t_tareas_patota*);
uint32_t crear_tareas(t_tareas_patota*);
void crear_TCB(t_tripulante );
void actualizar_posicion(t_movimiento );
void actualizar__estado(char, uint32_t);
void actualizar_prox_tarea(uint32_t, uint32_t );
void actualizar_patota(uint32_t, uint32_t);
uint32_t compactar();
uint32_t buscar_siguiente_hueco();
t_segmento* buscar_siguiente_segmento(uint32_t);




uint32_t calcular_direccion_fisica(uint32_t);
void guardar_en_memoria(uint32_t, void*, uint32_t);
char* generar_string_tareas(t_list*);
uint32_t generarDireccionLogica(uint32_t, uint32_t );
uint32_t generador_id_segmento();
uint32_t obtenerId(uint32_t);
uint32_t obtenerDesplazamiento(uint32_t );
uint32_t buscarBase(uint32_t);
uint32_t dar_vuelta_id(uint32_t );

char* primerTarea(char* );


#endif /* MIRAMHQ_AUXILIARES_H_ */