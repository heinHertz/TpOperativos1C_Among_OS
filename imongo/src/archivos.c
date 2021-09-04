#include "archivos.h"



void crear_archivos_recursos_files(void){

		char * oxigeno = "Oxigeno";

		char * comida = "Comida";

		char * basura = "Basura";

	

       path_oxigeno =  crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , oxigeno );

       path_comida = crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , comida );

       path_basura = crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , basura );	
	   	 
}


char * crear_Archivo_Files_ARCHIVO(char * path_punto_montaje , char * NOMBRE_ARCHIVO ){    // INICIALIZACION  
        
        int fd;

		char *test;

		test = string_duplicate(path_punto_montaje);

		string_append_with_format(&test, "/Files/%s.ims", NOMBRE_ARCHIVO);


		if(  verificar_existe_archivo(test) == 0 ){

				//SI EXISTE CARGARLO
				files_config_data_LOADER(test , NOMBRE_ARCHIVO );

                log_info(logger_imongo, "SE COMPRUEBA QUE EXISTE ARCHIVO  %s.ims" , NOMBRE_ARCHIVO );
				
		}else{  //no existe lo creo y cargo
			  
				log_info(logger_imongo, "INTENTANDO CREAR UN NUEVO ARCHIVO DE: %s.ims", NOMBRE_ARCHIVO );

			if ((fd=open( test, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s.ims", NOMBRE_ARCHIVO );
				}

		      instanciar_FILES_RECURSO( test , NOMBRE_ARCHIVO );  //creo e inicio

			  files_config_data_LOADER(test , NOMBRE_ARCHIVO );  //cargo a filesystem
			  

		 	if (  verificar_existe_archivo(test) == 0 )
				{
                        log_info( logger_imongo, "SE CREO CON EXITO EL ARCHIVO: %s.ims", NOMBRE_ARCHIVO);							
				}else{
							log_info( logger_imongo, "ERROR AL CREAR EL ARCHIVO: %s.ims", NOMBRE_ARCHIVO);
				}

				close(fd);

			}				
		
	 return test;

}



void instanciar_FILES_RECURSO( char * path_archivo_RECURSO , char * nombre ){   //DESPUES DE QUE SE CREAR LOS FILES   CARGA
																				//  REFACTORIZAR, METER CONFIG DIRECTAMENTE

    t_files_config_loader(path_archivo_RECURSO, nombre);      

}


void files_config_data_LOADER(char * path_file, char * nombre){  //CARGO EL ARCHIVO AL FILESYSTEM
      
		int fd;

			if ((fd=open( path_file, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_error(logger_imongo, "No se pudo abrir el archivo: %s", path_file );
						close(fd);
						return NULL;
				}


    	t_config* config = config_create(path_file);


		if( strcmp(nombre , "Oxigeno" ) == 0 ){ 
			OXIGENO_config = malloc(sizeof(t_files_config));
			lista_Oxigeno = malloc(sizeof(t_list));
			lista_Oxigeno = files_config_parser(config, OXIGENO_config );
		} 


		if( strcmp(nombre , "Comida" ) == 0 ){ 
			COMIDA_config = malloc(sizeof(t_files_config));
			lista_Comida = malloc(sizeof(t_list));
			lista_Comida = files_config_parser(config, COMIDA_config  );			
		} 


		if( strcmp(nombre , "Basura" ) == 0 ){ 
			BASURA_config = malloc(sizeof(t_files_config));
			lista_Basura = malloc(sizeof(t_list));
			lista_Basura = files_config_parser(config, BASURA_config ); 
		} 
    
    config_destroy(config);

	close(fd);

}



t_list * files_config_parser(t_config* config, t_files_config* file_config ) {

    file_config->block_count   = config_get_int_value(config, "BLOCK_COUNT");

    file_config->CARACTER_LLENADO   = config_get_string_value(config, "CARACTER_LLENADO");

    file_config->size   = config_get_int_value(config, "SIZE");
	//*size_total_config = file_config->size;  // var global actual


	file_config->MD5_ARCHIVO   = strdup(config_get_string_value(config, "MD5_ARCHIVO"));


    char** ppLista= config_get_array_value(config, "BLOCKS");   //REFACTORING DEBE CARGAR INT NO CHARS

    int index=0;

	t_list * listaFile  = list_create();

	while(ppLista[index] != NULL){

			char * nuevo =   malloc(sizeof(ppLista[index]));

            nuevo = ppLista[index];              

			list_add(listaFile, atoi(nuevo)  );

		index++;
	}

    file_config->blocks  = listaFile;

    liberadorArrays( ppLista);

	return listaFile;

}

void liberarArrays(char **array){

    int count=0;

    while(array[count] != NULL){
            free(array[count]);
            count++;
    }
    free(array);

}


int CONSUMIR_RECURSO_instanciador_de_archivo( t_files_config * RECURSO_config , int cantidad  ,  char * NOMBRE_ARCHIVO ){   

	    int fd;

		char *test;

     
		test = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&test, "/Files/%s.ims", NOMBRE_ARCHIVO);   ///-******


		if(  verificar_existe_archivo(test) == 0 ){	//SI EXISTE CARGARLO

			
				files_config_data_LOADER(test , NOMBRE_ARCHIVO );  //CARGA OTRA VER EL ARCHIVO . -------------->>>>>>> CORREGIR  

                log_info(logger_imongo, "SE COMPRUEBA QUE EXISTE ARCHIVO  %s.ims" , NOMBRE_ARCHIVO );

				int RECURSO_CONFIG_SIZE;

		
				RECURSO_CONFIG_SIZE = RECURSO_config->size; 

		
				// EL DE BASURA EXIST			

                if( RECURSO_CONFIG_SIZE < cantidad ){

                log_error(logger_imongo, "SE QUISIERON ELIMINAR DE MAS CARACTERES EN EL ARCHIVO  %s.ims Y LIMPIANDO EL ARCHIVO" , NOMBRE_ARCHIVO );

				return cantidad - RECURSO_CONFIG_SIZE;
              //  truncate(test, 0);
 				
				///instanciar_FILES_RECURSO( test , NOMBRE_ARCHIVO );  //creo e inicio

                }
				
		}else{  //NO EXISTE

                //ACA PONER MANDAR MENSAJE AL DISCODIADOR QUE NO EXISTE EL ARCHIVO
                //------------->>>>>>>>>>>>>>                   <<<<<<<<<<<<<<------------------ NO OLVIDARSE
				//------------->>>>>>>>>>>>>>					<<<<<<<<<<<<<<<<<<<-----------
                
			  
				log_info(logger_imongo, "INTENTANDO CREAR UN NUEVO ARCHIVO DE: %s.ims", NOMBRE_ARCHIVO );

			if ((fd=open( test, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s.ims", NOMBRE_ARCHIVO );
				}


		      instanciar_FILES_RECURSO( test , NOMBRE_ARCHIVO );  //creo e inicio

			  files_config_data_LOADER(test , NOMBRE_ARCHIVO );  //cargo a filesystem
			  

		 	if (  verificar_existe_archivo(test) == 0 )
				{
                        log_info( logger_imongo, "SE CREO CON EXITO EL ARCHIVO: %s.ims", NOMBRE_ARCHIVO);							
				}else{
							log_info( logger_imongo, "ERROR AL CREAR EL ARCHIVO: %s.ims", NOMBRE_ARCHIVO);
				}

				close(fd);

				free(test);

			}		

			return cantidad;	
}





void t_files_config_loader(char* path_config_file_RECURSO, char * nombre) {  //FUNCION INICIALIZADORA


		char * caracter = malloc(sizeof(char));

		caracter = "T";

		t_config * t_config_files = config_create(path_config_file_RECURSO);



		if( strcmp(nombre , "Oxigeno" ) == 0 ){ caracter = "O"; }
		if( strcmp(nombre , "Comida" ) == 0 ){  caracter = "C"; }
		if( strcmp(nombre , "Basura" ) == 0 ){  caracter = "B"; }	


		if( strcmp(caracter , "T" ) == 0 ){ 
	
			config_set_value(t_config_files,  "SIZE", "0");
		
			config_set_value(t_config_files, "BLOCKS", "[]");

		}else{

			config_set_value(t_config_files,  "SIZE", "0");
		
			config_set_value(t_config_files, "BLOCK_COUNT", "0");

			config_set_value(t_config_files, "BLOCKS", "[]");

			config_set_value(t_config_files, "CARACTER_LLENADO", caracter);

			config_set_value(t_config_files, "MD5_ARCHIVO", "" );

			config_save(t_config_files); ////-------

			
			char * md5 =  MD5_CHECKSUM( path_config_file_RECURSO);		
			char * md2 = malloc(sizeof(char) * 32);
			memcpy(md2, md5, 32);   //de alguna manera el string no termina en \0
			 md2[32] = '\0';
			config_set_value(t_config_files, "MD5_ARCHIVO", md2 );

			free(md2);
			free(md5);
		}

			config_save(t_config_files);

			config_destroy(t_config_files);

	  
			//	free(caracter);
}




char * serializar_t_lista( t_list * lista){  //usa string new --> LIBERAR!   retorna un char de la lista ingresada

	int counter=1;
	
	int i;

	char * stringA = string_new();

	char * listaFormateada = string_new();

	t_list_iterator* iterador;

	int tam = list_size(lista);

	iterador = list_iterator_create(lista);


	while(list_iterator_has_next(iterador) ){

		i = (int)list_iterator_next( iterador);

		if( counter != tam  ){

			string_append_with_format(&stringA , "%d, ",  i );
		}else{

			string_append_with_format(&stringA , "%d" ,  i );
		};

		counter++;
	}

	string_append_with_format(&listaFormateada , "[%s]" ,  stringA );

//	list_destroy_and_destroy_elements(lista, eliminar_dato );  // VALGRIND OK
	
	
	free(stringA);

	free(iterador);

	return listaFormateada;
	
}

void eliminar_dato( int dato){

//	free(dato);

};


char * deserializar_DATA_t_lista( t_list * lista, int size){  //RETORNA UN STRING CON TODOS LOS DATOS JUNTOS

	int counter=0;

	char * stringA = malloc(sizeof(char)*size);


	t_list_iterator* iterador;

	int tam = list_size(lista);

	iterador = list_iterator_create(lista);

	int tamanio = Block_size;

	while(list_iterator_has_next(iterador) ){
		
		int  i; char * buffer;

		i =  list_iterator_next( iterador) ;

		if( tam != counter  ){

			buffer = malloc( sizeof(char) * Block_size );

		}else{  
			tamanio = size - ((tam-1)*Block_size); 

			buffer = malloc( sizeof(char) * tamanio );
		}

		memcpy( buffer, bloques_mapper + Block_size * i  , sizeof(char)*tamanio );

		memcpy( stringA + Block_size * counter, buffer   , sizeof(char)*tamanio );
		//string_append(&stringA ,  buffer );

		free(buffer);
		counter++;
	}

	
	free(iterador);

	return stringA;
}


int verificar_existe_archivo(char *path){

		struct stat estado;
		
		int resultado  = stat(path , &estado);  // devuelve 0 si existe el archivo/directorio, y -1 si no existe el archivo/directorio


		return  resultado;
}


void archivos_finally(void){

		free( path_oxigeno );

		free( path_basura );

		free( path_comida );

		files_destroy( OXIGENO_config);

		files_destroy( BASURA_config);

		files_destroy( COMIDA_config);

		list_destroy_and_destroy_elements(lista_Oxigeno, (void*)&file_clave_destroyer);

		list_destroy_and_destroy_elements(lista_Basura, (void*)&file_clave_destroyer);

		list_destroy_and_destroy_elements(lista_Comida, (void*)&file_clave_destroyer);

}


void file_clave_destroyer(void* elemento){
//	char* bb = (char*) elemento;
//	free( bb );
}

void files_destroy(t_files_config* files_config) {
    free(files_config->MD5_ARCHIVO);
   // free(files_config->CARACTER_LLENADO);
   

 // list_destroy_and_destroy_elements(lista_Oxigeno, (void*)&file_clave_destroyer);

    free(files_config);
}
