#include "imongo_config.h"



void imongo_init(void){   

    log_info(logger_imongo, "INICIALIZANDO IMONGO");

     imongo_config_loader("./cfg/imongo.config");

}


void imongo_config_loader(char* path_config_file) {

    t_config* config = config_create(path_config_file);

    imongo_config = malloc(sizeof(t_imongo_config));

    imongo_config_parser(config, imongo_config);
    
    config_destroy(config);

}



void imongo_config_parser(t_config* config, t_imongo_config* imongo_config) {

    imongo_config->punto_montaje = strdup(config_get_string_value(config, "PUNTO_MONTAJE"));

    punto_de_montaje_GLOBAL = string_duplicate(  imongo_config->punto_montaje    );


    imongo_config->puerto_escucha =  strdup(config_get_string_value(config, "PUERTO"));  
    imongo_config->tiempo_sincronizacion =  config_get_int_value(config, "TIEMPO_SINCRONIZACION");


    char** ppLista= config_get_array_value(config, "POSICIONES_SABOTAJE");

    int index=0;

	t_list * lista  = list_create();

	while(ppLista[index] != NULL){

        char * element =   malloc(sizeof(ppLista[index]));
           
        strcpy(element, ppLista[index] );   

		list_add(lista, element );

		index++;
	}

    imongo_config->list_posiciones_sabotaje = lista;

    liberadorArrays( ppLista);


}

void liberadorArrays(char **array){

    int count=0;

    while(array[count] != NULL){
            free(array[count]);
            count++;
    }
    free(array);

}

void clave_destroyer(void* elemento){
	char* bb = (char*) elemento;
	free( bb );
	//list_destroy_and_destroy_elements(bloqueados_por_clave->bloqueados, blocked_destroyer);
}

void imongo_finally(t_imongo_config* imongo_config, t_log* logger_imongo) {
    imongo_destroy(imongo_config);
    log_destroy(logger_imongo);
}


void imongo_destroy(t_imongo_config* imongo_config) {
    free(imongo_config->puerto_escucha);
    free(imongo_config->punto_montaje);
   // free(imongo_config->tiempo_sincronizacion);

 // list_destroy_and_destroy_elements(lista, (void*)&clave_destroyer);


   // free(imongo_config->list_posiciones_sabotaje);
    free(imongo_config);
}


void var_char_destroyers(){
    
        free(punto_de_montaje_GLOBAL );
    //free(path_del_BLOCKS_IMS_GLOBAL);

}