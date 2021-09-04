#include "mensajes_consola.h"
#include "administrar_tripulantes.h"

void leer_consola(t_log* logger,t_modulo* modulo) {

    pthread_t thread_mensaje;
	
	char* leido = readline(">");
    
	while(strncmp(leido,"", 1) != 0) {
        log_info(logger, leido);
        t_struct_mensajes* t_parametro_mensajes = malloc(sizeof(t_struct_mensajes));
        t_parametro_mensajes->modulo = modulo;
        t_parametro_mensajes->mensaje_completo = string_new();
        string_append(& t_parametro_mensajes->mensaje_completo, leido);
        pthread_create(&thread_mensaje,NULL,(void*)enviar_mensajes_por_consola, t_parametro_mensajes);
	    pthread_detach(thread_mensaje);
        //enviar_mensajes_por_consola(t_parametro_mensajes);
		//free(leido);
		leido = readline(">");
	}

	free(leido);
}

void enviar_mensajes_por_consola(t_struct_mensajes* mensaje){

    char** string_prueba = NULL;
    string_prueba = string_split(mensaje->mensaje_completo," ");

    string_prueba = separar_por_comillas(string_prueba);

    if (string_prueba == NULL){ 
        return;
    }

    int numero_mensaje = obtener_numero_mensaje(string_prueba[0]);

    switch(numero_mensaje){

        case iniciar_patota : crear_patota(mensaje->modulo,string_prueba[1],string_prueba[2],string_prueba);
        break;
        case listar_tripulantes : listar_tripulantes_por_consola();
        break;
//        case iniciar_planificacion : iniciar_planificador();
//        break;
        default : printf("No es un mensaje valido \n");

    }


}