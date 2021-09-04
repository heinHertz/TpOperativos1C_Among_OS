#include "atender_mensajes.h"

uint32_t administrador_tabla_segmento(uint32_t tamanio, uint32_t id, admin_segmentacion cod){

	//las direcciones logicas, los primeros 3 son el id dado vuelta, y los demas el desplazamiento

	t_segmento* segmento = malloc(sizeof(t_segmento));
	t_segmento* segmentoAux;

	bool buscar_tripulante(t_segmento* s){
	return s->id_tripulante == id;
	}
	
	bool buscar_patota(t_segmento* s){
			return s->id_patota == id;
	}

	void destructor(t_segmento* s){
		free(s);
	}

	bool buscar_tareas(t_segmento* s){
		return s->tareas_patota == id;
	}

	switch (cod) {
	case NUEVO_PCB:

		segmento->base = buscarBase(tamanio); //si llega que hay que compactar. Compactar aca y buscar base de nuevo. Y actualizar la DL de las tareas
		segmento->limite = tamanio;
		segmento->id = generador_id_segmento();
		segmento->id_patota = id;
		segmento->id_tripulante = 0;
		segmento->tareas_patota = 0;

		list_add(tabla_segmentos, segmento);

		return generarDireccionLogica(segmento->id, 0);

		break;

	case NUEVAS_TAREAS:

		segmento->base = buscarBase(tamanio);
		segmento->limite = tamanio;
		segmento->id = generador_id_segmento();
		segmento->id_patota = 0;
		segmento->id_tripulante = 0;
		segmento->tareas_patota = id;

		list_add(tabla_segmentos, segmento);

		return generarDireccionLogica(segmento->id, 0);

		break;

	case NUEVO_TCB:

		segmento->base = buscarBase(sizeof(t_tripulante));
		segmento->limite = sizeof(t_tripulante);
		segmento->id = generador_id_segmento();
		segmento->id_patota = 0;
		segmento->id_tripulante = id;
		segmento->tareas_patota = 0;

		list_add(tabla_segmentos, segmento);

		return generarDireccionLogica(segmento->id, 0);

		break;

	case BORRAR:

		list_remove_and_destroy_by_condition(tabla_segmentos, (void*)buscar_tripulante, (void*)destructor);

		free(segmento);

		return 0;

		break;

	case BUSCAR_TRIPULANTE:

		segmentoAux = list_find(tabla_segmentos, (void*)buscar_tripulante);
		return generarDireccionLogica(segmentoAux->id, 0); //si no encuentra da segfault, validar eso

		break;

	case BUSCAR_PATOTA:

		segmentoAux = list_find(tabla_segmentos, (void*)buscar_patota);
		return generarDireccionLogica(segmentoAux->id, 0);

		break;

	case TAMANIO_TAREAS:

		segmentoAux = list_find(tabla_segmentos, (void*)buscar_tareas);
		return segmentoAux->limite;

		break;

    default:
        return 0;    

	}

}


void crear_PCB(t_tareas_patota* tareas_de_patota){

	t_patota PCB;
	t_tripulante* tripulante;
	uint32_t cant_tripulantes = list_size(tareas_de_patota->tripulantes);

	PCB.id = tareas_de_patota->patota;
	PCB.segmentoTareas = crear_tareas(tareas_de_patota); // numSegmento +0 , porque es donde empieza. Devolver -1 si no hay lugar


	uint32_t direccionLogica = administrador_tabla_segmento(8, PCB.id, NUEVO_PCB); //aca diferenciar entre seg y pag

	uint32_t direccion_fisica = calcular_direccion_fisica(direccionLogica);

	void* a_guardar = malloc(sizeof(t_patota));

	int desplazamiento = 0;

	memcpy(a_guardar + desplazamiento, &PCB.id, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_guardar + desplazamiento, &PCB.segmentoTareas, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	guardar_en_memoria(direccion_fisica, a_guardar, sizeof(t_patota)); //Aca termina el proceso de crear un PCB

	for(uint32_t i=0; i<cant_tripulantes; i++){

		tripulante = list_get(tareas_de_patota->tripulantes, i);
		crear_TCB(*tripulante);// verificar si entra todo (antes y despues de compactar), si no denegar
	}

	free(a_guardar);

}

uint32_t crear_tareas(t_tareas_patota* tareas_de_patota){

	char* tareas_a_guardar = generar_string_tareas(tareas_de_patota->tareas);

	uint32_t tamanio_que_se_guarda = string_length(tareas_a_guardar);

	uint32_t direccionLogica = administrador_tabla_segmento(tamanio_que_se_guarda, tareas_de_patota->patota, NUEVAS_TAREAS); // creo el segmento y lo agrego a la tabla.

	uint32_t direccion_fisica = calcular_direccion_fisica(direccionLogica);

	guardar_en_memoria(direccion_fisica, tareas_a_guardar, strlen(tareas_a_guardar));

	free(tareas_a_guardar);
	return direccionLogica;
}

void crear_TCB(t_tripulante tripulante){


	uint32_t direccionLogica = administrador_tabla_segmento(0, tripulante.id, NUEVO_TCB);

	uint32_t direccion_fisica= calcular_direccion_fisica(direccionLogica);

	uint32_t DL_PCB = administrador_tabla_segmento(0, tripulante.patota, BUSCAR_PATOTA);

	void* a_guardar = malloc(sizeof(t_tripulante));
	uint32_t desplazamiento =0;

	memcpy(a_guardar +desplazamiento, &(tripulante.id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_guardar +desplazamiento, &(tripulante.estado), sizeof(char));
	desplazamiento += sizeof(char);
	memcpy(a_guardar +desplazamiento, &(tripulante.posicionX), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_guardar +desplazamiento, &(tripulante.posicionY), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_guardar +desplazamiento, &(tripulante.proximaTarea), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_guardar +desplazamiento, &DL_PCB, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	guardar_en_memoria(direccion_fisica, a_guardar, sizeof(t_tripulante));



	free(a_guardar);
}

void actualizar_posicion(t_movimiento posicion){

	uint32_t direccionLogica = administrador_tabla_segmento(0, posicion.id, BUSCAR_TRIPULANTE);

	uint32_t direccion_fisica= calcular_direccion_fisica(direccionLogica);

	uint32_t posX = 5, posY = 9;

	memcpy(puntero_memoria_principal + direccion_fisica + posX, &(posicion.X), sizeof(uint32_t));
	memcpy(puntero_memoria_principal + direccion_fisica + posY, &(posicion.Y), sizeof(uint32_t));

}

void actualizar__estado(char estado, uint32_t id){

	uint32_t direccionLogica = administrador_tabla_segmento(0, id, BUSCAR_TRIPULANTE);

	uint32_t direccion_fisica= calcular_direccion_fisica(direccionLogica);

	uint32_t est = 4;

	memcpy(puntero_memoria_principal + direccion_fisica + est, &estado, sizeof(uint32_t));


}

void actualizar_prox_tarea(uint32_t DL_tarea, uint32_t id){

	uint32_t direccionLogica = administrador_tabla_segmento(0, id, BUSCAR_TRIPULANTE);

	uint32_t direccion_fisica= calcular_direccion_fisica(direccionLogica);

	uint32_t proxTarea = 13;

	memcpy(puntero_memoria_principal + direccion_fisica + proxTarea, &DL_tarea, sizeof(uint32_t));

}

void actualizar_patota(uint32_t DL_tareas, uint32_t id_patota){

	uint32_t direccionLogica = administrador_tabla_segmento(0, id_patota, BUSCAR_PATOTA);

	uint32_t direccion_fisica= calcular_direccion_fisica(direccionLogica);

	uint32_t tareas = 4;

	memcpy(puntero_memoria_principal + direccion_fisica + tareas, &DL_tareas, sizeof(uint32_t));

}

void borrar_TCB(uint32_t id){

	administrador_tabla_segmento(0, id, BORRAR);

}

t_tarea buscar_proxima_tarea(uint32_t id){//devulve -1 en id tarea si no hay mas tareas

	uint32_t DL_tripulante = administrador_tabla_segmento(0, id, BUSCAR_TRIPULANTE);
	uint32_t DF_tripulante = calcular_direccion_fisica(DL_tripulante);

	uint32_t patota = 17, tareaRealizada=13, DL_tareaRealizada, DL_PCB;
	memcpy(&DL_PCB, puntero_memoria_principal + DF_tripulante + patota , sizeof(uint32_t));
	memcpy(&DL_tareaRealizada, puntero_memoria_principal + DF_tripulante+ tareaRealizada , sizeof(uint32_t));

	uint32_t DF_PCB = calcular_direccion_fisica(DL_PCB);

	uint32_t id_patota, DL_tareas;
	memcpy(&id_patota, puntero_memoria_principal + DF_PCB , sizeof(uint32_t));
	memcpy(&DL_tareas, puntero_memoria_principal + DF_PCB + 4 , sizeof(uint32_t));

	uint32_t DF_tareas = calcular_direccion_fisica(DL_tareas);
	uint32_t tamanio_tareas = administrador_tabla_segmento(0, id_patota, TAMANIO_TAREAS);

	char* tareas= malloc(tamanio_tareas);
	memcpy(tareas, puntero_memoria_principal + DF_tareas, tamanio_tareas);

	if(DL_tareaRealizada == 0){

		uint32_t idSeg = obtenerId(DL_tareas);
		uint32_t proxTarea = generarDireccionLogica(idSeg ,0);

		char* tareaSig = primerTarea(tareas);

		actualizar_prox_tarea(proxTarea, id);

		return obtener_tarea(tareaSig);

	}else{

		uint32_t desplazamiento = obtenerDesplazamiento(DL_tareaRealizada);

		char* tareaSig = string_substring_from(tareas, desplazamiento); //esto no deberia modificar el tamaño de tareas. chequear

		uint32_t contador = desplazamiento;

		contador += strlen(primerTarea(tareaSig)) +1;

		tareaSig = string_substring_from(tareas, contador);

		if(strcmp(tareaSig, "\0") == 0){

			t_tarea tarea;
			tarea.id = -1;
			return tarea;
		}


		uint32_t idSeg = obtenerId(DL_tareas);
		uint32_t proxTarea = generarDireccionLogica(idSeg, contador);

		tareaSig = primerTarea(tareaSig);

		actualizar_prox_tarea(proxTarea, id);

		return obtener_tarea(tareaSig);
	}


}


uint32_t compactar(){

	uint32_t base_hueco;
	t_segmento* segmento ;

	bool ordenador(t_segmento* s1, t_segmento* s2){
		return s1->base < s2->base;
	}
	list_sort(tabla_segmentos, (void*)ordenador);

	
	do{
		base_hueco  = buscar_siguiente_hueco();

		segmento = buscar_siguiente_segmento(base_hueco);

		memcpy(puntero_memoria_principal + base_hueco, puntero_memoria_principal + segmento->base, sizeof(segmento->limite));

		segmento->base = base_hueco; //actualizo nueva base

	}
	while(base_hueco < segmento->base);

	list_sort(tabla_segmentos, (void*)ordenador);
	base_hueco  = buscar_siguiente_hueco();
	return tamanioMemoria - base_hueco;


}

uint32_t buscar_siguiente_hueco(){
	t_segmento* segmento;
	uint32_t base, limite;
	

	for(int i=0; i < list_size(tabla_segmentos); i++){
		
		segmento = list_get(tabla_segmentos, i);

		base = segmento->base;
		limite = segmento->limite;

		segmento = list_get(tabla_segmentos, i+1); 
		if(segmento == NULL) segmento->base = tamanioMemoria;

		if(i == 0 && base != 0) return 0;

		if(segmento->base != (base + limite)){

			return (base + limite);
		} 


	}

	return -1;
}

t_segmento* buscar_siguiente_segmento(uint32_t base_hueco){

	bool siguiente(t_segmento* s){
		return s->base > base_hueco;
	}
	t_segmento* seg = list_find(tabla_segmentos, (void*)siguiente);
	return seg;
}


//==============================Funciones Auxiliares=======================================================================

uint32_t generarDireccionLogica(uint32_t id, uint32_t desplazamiento){

	uint32_t ID;
	ID = dar_vuelta_id(id);

	char* ID_STRING = string_itoa(ID);
	char* DESPLAZAMIENTO =  string_itoa(desplazamiento);

	char* a = string_new();
		string_append(&a, ID_STRING);
		string_append(&a, DESPLAZAMIENTO);

	uint32_t direccionLogica = atoi(a);
	return direccionLogica;
}


uint32_t obtenerId(uint32_t num){


	char* id = string_new();

	uint32_t id_retornado;

	id = string_itoa(num);
	id  = string_substring(id, 0, 3);

	id = string_reverse(id);


	return id_retornado = atoi(id);

}

uint32_t obtenerDesplazamiento(uint32_t num){

		char* id = string_new();

		uint32_t id_retornado;

		id = string_itoa(num);
		id =  string_substring_from(id, 3);

		return id_retornado = atoi(id);

}

uint32_t dar_vuelta_id(uint32_t num){

	char* id = string_new();

	uint32_t id_retornado;
	id = string_itoa(num);
	id = string_reverse(id);
	uint32_t veces = 3 - string_length(id);

	char* ceros = string_repeat('0', veces);

	char* a = string_new();
	string_append(&a, id);
	string_append(&a, ceros);

	return id_retornado = atoi(a);

}

uint32_t calcular_direccion_fisica(uint32_t direccionLogica){

	uint32_t direccionFisica;

	uint32_t id = obtenerId(direccionLogica);

	uint32_t desplazamiento = obtenerDesplazamiento(direccionLogica);

	if(strcmp(esquemaMemoria, "SEGMENTACION") == 0){

		bool buscarIdsegmento(t_segmento* s){
			return s->id == id;
		}

		t_segmento* segmento = list_find(tabla_segmentos,(void*)buscarIdsegmento);


		direccionFisica = segmento->base + desplazamiento;
	}else{

	}


	return direccionFisica;
}

void guardar_en_memoria(uint32_t direccionFisica, void* stream_a_guardar, uint32_t tam){

	memcpy(puntero_memoria_principal + direccionFisica, stream_a_guardar, tam);


}


char* generar_string_tareas(t_list* tareas){

	char* tareas_a_guardar = string_new();


	void iterarTareas(t_tarea* t){

		switch(t->id){
		case GENERAR_OXIGENO:
			string_append(&tareas_a_guardar, "GENERAR_OXIGENO ");
		break;
		case CONSUMIR_OXIGENO:
			string_append(&tareas_a_guardar, "CONSUMIR_OXIGENO ");
		break;
		case GENERAR_COMIDA:
			string_append(&tareas_a_guardar, "GENERAR_COMIDA ");
		break;
		case CONSUMIR_COMIDA:
			string_append(&tareas_a_guardar, "CONSUMIR_COMIDA ");
		break;
		case GENERAR_BASURA:
			string_append(&tareas_a_guardar, "GENERAR_BASURA ");
		break;
		case DESCARTAR_BASURA:
			string_append(&tareas_a_guardar, "DESCARTAR_BASURA ");
		break;
		}


		string_append(&tareas_a_guardar,  string_itoa(t->cantidad));
		string_append(&tareas_a_guardar, ";");
		string_append(&tareas_a_guardar, string_itoa(t->posicionX));
		string_append(&tareas_a_guardar, ";");
		string_append(&tareas_a_guardar, string_itoa(t->posicionY));
		string_append(&tareas_a_guardar, ";");
		string_append(&tareas_a_guardar, string_itoa(t->duracion));
		string_append(&tareas_a_guardar, "|");
	}

	list_iterate(tareas, (void*)iterarTareas);


	return tareas_a_guardar;
}


uint32_t generador_id_segmento(){

	uint32_t id = id_segmento;

	if(id%10 == 0){ // no pueden ser multiplos de 10 porque al darlos vuelta lo toma como octal
		id++;
		id_segmento++;
	}
	id_segmento++;
	return id;

}



uint32_t buscarBase(uint32_t tamanio){

	uint32_t hueco;
	uint32_t cant_segmentos = list_size(tabla_segmentos) - 1;
	uint32_t vuelta = 0;

	if(list_size(tabla_segmentos) == 0){

		return 0;

	}else{

		t_segmento* base_anterior = malloc(sizeof(t_segmento));
		t_segmento* base_siguiente ;

		bool ordenador(t_segmento* s1, t_segmento* s2){
					return s1->base < s2->base;
		}
		list_sort(tabla_segmentos, (void*)ordenador); //Ordena la lista para buscar por orden

		base_anterior->base = 0;
		base_anterior->limite = 0;

		 do{

			bool baseSiguiente(t_segmento* segmento) {

				return segmento->base > base_anterior->base;

			}
			base_siguiente = list_find(tabla_segmentos, (void*) baseSiguiente);

			if(base_siguiente == NULL){

		        base_siguiente = malloc(sizeof(t_segmento));
				base_siguiente->base = tamanioMemoria;
			}

			//si la lista tiene un elemento, que el hueco sea el max de memoria hasta el limite del primer segmento
			bool baseCero (t_segmento* s){
				return s->base == 0;
			}

			if(vuelta == 0 && list_find(tabla_segmentos, (void*)baseCero) != NULL){
				base_anterior = list_find(tabla_segmentos, (void*)baseCero);
			}

			hueco = base_siguiente->base
					- (base_anterior->base + base_anterior->limite);



			base_anterior = base_siguiente;
			cant_segmentos--;
			vuelta++;

		 }while (hueco < tamanio || cant_segmentos <= 0 );//menor a 0?


		if(hueco < tamanio) { //si ya recorrio toda la tabla y no encontro lugar
			//devolver que se necesita compactar y probar de nuevo
		}else{
		return base_siguiente->base - hueco;
		}

		free(base_anterior);
		free(base_siguiente);
	}

    return 0;
}

char* primerTarea(char* tareas){

	/*char** spliteado = string_new();
	spliteado = string_split(tareas, "|");*/

    char** spliteado = string_split(tareas, "|");

	return spliteado[0];

}

t_tarea obtener_tarea(char* tarea){

	t_tarea tarea_armada;

	char** spliteado = string_split(tarea, " ");

	if(strcmp(spliteado[0], "GENERAR_OXIGENO")==0){
		tarea_armada.id = GENERAR_OXIGENO;
	}
	if(strcmp(spliteado[0], "CONSUMIR_OXIGENO")==0){
		tarea_armada.id = CONSUMIR_OXIGENO;
	}
	if(strcmp(spliteado[0], "GENERAR_COMIDA")==0){
		tarea_armada.id = GENERAR_COMIDA;
	}
	if(strcmp(spliteado[0], "CONSUMIR_COMIDA")==0){
		tarea_armada.id = CONSUMIR_COMIDA;
	}
	if(strcmp(spliteado[0], "GENERAR_BASURA")==0){
		tarea_armada.id = GENERAR_BASURA;
	}
	if(strcmp(spliteado[0], "DESCARTAR_BASURA")==0){
		tarea_armada.id = DESCARTAR_BASURA;
	}

	char** spliteado2 = string_split(spliteado[1], ";");

	tarea_armada.cantidad = atoi(spliteado2[0]);
	tarea_armada.posicionX = atoi(spliteado2[1]);
	tarea_armada.posicionY = atoi(spliteado2[2]);
	tarea_armada.duracion = atoi(spliteado2[3]);

	return tarea_armada;

}