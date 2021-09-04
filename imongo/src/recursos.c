#include "recursos.h"




void crear_tarea(){   }


void switcher_del_file_system(char *orden){    //-------------------> LA ORDEN DEL DISCODIADOR PUEDE ESTAR EN UN CHAR O EN UN ENUM

int seleccion_tarea;  // 


    switch(seleccion_tarea){
                    case desplazamiento: 
                    formatear_tarea_desplazamiento(orden);
        			break;
                    case ejecucion_tarea: 
                    formatear_tarea_ejecucion_tarea(orden);
        			break;
                    case finalizar_tarea:
        			formatear_tarea_finalizar_tarea(orden);
        			break;
                    case panico_ubicacion_sabotaje:
        			formatear_tarea_panico_ubicacion_sabotaje(orden);
        			break;
                    case resolver_sabotaje:
        			formatear_tarea_resolver_sabotaje(orden);
        			break;            
        		    default:
        			log_warning(logger_imongo, "Operacion desconocida. No quieras meter la pata");
        			break;
        }
    return;
}
  
  

// TEST FUNCTION --------->>>>>>>>>>>>
t_list * string_de_array_a_t_lista(char * string){

        t_list * listaNueva = list_create();
    
        int commas=0;

        char ** ppLista  = NULL;

        ppLista = string_get_string_as_array( string);        

        int i, j;

        int index=0;

        while(ppLista[index] != NULL){

                char * element =   malloc(sizeof(ppLista[index]));
                
                strcpy(element, ppLista[index] );   

                list_add(listaNueva, element );

                index++;
            }

   
    if(list_is_empty(listaNueva) == 1 ){
        printf("lista vacia\n");
    }else{
        printf("lista no vacia\n");
    };

 //------------------->>>>>>>>>>>SACAR TESTFUNCTION

    return listaNueva;
}  

void convertir_buffer_RECURSO_TAREA(char * recurso  );    //

void convertir_buffer_RECURSO_CARACTER (char * recurso); //PROTOTIPO PARAMS:  TRIPULANTE, CARACTER, CANTIDADCARACTER



char * formatear_tarea_desplazamiento(char * data){ char *a="tarea"; return a ; }   //deberia retornar char *    //CODIFICAR A STRING

char * formatear_tarea_ejecucion_tarea(char * data){char *a="tarea"; return a ; }

char* formatear_tarea_finalizar_tarea(char * data){ char *a="tarea";  return a ; }

char * formatear_tarea_panico_ubicacion_sabotaje(char * data){ char *a="tarea";; return a ; }

char * formatear_tarea_resolver_sabotaje(char * data){char *a="tarea"; return a ; }


void almacenar_en_archivo_oxigeno_ims(char * buffer, t_list * listado){

        int fd;

		t_config * t_config_files = config_create(path_oxigeno);


        if ((fd=open( path_oxigeno, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_oxigeno );
				}

        // HAY QUE SUMAR A LA LISTA QUE YA EXISTE OTRA LISTA NUEVA LA DE AGREGADO            

            int largo = strlen(buffer);

            OXIGENO_config->size = OXIGENO_config->size + largo;                


            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", OXIGENO_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );           

////*------------------------------------------------------------

            if(list_size(listado) != 0){    
            list_add_all(OXIGENO_config->blocks , listado);    //----------aSUMA LISTA
            }

            char * bufern2 = malloc(sizeof(uint32_t));
            
            memset(bufern2,0 , sizeof(uint32_t));

            sprintf(bufern2, "%u",  list_size(OXIGENO_config->blocks) );
            
            config_set_value(t_config_files, "BLOCK_COUNT", bufern2);
            

            OXIGENO_config->blocks = lista_Oxigeno;   //*********************************************>>>>>>>>>>>>>>>>>>>*



            char * lista_serializada = serializar_t_lista(OXIGENO_config->blocks);     // -------------

			config_set_value(t_config_files, "BLOCKS", lista_serializada );

			config_set_value(t_config_files, "CARACTER_LLENADO", "O");

			config_set_value(t_config_files, "MD5_ARCHIVO", "" );
			config_save(t_config_files); ////-------

//////-------------------------------------
            char * md5 = MD5_CHECKSUM( path_oxigeno);			
			char * md2 = malloc(sizeof(char) * 32);
			memcpy(md2, md5, 32);
            md2[32] = '\0';
			config_set_value(t_config_files, "MD5_ARCHIVO", md2 );

			free(md2);
			free(md5);
		//	char * md5 = string_duplicate( MD5_CHECKSUM( path_oxigeno) );
		//	config_set_value(t_config_files, "MD5_ARCHIVO",  md5 );	

         //   free(md5);
////*************************************************************************************

			config_save(t_config_files);

			config_destroy(t_config_files);  


            free(bufern2);
    	    free(bufern);


        close(fd);
}


void almacenar_en_archivo_comida_ims(char * buffer, t_list * listado){

    int fd;


		t_config * t_config_files = config_create(path_comida);

        if ((fd=open( path_comida, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_comida );
				}

        // HAY QUE SUMAR A LA LISTA QUE YA EXISTE OTRA LISTA NUEVA LA DE AGREGADO

  
            int largo = strlen(buffer);

            COMIDA_config->size = COMIDA_config->size + largo;                
          //  size_total_Comida =  COMIDA_config->size;

            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", COMIDA_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );           

////*------------------------------------------------------------

            list_add_all(lista_Comida , listado);    //----------aSUMA LISTA

            char * bufern2 = malloc(sizeof(uint32_t));

            memset(bufern2,0 , sizeof(uint32_t));

            sprintf(bufern2, "%u",  list_size(lista_Comida) );
            
            config_set_value(t_config_files, "BLOCK_COUNT", bufern2);



            COMIDA_config->blocks = lista_Comida;   //*********************************************>>>>>>>>>>>>>>>>>>>*



            char * lista_serializada = serializar_t_lista(COMIDA_config->blocks);     // ---------------------------------<<<<<<<<<<<<

			config_set_value(t_config_files, "BLOCKS", lista_serializada );

			config_set_value(t_config_files, "CARACTER_LLENADO", "C");
       
			config_set_value(t_config_files, "MD5_ARCHIVO", "" );
			config_save(t_config_files); ////-------

			//config_set_value(t_config_files, "MD5_ARCHIVO",  COMIDA_config->MD5_ARCHIVO  );	
            char * md5 = MD5_CHECKSUM( path_oxigeno);			
			char * md2 = malloc(sizeof(char) * 32);
			memcpy(md2, md5, 32);
            md2[32] = '\0';
			config_set_value(t_config_files, "MD5_ARCHIVO", md2 );

			free(md2);
			free(md5);

////*************************************************************************************

			config_save(t_config_files);

			config_destroy(t_config_files);  


        free(bufern2);

    	free(bufern);


        close(fd);
}

void almacenar_en_archivo_basura_ims(char * buffer, t_list * listado){

        int fd;

		t_config * t_config_files = config_create(path_basura);


        if ((fd=open( path_basura, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_basura );
				}

        // HAY QUE SUMAR A LA LISTA QUE YA EXISTE OTRA LISTA NUEVA LA DE AGREGADO            

            int largo = strlen(buffer);

            BASURA_config->size = BASURA_config->size + largo;                


            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", BASURA_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );           

////*------------------------------------------------------------

            list_add_all(lista_Basura , listado);    //----------aSUMA LISTA

            char * bufern2 = malloc(sizeof(uint32_t));
            
            memset(bufern2,0 , sizeof(uint32_t));

            sprintf(bufern2, "%u",  list_size(lista_Basura) );
            
            config_set_value(t_config_files, "BLOCK_COUNT", bufern2);


            BASURA_config->blocks = lista_Basura;   //*********************************************>>>>>>>>>>>>>>>>>>>*



            char * lista_serializada = serializar_t_lista(BASURA_config->blocks);     // -------------

			config_set_value(t_config_files, "BLOCKS", lista_serializada );

			config_set_value(t_config_files, "CARACTER_LLENADO", "B");


			config_set_value(t_config_files, "MD5_ARCHIVO", "" );
			config_save(t_config_files); ////-------


			//config_set_value(t_config_files, "MD5_ARCHIVO",  BASURA_config->MD5_ARCHIVO  );	
            char * md5 = MD5_CHECKSUM( path_oxigeno);			
			char * md2 = malloc(sizeof(char) * 32);
			memcpy(md2, md5, 32);
            md2[32] = '\0';
			config_set_value(t_config_files, "MD5_ARCHIVO", md2 );

			free(md2);
			free(md5);

////*************************************************************************************

			config_save(t_config_files);

			config_destroy(t_config_files);  


            free(bufern2);
    	    free(bufern);


        close(fd);
}

void almacenar_en_archivo_tripulanteXX_ims(char * buffer, t_list * listado){

        int fd;

		t_config * t_config_files = config_create(path_tripulante_actual);


        if ((fd=open( path_tripulante_actual, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_tripulante_actual );
				}

        // HAY QUE SUMAR A LA LISTA QUE YA EXISTE OTRA LISTA NUEVA LA DE AGREGADO            

            int largo = strlen(buffer);

            TRIPULANTE_config->size = TRIPULANTE_config->size + largo;                


            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", TRIPULANTE_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );           


            list_add_all(TRIPULANTE_config->blocks , listado);   



            char * lista_serializada = serializar_t_lista(TRIPULANTE_config->blocks);   

			config_set_value(t_config_files, "BLOCKS", lista_serializada );



			config_save(t_config_files);

			config_destroy(t_config_files);  


         //   free(bufern2);
    	    free(bufern);


        close(fd);
}

void actualizar_archivos_RECURSO_ims(t_files_config * RECURSO_config, int cantidad , char *  path_RECURSO){   //esta persiste la lista en los archivos cuando resto
 
    int fd;


		t_config * t_config_files = config_create(path_RECURSO);


        if ((fd=open( path_RECURSO, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_RECURSO );
				}

            RECURSO_config->size = RECURSO_config->size - cantidad;                
           

            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", RECURSO_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );

           

////*------------------------------------------------------------


            memset(bufern,0 , sizeof(uint32_t));

            sprintf(bufern, "%u",  list_size(RECURSO_config->blocks) );
            
            config_set_value(t_config_files, "BLOCK_COUNT", bufern);



            char * lista_serializada = serializar_t_lista( RECURSO_config->blocks );      //serializar para almacenarla  ----<<<<<<<<<<<<<<<<<<<<<<<<

			config_set_value(t_config_files, "BLOCKS", lista_serializada );

            //CASTEO NO LEE CHAR SINO STRINGS
            //char * caracter_llenado_caster = RECURSO_config->CARACTER_LLENADO;
            //char c = RECURSO_config->CARACTER_LLENADO;;
            char caracter_llenado_caster[2];
             caracter_llenado_caster[0] = RECURSO_config->CARACTER_LLENADO;
             caracter_llenado_caster[1] = '\0';
            //char str2[5] = "";
            //strcpy(str2,str1);
			config_set_value(t_config_files, "CARACTER_LLENADO", caracter_llenado_caster );

          //  char * val = strdup(config_get_string_value(t_config_files, "MD5_ARCHIVO")) 


			config_set_value(t_config_files, "MD5_ARCHIVO", "" );
			config_save(t_config_files); ////-------

            
			//config_set_value(t_config_files, "MD5_ARCHIVO",  RECURSO_config->MD5_ARCHIVO  );
            char * md5 = MD5_CHECKSUM( path_oxigeno);			
			char * md2 = malloc(sizeof(char) * 32);
			memcpy(md2, md5, 32);
            md2[32] = '\0';
			config_set_value(t_config_files, "MD5_ARCHIVO", md2 );

			free(md2);
			free(md5);

////*************************************************************************************

			config_save(t_config_files);

			config_destroy(t_config_files);  

    	    free(bufern);


        close(fd);



}

/*

void actualizar_REDUCCION_archivos_RECURSOS_ims(t_files_config * RECURSO_config, int cantidad , char * path_RECURSO ){   //esta persiste la lista en los archivos cuando resto
 
    int fd;


		t_config * t_config_files = config_create(path_RECURSO);


        if ((fd=open( path_RECURSO, O_RDWR , S_IRUSR|S_IWUSR)) == -1)
				{
						log_info(logger_imongo, "No se pudo abrir el archivo: %s", path_RECURSO );
				}

            RECURSO_config->size = RECURSO_config->size - cantidad;                
         //   size_total_Oxigeno =    OXIGENO_config->size;

            char * bufern = malloc(sizeof(uint32_t));

            sprintf(bufern, "%u", RECURSO_config->size );

	        config_set_value(t_config_files,  "SIZE", bufern );

           

//// *------------------------------------------------------------

         //   lista_RECURSO = lista;

            memset(bufern,0 , sizeof(uint32_t));

            sprintf(bufern, "%u",  list_size(RECURSO_config->blocks) );
            
            config_set_value(t_config_files, "BLOCK_COUNT", bufern);


           // RECURSO_config->blocks = lista_RECURSO;   // *********************************************>>>>>>>>>>>>>>>>>>>*



            char * lista_serializada = serializar_t_lista( RECURSO_config->blocks );      //serializar para almacenarla  ----<<<<<<<<<<<<<<<<<<<<<<<<

			config_set_value(t_config_files, "BLOCKS", lista_serializada );

			config_set_value(t_config_files, "CARACTER_LLENADO", "O");

          //  char * val = strdup(config_get_string_value(t_config_files, "MD5_ARCHIVO")) 

			config_set_value(t_config_files, "MD5_ARCHIVO",  RECURSO_config->MD5_ARCHIVO  );
		

//// *************************************************************************************

			config_save(t_config_files);

			config_destroy(t_config_files);  

    	    free(bufern);


        close(fd);
}
*/

void GENERAR_OXIGENO( int cantidad ){   //agrega cantidad al archivo


        if(cantidad <= 0){

            log_error(logger_imongo, "OCURRIO UN ERROR INGRESE UNA CANTIDAD MAYOR DE 0");
        }
   
		char * oxigeno = "Oxigeno";	

        path_oxigeno =  crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , oxigeno );
        
        char* bufferProvisorio = (char *)malloc(sizeof(char) * cantidad);

        bufferProvisorio = string_repeat('O', cantidad);
       
        t_list * listado  = malloc(sizeof(t_list));  

        int * correcto = malloc(sizeof(int));

        *correcto = 0;

        listado = almacenar_en_memoria_del_file_systemAUXILIAR(OXIGENO_config , bufferProvisorio, &correcto );  //escrib elos bytes en blocks.ims
       
            if( *correcto !=  0 ){
                
                return;
            }

        almacenar_en_archivo_oxigeno_ims( bufferProvisorio, listado );  //almacena datos nomas

        list_destroy_and_destroy_elements(listado, (void*)&lista_destroyer);

        log_info(logger_imongo,"SE GENERARON %d UNIDADES DE OXIGENO EN %s", cantidad, path_oxigeno );

        free(bufferProvisorio);

     //   free(oxigeno);
            
 }

void GENERAR_STRING( int tripulante , char * buffer){   /// PROTOTIPO TRIPULANTES


        int cantidad = strlen(buffer);

        if(cantidad <= 0){

            log_error(logger_imongo, "OCURRIO UN ERROR INGRESE UNA CANTIDAD MAYOR DE 0");
        }
   
        crear_Tripulante_en_File_System( tripulante , TRIPULANTE_config );

        char* bufferProvisorio; // = "ABCDEFGHIJKLMNO"; //(char *)malloc(sizeof(char) * cantidad);

        bufferProvisorio = string_duplicate(buffer);        

        t_list * listado  = malloc(sizeof(t_list));  

        int * correcto = malloc(sizeof(int));
       
        *correcto = 0;       

         listado = almacenar_en_memoria_del_file_system(TRIPULANTE_config , bufferProvisorio, &correcto );  //escribe elos bytes en blocks.ims
    
        if( *correcto !=  0 ){
            
             return;
         }

        almacenar_en_archivo_tripulanteXX_ims( bufferProvisorio, listado );  //almacena datos nomas

        list_destroy_and_destroy_elements(listado, (void*)&lista_destroyer);

        log_info(logger_imongo,"SE ALMACENO LA TAREA DEL TRIPULANTE %d : %s ", tripulante , bufferProvisorio);

        free(bufferProvisorio);            
 }


void CONSUMIR_OXIGENO(int cantidad){   //cantidad a borrar

//  1 .Verificar que exista un archivo llamado Oxigeno.ims en el i-Mongo-Store
//  2. Si no existe informar que no existe el archivo y finalizar la tarea. Deberán esperar el tiempo en la cola de bloqueado del discordiador de todas formas.
//  3. Eliminar tantos caracteres de llenado del archivo como indique el parámetro CANTIDAD.  En caso de que intenten eliminarse de más, dejar el
//       archivo completamente vacío e informar por log que se quisieron eliminar más caracteres de los existentes.

        char * nombre_archivo = "Oxigeno";

        int emergencia=0;

      int resultado = CONSUMIR_RECURSO_instanciador_de_archivo(OXIGENO_config,  cantidad  , nombre_archivo ); 
        
          if(  resultado != cantidad ){

           emergencia = 1;

           cantidad=resultado;
       }

    
        if(OXIGENO_config->size >= cantidad){                    

            int resto =  OXIGENO_config->size -  Block_size * ( OXIGENO_config->block_count - 1 ); //saber cuantos tiene el ultimo item de la lista

            int la_diferencial =  OXIGENO_config->size  - cantidad;  //lo que deberia tener despues de restar
            
            int tamanio = la_diferencial;

            int cantidad_bloques_necesarios=( tamanio ) /Block_size;    //REFACTORIZAR PODRIA SER "BORRAR HACIA ATRAS BUSCANDO EL CHAR"
    
                if( (tamanio ) % Block_size != 0){

                    cantidad_bloques_necesarios++;  

                }//else{ cantidad_bloques_necesarios = 0; }
    

                int borrar_bloques = OXIGENO_config->block_count  - cantidad_bloques_necesarios; //cantidad de bloques a borrar  OK

                int ultimo_bloque;  // EL BLOQUE QUE ES EL NUEVO ULTIMO DE LA LISTA (CONTENIDO)

        
            if(borrar_bloques == 0 ){  //no hay que borrar bloques sino elementos

                    ultimo_bloque = list_get(OXIGENO_config->blocks, list_size(OXIGENO_config->blocks) -1 );                    
                
                    int ultimo_cantidad = la_diferencial - ( cantidad_bloques_necesarios-1 ) * Block_size;


                    if(ultimo_cantidad != Block_size ){ //MODIFICAR EL ULTIMO , NO HACE FALTA BORRAR bloques de la lista si coincide con el Block_size

                        char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                        char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);



                        bufferProvisorio = string_repeat( 0 , Block_size);

                        bufferProvisorio2 = string_repeat('O', ultimo_cantidad );

                        memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                        memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );
                    }                                          
                
            }
            else{   // bloques y elementos

                    int diff = cantidad_bloques_necesarios -1;

                    if(diff >= 0){

                             ultimo_bloque = list_get(OXIGENO_config->blocks, cantidad_bloques_necesarios -1 );                 
                
                ///-----------------------------------------------------------------------------------------
                        //    int ultimo_cantidad = la_diferencial - ( cantidad_bloques_necesarios-1 ) * Block_size;
                            int diff2 = ( diff ) * Block_size;
                            int ultimo_cantidad = la_diferencial - diff2;


                            if(ultimo_cantidad != Block_size ){ //MODIFICAR EL ULTIMO , NO HACE FALTA BORRAR bloques de la lista si coincide con el Block_size

                                    char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                                    char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);

                                    bufferProvisorio = string_repeat( 0 , Block_size);

                                    bufferProvisorio2 = string_repeat('O', ultimo_cantidad );

                                    memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                                    memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );

                                }
                    }                                                                  
                        //--------------------------------------------------------------------------

                    //REMOVER LOS BLOQUESS
                    int i;

                    int contador = OXIGENO_config->block_count;

                    t_list * listaBloquesABorrar = malloc(sizeof(t_list));

                    listaBloquesABorrar = list_create();


                            for(i=0;i<borrar_bloques;i++){
                        
                                list_add(listaBloquesABorrar, (int)list_get(OXIGENO_config->blocks, contador-1) );
                            
                                list_remove(OXIGENO_config->blocks, contador  - 1 );

                                contador--;

                            }                                    

                    liberar_memoria_del_file_system(listaBloquesABorrar);

        //            OXIGENO_config->blocks = listado;    

                //  list_destroy_and_destroy_elements(lista, (void*)&lista_destroyer);  ------->>>>>>>DA ERROR                                         

                }          
                actualizar_archivos_RECURSO_ims( OXIGENO_config, cantidad, path_oxigeno);


        }else{   //CONTROLAR SI ES NECESARIO
                //BORRAR EL FILE
                //  CONSUMIR_OXIGENO_instanciador_de_archivo(  cantidad  );   
        }

              if(emergencia != 0){  //EXCESO DE CARACTERES ELIMINADOS

               char * test = string_duplicate(punto_de_montaje_GLOBAL);

                string_append_with_format(&test, "/Files/%s.ims", nombre_archivo);   ///-******

                truncate(test, 0);
 				
				instanciar_FILES_RECURSO( test , nombre_archivo );  //creo e inicio

                
        }else{
                log_info(logger_imongo, "SE CONSUMIERON  %d UNIDADES DE OXIGENO DEL FILE SYSTEM." , cantidad);            
        }

}

void GENERAR_COMIDA( int cantidad ){   //agrega cantidad al archivo


        if(cantidad <= 0){

            log_error(logger_imongo, "OCURRIO UN ERROR INGRESE UNA CANTIDAD MAYOR DE 0");
        }
   
		char * comida = "Comida";	


       path_comida =  crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , comida );


        char* bufferProvisorio = (char *)malloc(sizeof(char) * cantidad);

        bufferProvisorio = string_repeat('C', cantidad);

        t_list * listado  = malloc(sizeof(t_list));  

       // char caracter = 'C';

     
        int * correcto = malloc(sizeof(int));
        *correcto = 0;
        listado = almacenar_en_memoria_del_file_systemAUXILIAR( COMIDA_config, bufferProvisorio ,&correcto  );  
        if( *correcto !=  0 ){
            
             return;
         }

        almacenar_en_archivo_comida_ims( bufferProvisorio, listado );


        list_destroy_and_destroy_elements(listado, (void*)&lista_destroyer);

        log_info(logger_imongo, "SE GENERARON %d UNIDADES DE COMIDA EN %s", cantidad, path_comida );
            
        free(bufferProvisorio);
}

void CONSUMIR_COMIDA(int cantidad){

        char * nombre_archivo = "Comida";

        int emergencia=0;

        int resultado = CONSUMIR_RECURSO_instanciador_de_archivo(COMIDA_config ,  cantidad , nombre_archivo  );   

          if(  resultado != cantidad ){

           emergencia = 1;

           cantidad=resultado;
       }
    
        if(COMIDA_config->size >= cantidad){                    

            int resto =  COMIDA_config->size -  Block_size * ( COMIDA_config->block_count - 1 ); //saber cuantos tiene el ultimo item de la lista

            int la_diferencial =  COMIDA_config->size  - cantidad;  //lo que deberia tener despues de restar
            
            int tamanio = la_diferencial;

            int cantidad_bloques_necesarios=( tamanio ) /Block_size;    //REFACTORIZAR PODRIA SER "BORRAR HACIA ATRAS BUSCANDO EL CHAR"
    
                if( (tamanio ) % Block_size != 0){

                    cantidad_bloques_necesarios++;  

                }//else{ cantidad_bloques_necesarios = 0; }
    

                int borrar_bloques = COMIDA_config->block_count  - cantidad_bloques_necesarios; //cantidad de bloques a borrar  OK

                int ultimo_bloque;  // EL BLOQUE QUE ES EL NUEVO ULTIMO DE LA LISTA (CONTENIDO)

        
            if(borrar_bloques == 0 ){  //no hay que borrar bloques sino elementos

                    ultimo_bloque = list_get(COMIDA_config->blocks, list_size(COMIDA_config->blocks) -1 );
                    
                
                    int ultimo_cantidad = la_diferencial - ( cantidad_bloques_necesarios-1 ) * Block_size;


                    if(ultimo_cantidad != Block_size ){ //MODIFICAR EL ULTIMO , NO HACE FALTA BORRAR bloques de la lista si coincide con el Block_size

                        char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                        char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);



                        bufferProvisorio = string_repeat( 0 , Block_size);

                        bufferProvisorio2 = string_repeat('C', ultimo_cantidad );

                        memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                        memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );
                    }                                          
                
            }
            else{   // bloques y elementos

                    int diff = cantidad_bloques_necesarios -1;

                    if(diff >= 0){

                             ultimo_bloque = list_get(COMIDA_config->blocks, cantidad_bloques_necesarios -1 );                 
                
                ///-----------------------------------------------------------------------------------------
                        //    int ultimo_cantidad = la_diferencial - ( cantidad_bloques_necesarios-1 ) * Block_size;
                                int diff2 = ( diff ) * Block_size;
                              int ultimo_cantidad = la_diferencial - diff2;


                            if(ultimo_cantidad != Block_size ){ //MODIFICAR EL ULTIMO , NO HACE FALTA BORRAR bloques de la lista si coincide con el Block_size

                                    char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                                    char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);



                                    bufferProvisorio = string_repeat( 0 , Block_size);

                                    bufferProvisorio2 = string_repeat('C', ultimo_cantidad );

                                    memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                                    memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );

                                }
                    }                                                                  
                        //--------------------------------------------------------------------------

                    //REMOVER LOS BLOQUESS
                    int i;

                    int contador = COMIDA_config->block_count;

                    t_list * listaBloquesABorrar = malloc(sizeof(t_list));

                    listaBloquesABorrar = list_create();


                            for(i=0;i<borrar_bloques;i++){
                        
                                list_add(listaBloquesABorrar, (int)list_get(COMIDA_config->blocks, contador-1) );
                            
                                list_remove(COMIDA_config->blocks, contador  - 1 );

                                contador--;

                            }                                    

                    liberar_memoria_del_file_system(listaBloquesABorrar);

               //     COMIDA_config->blocks = listado;    

                //  list_destroy_and_destroy_elements(lista, (void*)&lista_destroyer);  ------->>>>>>>DA ERROR                                         

                }          
                actualizar_archivos_RECURSO_ims( COMIDA_config, cantidad , path_comida);


        }else{   //CONTROLAR SI ES NECESARIO
                //BORRAR EL FILE
                //  CONSUMIR_OXIGENO_instanciador_de_archivo(  cantidad  );   
        }

          if(emergencia != 0){  //EXCESO DE CARACTERES ELIMINADOS

               char * test = string_duplicate(punto_de_montaje_GLOBAL);

                string_append_with_format(&test, "/Files/%s.ims", nombre_archivo);   ///-******

                truncate(test, 0);
 				
				instanciar_FILES_RECURSO( test , nombre_archivo );  //creo e inicio

                
        }else{
                log_info(logger_imongo, "SE CONSUMIERON  %d UNIDADES DE COMIDA DEL FILE SYSTEM." , cantidad);            
        }


}



void GENERAR_BASURA( int cantidad ){   //agrega cantidad al archivo


        if(cantidad <= 0){

            log_error(logger_imongo, "OCURRIO UN ERROR INGRESE UNA CANTIDAD MAYOR DE 0");
       }
   
		char * basura = "Basura";	

        path_basura =  crear_Archivo_Files_ARCHIVO(punto_de_montaje_GLOBAL , basura );
        
        char* bufferProvisorio = (char *)malloc(sizeof(char) * cantidad);

        bufferProvisorio = string_repeat('B', cantidad);

        t_list * listado  = malloc(sizeof(t_list));  

        int * correcto = malloc(sizeof(int));

        *correcto = 0;

        listado = almacenar_en_memoria_del_file_systemAUXILIAR( BASURA_config, bufferProvisorio, &correcto  );  //escribe en los bytes en blocks.ims
       
            if( *correcto !=  0 ){
                
                return;
            }

        almacenar_en_archivo_basura_ims( bufferProvisorio, listado );  //almacena datos nomas

        log_info(logger_imongo, "SE GENERO BASURA EN %s", path_basura );

        list_destroy_and_destroy_elements(listado, (void*)&lista_destroyer);

        free(bufferProvisorio);            
 }


void DESCARTAR_BASURA(void){
           // 1. Verificar que exista un archivo llamado Basura.ims en el i-Mongo-Store
           // 2. Si el archivo existe, Eliminarlo
           // 3. Si el archivo NO existe, notificar por el log (ANTES DE ELIMINAR VACIAR LAS B DE IMONGO)

     
   
		char * basura = "Basura";	
        
		char *ruta;

		ruta = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&ruta, "/%s.ims", basura);


		if(  verificar_existe_archivo(ruta) == 0 ){//SI EXISTE


                log_info(logger_imongo, "SE COMPRUEBA QUE EXISTE ARCHIVO  %s.ims" , basura );

               if(  remove(ruta) == 0 ){

                     CONSUMIR_BASURA(BASURA_config->size);   //vaciar el archivo de basura

                    log_info(logger_imongo, "EL ARCHIVO  %s.ims HA SIDO ELIMINADO DEL I-MONGO FILE SYSTEM" , basura );

               }

				
		}else{  //no existe 

           log_info(logger_imongo, "SE COMPRUEBA QUE EXISTE ARCHIVO  %s.ims" , basura );

        }
}



///***********************************************

void CONSUMIR_BASURA(int cantidad){

        char * nombre_archivo = "Basura";

        CONSUMIR_RECURSO_instanciador_de_archivo(BASURA_config ,  cantidad , nombre_archivo  );   

    
        if(BASURA_config->size >= cantidad){                    

            int resto =  BASURA_config->size -  Block_size * ( BASURA_config->block_count - 1 ); //saber cuantos tiene el ultimo item de la lista


      //      t_list *  listado  = malloc(sizeof(t_list));

       //     listado = BASURA_config->blocks; 

            int la_diferencial =  BASURA_config->size  - cantidad; 
            
            int tamanio = la_diferencial;

            int cantidad_bloques_necesarios=( tamanio ) /Block_size;    
    
                if( (tamanio ) % Block_size != 0){

                    cantidad_bloques_necesarios++;  

                }
    

                int borrar_bloques = BASURA_config->block_count  - cantidad_bloques_necesarios; //cantidad de bloques a borrar  OK

                int ultimo_bloque;  // EL BLOQUE QUE ES EL NUEVO ULTIMO DE LA LISTA (CONTENIDO)

        
            if(borrar_bloques == 0 ){  //no hay que borrar bloques sino elementos

                    ultimo_bloque = list_get(BASURA_config->blocks, list_size(BASURA_config->blocks) -1 );
                    
                
                    int ultimo_cantidad = la_diferencial - ( cantidad_bloques_necesarios-1 ) * Block_size;


                    if(ultimo_cantidad != Block_size ){ //MODIFICAR EL ULTIMO , NO HACE FALTA BORRAR bloques de la lista si coincide con el Block_size

                        char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                        char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);



                        bufferProvisorio = string_repeat( 0 , Block_size);

                        bufferProvisorio2 = string_repeat('B', ultimo_cantidad );

                        memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                        memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );
                    }                                          
                
            }
            else{   // bloques y elementos

                    int diff = cantidad_bloques_necesarios -1;

                    if(diff >= 0){

                             ultimo_bloque = list_get(BASURA_config->blocks, cantidad_bloques_necesarios -1 );                 
                
              
                            int diff2 = ( diff ) * Block_size;
                            int ultimo_cantidad = la_diferencial - diff2;


                            if(ultimo_cantidad != Block_size ){ 

                                    char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

                                    char* bufferProvisorio2 = (char *)malloc(sizeof(char) * ultimo_cantidad);



                                    bufferProvisorio = string_repeat( 0 , Block_size);

                                    bufferProvisorio2 = string_repeat('B', ultimo_cantidad );

                                    memcpy( bufferProvisorio , bufferProvisorio2 , ultimo_cantidad  );

                                    memcpy(bloques_mapper + Block_size * ultimo_bloque  , bufferProvisorio, Block_size );

                                }
                    }                                                                 
                       
                    int i;

                    int contador = BASURA_config->block_count;

                    t_list * listaBloquesABorrar = malloc(sizeof(t_list));

                    listaBloquesABorrar = list_create();


                            for(i=0;i<borrar_bloques;i++){
                        
                                list_add(listaBloquesABorrar, (int)list_get(BASURA_config->blocks, contador-1) );
                            
                                list_remove(BASURA_config->blocks, contador  - 1 );

                                contador--;

                            }                                    

                    liberar_memoria_del_file_system(listaBloquesABorrar);

                 //   BASURA_config->blocks = listado;                                           

                }          
                actualizar_archivos_RECURSO_ims( BASURA_config, cantidad , path_basura);
        }
}


//**************************************************************




void deserializar(){}

void recursos_finally(){

    	//lista
  //  list_destroy_and_destroy_elements(lista, eliminar_dato );  // VALGRIND OK


}

void lista_destroyer(void* elemento){

// NO CONTIENE NADA PORQEU ES UN INT

}




