#include "atender_mensajes.h"

/*
 * @NAME: rnd
 * @DESC: Retorna un entero en el rango [-1, 1]
 */
int rnd() {
	return (rand() % 3) - 1;
}

int dibujado_mapa_miramhq(void) {

	int cols, rows;
	int err;

	err = nivel_gui_inicializar();
	if(err != 0) log_info(logger, "Error nivel_gui_inicializar. [%d] \n",err);

	err = nivel_gui_get_area_nivel(&cols, &rows);
	if(err != 0) log_info(logger, "Error nivel_gui_get_area_nivel. [%d] \n",err);

	nivel = nivel_crear("Nivel inicial");

	err = nivel_gui_dibujar(nivel);
	if(err != 0) log_info(logger, "Error nivel_gui_dibujar. [%d] \n",err);

	while(1){
		sem_wait(sem_dibujar_mapa);
		err = nivel_gui_dibujar(nivel);
		if(err != 0) log_info(logger, "Error nivel_gui_dibujar. [%d] \n",err);
	}
}

void crearPersonajeEnMapa(char tid,uint32_t posicionX,uint32_t posicionY){
        int sePudoCrear = personaje_crear(nivel,tid,posicionX,posicionY);
        if(sePudoCrear != 0){
			char string_log[100] = "";
   			sprintf(string_log, "Error al crear tripulante[%c] en el mapa: %d",tid,sePudoCrear);
            log_info(logger, string_log);
		} 
		sem_post(sem_dibujar_mapa);
}

void moverPersonajeEnMapa(char tid,uint32_t posicionX,uint32_t posicionY){
        int sePudoMover = item_mover(nivel,tid,posicionX,posicionY);
        if(sePudoMover != 0){
			char string_log[100] = "";
   			sprintf(string_log, "Error al mover tripulante[%c] en el mapa: %d",tid,sePudoMover);
            log_info(logger, string_log);
		} 
		sem_post(sem_dibujar_mapa);
}

void eliminarPersonajeDelMapa(uint32_t tid){
        int sePudoBorrar = item_borrar(nivel,(char)tid);
        if(sePudoBorrar != 0){
			char string_log[100] = "";
   			sprintf(string_log, "Error al borrar tripulante[%c] del mapa: %d",tid,sePudoBorrar);
            log_info(logger, string_log);
		} 	        
        sem_post(sem_dibujar_mapa);
}
