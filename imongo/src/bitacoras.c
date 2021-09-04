#include "bitacoras.h"
#include "filesystem.h"

//char * bitacora_ejemplo = "TAREA_PARAMETROS;POSX;POSY;TIEMPO;";

char * OBTENER_BITACORA(int tripulante){		

        if(tripulante <= 0){

            log_error(logger_imongo, "OCURRIO UN ERROR INGRESE UNA CANTIDAD MAYOR DE 0");
			return NULL;
        }
	
		 int fd;

		char *path_tripulante;

		char * bitacora;

		path_tripulante = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_tripulante, "/Files/Bitacoras/Tripulante%d.ims", tripulante);


		t_files_config * tripulante_config_bitacora;

		if(  verificar_existe_archivo(path_tripulante) == 0 ){

                log_info(logger_imongo, "SE COMPRUEBA QUE EXISTE ARCHIVO  %s" , path_tripulante );

				tripulante_config_bitacora = malloc(sizeof(t_files_config));

				tripulante_config_bitacora->blocks= malloc(sizeof(t_list));

				bitacora_config_loader( tripulante_config_bitacora, path_tripulante);

				bitacora = deserializar_DATA_t_lista( tripulante_config_bitacora->blocks ,tripulante_config_bitacora->size);
				
		}else{
				
                log_info(logger_imongo, "NO EXISTE EL TRIPULANTE SOLICITADO NRO :   %d", tripulante );

				bitacora = NULL;
		}


	return bitacora;
}


void TRIPULANTE_GENERAR_TAREA(int tripulante, char * tarea){

	 TRIPULANTE_config = malloc(sizeof( t_files_config ) );
    
    GENERAR_STRING(tripulante, tarea);
}


t_files_config * inicializar_tripulante(int tripulante){

	 t_files_config * tripu = malloc(sizeof( t_files_config ) );   // le free
      
     crear_Tripulante_en_File_System(tripulante, tripu);  	

	list_destroy_and_destroy_elements(tripu->blocks, (void*)&tripu_clave_destroyer);
	
	return tripu;
}


t_files_config * tripulante_config_loader(t_files_config * tripulante_config, int numero_tripulante){

		char *path_tripulante;
     
		path_tripulante = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_tripulante, "/Files/Bitacoras/Tripulante%d.ims", numero_tripulante);

		bitacora_config_loader( tripulante_config, path_tripulante);
}


void lista_bitacora_destroyer(void* elemento){  }


void serializar_funcion_se_mueve(int inicioX, int inicioY, int destinoX, int destinoY ){  }   // terminarla


void deserializar_TAREA(char * tarea){ }


void bitacora_config_loader(t_files_config * tripulante_config1, char* path_config_file) {

    t_config* config = config_create(path_config_file);

   // tripulante_config1 = malloc(sizeof(t_files_config));   //-----> global tripulante_config

    bitacora_de_tripulante_config_parser(config, tripulante_config1);
    
    config_destroy(config);
}


void bitacora_de_tripulante_config_parser(  t_config * config , t_files_config * tripulante_config) {

	tripulante_config->size =  config_get_int_value(config, "SIZE");

    char** ppLista = config_get_array_value(config, "BLOCKS");

    int index=0;


	t_list * listaFile = list_create();


	while(ppLista[index] != NULL){		

       char * nuevo =   malloc(sizeof(ppLista[index]));

            nuevo = ppLista[index];              

			list_add(listaFile, atoi(nuevo)  );

		index++;

	}


	tripulante_config->blocks = listaFile;

    liberadorDeArrays( ppLista );
}


void liberadorDeArrays(char **array){

    int count=0;

    while(array[count] != NULL){
            free(array[count]);
            count++;
    }

    free(array);
}



void  crear_Tripulante_en_File_System(int numeroTripulante, t_files_config * tripulante_config ){  //PARA CREAR EL IMS. del TRIPULANTE

       	char * tripulante = string_new();

		string_append_with_format(&tripulante, "Tripulante%d.ims", numeroTripulante) ;

		crear_Archivo_Files_TRIPULANTE(punto_de_montaje_GLOBAL , tripulante, tripulante_config  );

		free(tripulante); 
} 



char * crear_Archivo_Files_TRIPULANTE(char * path_punto_montaje , char * NOMBRE_ARCHIVO, t_files_config* tripulante_config  ){    // INICIALIZACION  
        
        int fd;

		char *test;

		test = string_duplicate(path_punto_montaje);

		string_append_with_format(&test, "/Files/Bitacoras/%s", NOMBRE_ARCHIVO);

		

		if(  verificar_existe_archivo(test) == 0 ){
			
				tripulantes_config_data_LOADER(test , tripulante_config );
				
		}else{  //no existe lo creo y cargo	  
		

			if ((fd=open( test, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s.ims", NOMBRE_ARCHIVO );
				}

		      t_tripulantes_config_loader( test  );  //creo e inicio

			  tripulantes_config_data_LOADER(test , tripulante_config );  //cargo a filesystem
	
				close(fd);
			}				

		
 		path_tripulante_actual = string_duplicate(test);
	//	strcpy(path_tripulante_actual , test ) ;

	 return test;

}


void t_tripulantes_config_loader(char* path_config_file_TRIPULANTE) {  //FUNCION INICIALIZADORA

		t_config * t_config_files = config_create(path_config_file_TRIPULANTE);
	
		config_set_value(t_config_files,  "SIZE", "0");
	
		config_set_value(t_config_files, "BLOCKS", "[]");

		config_save(t_config_files);

		config_destroy(t_config_files);
}


void tripulantes_config_data_LOADER(char * path_file, t_files_config* tripulante_config ){  //CARGO EL ARCHIVO AL FILESYSTEM
      
		int fd;

			if ((fd=open( path_file, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_error(logger_imongo, "No se pudo abrir el archivo: %s", path_file );
						return NULL;
				}


    	t_config* config = config_create(path_file);

		//path_tripulante_actual = path_file;

		tripulantes_config_parser(config, tripulante_config);
		
		TRIPULANTE_config = tripulante_config;   //var global
		
		config_destroy(config);

		close(fd);
}


t_list * tripulantes_config_parser(t_config* config, t_files_config * tripulante_config) {

    tripulante_config->size   = config_get_int_value(config, "SIZE");

    char** ppLista= config_get_array_value(config, "BLOCKS");   //REFACTORING DEBE CARGAR INT NO CHARS

    int index=0;

	t_list * listaFile  = list_create();

	while(ppLista[index] != NULL){

			char * nuevo =   malloc(sizeof(ppLista[index]));

            nuevo = ppLista[index];              

			list_add(listaFile, atoi(nuevo)  );

		index++;
	}

    tripulante_config->blocks = listaFile;

    liberadorArrays( ppLista);

	return listaFile;
}


void t_tripulante_config_destroyer(t_files_config * tripu_config){

		list_destroy_and_destroy_elements(tripu_config->blocks, (void*)&tripu_clave_destroyer);

		free(tripu_config->blocks);

		free(tripu_config);
}


void tripu_clave_destroyer(void* elemento){
	char* bb = (char*) elemento;
	free( bb );
}



// ********************AL FILE SYSTEM.c ??-----------------
/* 
void almacenar_en_archivo_tripulante_ims(t_files_config * tripulante_config, char * path_tripulante_ims, int tamanio_buffer, t_list * listadoPrevio){

        int fd;

		t_config * t_config_files = config_create(path_tripulante_ims);


            tripulante_config->size = tripulante_config->size + tamanio_buffer;                


            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", tripulante_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );        


            list_add_all(tripulante_config->blocks , listadoPrevio);              


            char * lista_serializada = serializar_t_lista(tripulante_config->blocks );    

			config_set_value(t_config_files, "BLOCKS", lista_serializada );


			config_save(t_config_files);

			config_destroy(t_config_files);  

}*/

void bitacoras_finally(void){

}

