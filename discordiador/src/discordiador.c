#include "discordiador.h"
#include "administrar_tripulantes.h"

List * lista_tripulantes;
int sem_inicial = 0;
t_queue * COLA_READY;   // COLA NO ES LIST*
sem_t sem[10];   // AGREGAR DINAMICAMENTE LOS SEMAFOROS 10 COMo MAX
int MAXIMO_ACTUAL; //OBTENER CANTIDAD DE TRIPULANTES O HILOS TOTALES
int QUANTUM;

int main(){
    discordiador_init(&discordiador_config, &logger); //mete en una estructura global los datos de config
    algoritmo = get_algoritmo(discordiador_config);
    sem_init(&s_procesos_en_exec, 0, discordiador_config->grado_multitarea);
    
    modulo_memoria =
    crear_modulo_miram(discordiador_config->ip_mi_ram_hq, discordiador_config-> puerto_mi_ram_hq);
   
    modulo_file_system =
    crear_modulo_imongo(discordiador_config->ip_i_mongo_store, discordiador_config->puerto_i_mongo_store);
   
    if (modulo_memoria == NULL || modulo_file_system == NULL){
     log_error(logger, "%s\n", "El modulo indicado no existe.");
     finally(discordiador_config, logger);
     return -1;
    }
    
    pthread_t p_planificador;
	pthread_create(&p_planificador, NULL, (void*)hilo_planificador, NULL);
	pthread_detach(p_planificador);

    pthread_t p_bloqueados;
	pthread_create(&p_bloqueados, NULL, (void*)hilo_bloqueados, NULL);
	pthread_detach(p_bloqueados);

    pthread_t hilo_consola;
	pthread_create(&hilo_consola, NULL, (void*)hilo_leer_consola, NULL);
	pthread_join(hilo_consola,NULL);

    finally(discordiador_config,logger);

    return 0;
}

void hilo_leer_consola()
{ leer_consola(logger,modulo_memoria);
}

void discordiador_init(t_discordiador_config** discordiador_config, t_log** logger){
    *discordiador_config = discordiador_config_loader("./cfg/discordiador.config", logger);

    //Inicializo semáforos
    sem_init(&mutex_id_tripulantes,0,1);
    sem_init(&s_cola_ready_con_items, 0, 0);
    sem_init(&s_cola_bloqueados_con_items, 0, 0);
    sem_init(&s_iniciar_planificacion, 0 ,0);
    pthread_mutex_init(&mutex_cola_ready, NULL);
    
    //Inicializo lista de tripulantes
    lista_patotas = list_create();
    cola_ready = list_create();    //------>>>>> t_queue 
    cola_blocked = list_create();
}

t_discordiador_config* discordiador_config_loader(char* path_config_file, t_log** logger) {
    t_config* config = config_create(path_config_file);
    t_discordiador_config* discordiador_config = malloc(sizeof(t_discordiador_config));

    discordiador_config_parser(config, discordiador_config);
    config_destroy(config);
    
    *logger = log_create("./cfg/discordiador.log", "DISCORDIADOR", true, LOG_LEVEL_INFO);

    return discordiador_config;
}

void discordiador_config_parser(t_config* config, t_discordiador_config* discordiador_config) {
    discordiador_config->ip_mi_ram_hq = strdup(config_get_string_value(config, "IP_MI_RAM_HQ"));
    discordiador_config->puerto_mi_ram_hq = strdup(config_get_string_value(config, "PUERTO_MI_RAM_HQ"));
    discordiador_config->ip_i_mongo_store = strdup(config_get_string_value(config, "IP_I_MONGO_STORE"));
    discordiador_config->puerto_i_mongo_store = strdup(config_get_string_value(config, "PUERTO_I_MONGO_STORE"));
    discordiador_config->grado_multitarea = config_get_int_value(config, "GRADO_MULTITAREA");
    discordiador_config->algoritmo = strdup(config_get_string_value(config, "ALGORITMO"));
    discordiador_config->quantum = config_get_int_value(config, "QUANTUM");
    discordiador_config->duracion_sabotaje = config_get_int_value(config, "DURACION_SABOTAJE");
    discordiador_config->retardo_ciclo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");
}

t_modulo * crear_modulo_miram(char* ip, char* puerto){
    t_modulo* modulo = malloc(sizeof(t_modulo));
    modulo->ip = ip;
    modulo->puerto = puerto;
    modulo->socket = 0;
    modulo->identificacion = "MiRamHQ";
    return modulo;
}

t_modulo * crear_modulo_imongo(char* ip, char* puerto){
    t_modulo* modulo = malloc(sizeof(t_modulo));
    modulo->ip = ip;
    modulo->puerto = puerto;
    modulo->socket = 0;
    modulo->identificacion = "FileSystem";
    return modulo;
}

void finally(t_discordiador_config* cliente_config, t_log* logger) {
    discordiador_destroy(cliente_config);
    log_destroy(logger);
}

void discordiador_destroy(t_discordiador_config* cliente_config) {
    free(cliente_config->ip_mi_ram_hq);
    free(cliente_config->puerto_mi_ram_hq);
    free(cliente_config->ip_i_mongo_store);
    free(cliente_config->puerto_i_mongo_store);
    free(cliente_config->algoritmo);
    free(cliente_config);
}

void hilo_planificador(){
	while(1){
		sem_wait(&s_cola_ready_con_items);//inicializado en 0
		log_info(logger,"Llegó algo a la cola de READY\n");
        planificar();
	}
}

void planificar()
{
	switch(algoritmo.algoritmo_code){
        case FIFO:
            planificar_fifo();
        break;
        case RR:
            planificar_round_robin();
        break;
   }
}

void planificar_fifo(){
	pthread_mutex_lock(&mutex_cola_ready);
	t_tripulante* tripulante = list_remove(cola_ready, 0);
	pthread_mutex_unlock(&mutex_cola_ready);
    // poner en la cola de exec

	log_info(logger, "El próximo tripulante a planificar es %d porque es el primero en la cola de READY\n", tripulante->id_tripulante);
	
    tripulante->estado = EXEC;
    sem_post(&tripulante->s_tripulante);
}


void inicializar_semaforos(){    // ESTA FUNCION DEBERIA ESTAR EN ALGUN LADO EJECUTANDOSE UNA SOLA VEZ AL INICIAR EL ROUND ROBIN
    int i;
    int primero = 0;
    int CANTIDAD_TRIPULANTES;

        for(i=0;i<CANTIDAD_TRIPULANTES;i++){  //SACAR LA CANTIDAD_TRIPULANTES DE ALGUN LADO

            if(primero==0){
                primero=1;
                 sem_init(&sem[i],0,1);
            }else{
                sem_init(&sem[i],0,0);
            }
        }

}

void planificar_FIFO_REVISION(){
//---------------------- POTENCIAL FIFO ---------ejecuta en ORDEN-----------------
t_tripulante * tripulante = (t_tripulante*)queue_pop(COLA_READY); 

    int semaforo_actual =  tripulante->id_tripulante;
    int i;
   
    sem_wait(&sem[semaforo_actual]);

    //SECCION CRITICA
    ejecutarTripulante(tripulante);    //------------->>>>>>>>> UNA REVERSION DE EJECUTAR
   
    semaforo_actual++;
     if(semaforo_actual>MAXIMO_ACTUAL){
        semaforo_actual=0;
    }
    sem_post(&sem[semaforo_actual]);

}

void planificar_ROUND_ROBIN(){



}

//   controlen el quantum después de ejecutar cada tarea, 
//  si llega a 0 desalojan, si están con un valor mayor a cero ejecutan otra tarea 
   // lista_quantum


void ejecutarTripulante(t_tripulante* tripulante){
    
     log_info(logger, "Planificando hilo del tripulante %d\n",tripulante->id_tripulante);

     realizar_tarea(tripulante);

     char* tarea = proxima_tarea_a_realizar(tripulante);
    
     while ( tarea != NULL && tripulante->estado != BLOCKED){
    
      tripulante->tarea = procesar_proxima_tarea(tarea);
    
      realizar_tarea(tripulante);
    
      tarea = proxima_tarea_a_realizar(tripulante);
    }
}

int32_t get_algoritmo_code(char* algoritmo){

	if(string_equals_ignore_case(algoritmo, "RR")){
		return RR;
	} else if (string_equals_ignore_case(algoritmo, "FIFO")){
		return FIFO;
	}

	return 0;
}

t_algoritmo get_algoritmo(t_discordiador_config* config){
    t_algoritmo algoritmo;

	algoritmo.algoritmo_string = discordiador_config->algoritmo;
	algoritmo.algoritmo_code = get_algoritmo_code(algoritmo.algoritmo_string);
	algoritmo.retardo = discordiador_config->retardo_ciclo_cpu;

	if(algoritmo.algoritmo_code == RR){
		algoritmo.quantum = discordiador_config->quantum;
	} else {
		algoritmo.quantum = 0;
	}
	return algoritmo;
}

void hilo_bloqueados(){
	while(1){
		sem_wait(&s_cola_bloqueados_con_items);//inicializado en 0
		log_info(logger,"Llegó algo a la cola de BLOQUEADOS\n");
        administrar_cola_io();
	}
}

void administrar_cola_io()
{
    pthread_mutex_lock(&mutex_cola_bloqueados);
	t_tripulante* tripulante = list_remove(cola_blocked, 0);
	pthread_mutex_unlock(&mutex_cola_bloqueados);
    
    tripulante->estado = READY;
    list_add(cola_ready,tripulante);
}