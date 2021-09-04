#include "fsck.h"


void INICIAR_FSCK(){

        log_info( logger_imongo, "------INICIADO PROCESO DEL FSCK EN EL IMONGO FILE SYSTEM------");

        paths_setter();

 
     //   open_files_blocks_y_superbloque();

         log_info( logger_imongo, "INICIANDO CONTROL DEL LA CANTIDAD DE BLOQUES DEL SUPERBLOQUE....");  
/* 
        if( CONTROL_Cantidad_de_bloques_superbloque() != 0 ){

                    REPARAR_Cantidad_de_bloques();
        }
*/
//        open_files_blocks_y_superbloque();  // relectura de datos supuestamente mejorados
      
       log_info( logger_imongo, "INICIANDO CONTROL DEL BITMAP....");  

        open_files_blocks_y_superbloque();
     //   CONTROL_Bitmap(); //TAMBIEN REPARA  //ok de revisa las batacoras y files  //las bitacoras nunca se sabotean

        //en files
       // open_files_blocks_y_superbloque();        
      //  CONTROL_EN_FILES_SIZE();   //VACIO  ESTOY EN ESO

      //  open_files_blocks_y_superbloque();
     //   CONTROL_EN_FILES_BLOCKS();

     //   CONTROL_EN_FILES_BLOCK_COUNT();
        
      //  CONTROL_EN_FILES_SIZE_RECURSO("Oxigeno"); ///OKKK
    
       if(  CONTROL_EN_FILES_BLOCKS_CAMBIO_LUGAR_RECURSO("Oxigeno") == 1 ){
            return;        
       }

       if(  CONTROL_EN_FILES_BLOCKS_CAMBIO_LUGAR_RECURSO("Comida") == 1 ){
            return;        
       }

       if(  CONTROL_EN_FILES_BLOCKS_CAMBIO_LUGAR_RECURSO("Basura") == 1 ){
            return;        
       }
        // -1 se agrego un valor adicional, 1 la lista ha sido cambiada de lugar, 0 esta bien el resultado


        fsck_finally();
}


void paths_setter(){

		char *NOMBRE_ARCHIVO = "Blocks";

		path_blocks_ims = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_blocks_ims, "/%s.ims", NOMBRE_ARCHIVO);
        

		char *NOMBRE_ARCHIVO2 = "SuperBloque";

		path_superbloque = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_superbloque, "/%s.ims", NOMBRE_ARCHIVO2);


      //  free(NOMBRE_ARCHIVO);

     //   free(NOMBRE_ARCHIVO2);
}

int open_files_blocks_y_superbloque(){

            estado_superbloque_ims = malloc(sizeof(struct stat));

            estado_blocks_ims = malloc(sizeof(struct stat));

 
			if ((fd_blocks_ims=open( path_blocks_ims, O_RDWR  , S_IRUSR|S_IWUSR)) == -1)
				{                        
						log_error(logger_imongo, "No se pudo abrir el archivo: Blocks.ims" );
                        return -1;
				}

	        if ((fd_superbloque_ims=open( path_superbloque, O_RDWR  , S_IRUSR|S_IWUSR)) == -1)
				{                        
						log_error(logger_imongo, "No se pudo abrir el archivo: Superbloque.ims" );
                        return -1;
				}   
/* 
              FILE *fichero1;
              FILE * fichero2;

                fichero2 = fopen( path_blocks_ims, "rw" ); //O_RDWR  , S_IRUSR|S_IWUSR)) == -1)
				

	        fichero1 = fopen( path_superbloque, "rw" ); //O_RDWR  , S_IRUSR|S_IWUSR)) == -1)
			 
          */     

	            read(fd_superbloque_ims, &fsck_Block_size, sizeof(uint32_t) );   //lectura directa fsck_Block_size

	            read(fd_superbloque_ims, &fsck_Blocks, sizeof(uint32_t) );   //lectura directa fsck_Blocks

       

                if( stat(path_superbloque , estado_superbloque_ims) == -1) 
                	{                        
						log_error(logger_imongo, "No se pudo abrir el Estado del archivo: SuperBloque.ims" );
                        return -1;
				}   

                if( stat(path_blocks_ims , estado_blocks_ims) == -1) 
                	{                        
						log_error(logger_imongo, "No se pudo abrir el Estado del archivo: Blocks.ims" );
                        return -1;
				}   
                
                   
                 tamanio_bitmap_leido = estado_superbloque_ims->st_size - (sizeof(uint32_t)) - ( sizeof(uint32_t));

             
                if(tamanio_bitmap_leido > 0 ){
                    

                    	bitmap_leido = malloc ( sizeof(char)*tamanio_bitmap_leido );    //INICIALIZO BITMAP_GLOBAL

	                	read(fd_superbloque_ims, bitmap_leido, sizeof(char) * tamanio_bitmap_leido );                       

                }

  
                return 0;
}


/* 
void SABOTAJES_EN_SUPERBLOQUE(){
        
        if( CONTROL_Cantidad_de_bloques_superbloque != 0){

            REPARACION_SABOTAJES_EN_SUPERBLOQUE();
        }
}
*/
void CONTROL_EN_FILES_BLOCKS(void){
//Si el sabotaje es en la propiedad Blocks de la metadata de los archivos, ahí tienen 3 checkeos:
//1.- Ver si algún bloque se pasa de la cantidad máxima (por ejemplo les ponemos el bloque 1024 y en el FS solo tienen 512 bloques)
//2.- Validar que los bloques estén ocupados en el bitmap (ya que capaz les seteamos un bloque vacío)
//3.- Ver si el orden de los bloques esta bien y para eso tienen que validar con el MD5 para ver que el orden de los bloque este ok (posiblemente solo falle si cambian el último bloque)

      
     CONTROL_FILE_RECURSO("Oxigeno");
            


}


int CONTROL_FILE_RECURSO( char * nombre){  //CARGO EL ARCHIVO AL FILESYSTEM
      
		int fd;   
        int resultado =0;

		char *path_file;

		path_file = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_file, "/Files/%s.ims", nombre);

        

			if ((fd=open( path_file, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)
				{
						log_error(logger_imongo, "No se pudo abrir el archivo: %s", path_file );
						close(fd);
						return NULL;
				}



    	t_config* config = config_create(path_file);

        t_files_config * RECURSO_SABOTAJE_config = malloc(sizeof(t_files_config));

        t_list * lista_sabotaje = files_config_parser(config, RECURSO_SABOTAJE_config );
        

        int resultado1 =  CONTROL_FILE_RECURSO_BLOCKS_MAXIMO(RECURSO_SABOTAJE_config );

        if( resultado1 == 0 ){
		    log_info(logger_imongo, "SE REALIZO LA COMPROBACION CONTROL DE BLOCKS DEL FILE %s, NO SE ENCONTRO ERRORES", path_file );
        }else{
            log_info(logger_imongo, "SE REALIZO LA COMPROBACION CONTROL DE BLOCKS DEL FILE %s, SE ENCONTRARON ERRORES", path_file );

            ///---------->>>>>>>>>> reparacion
        }


        int resultado2 = VALIDAR_BLOQUES_CON_BITMAP(RECURSO_SABOTAJE_config);
        
        if( resultado2 == 0 ){
		    log_info(logger_imongo, "SE REALIZO LA VALIDACION DE BLOQUES CON BITMAP %s, NO SE ENCONTRO ERRORES", path_file );
        }else{
            log_info(logger_imongo, "SE REALIZO LA VALIDACION DE BLOQUES CON BITMAP %s, SE ENCONTRARON ERRORES", path_file );

            ///---------->>>>>>>>>> reparacion
        }

        
        int resultado3 = VALIDAR_RECURSO_CON_MD5_CHECKSUM(RECURSO_SABOTAJE_config, path_file, nombre);

        if( resultado3 == 0 ){
		    log_info(logger_imongo, "SE REALIZO LA VALIDACION DE BLOQUES CON MD5 DEL FILE  %s, NO SE ENCONTRARON ERRORES", path_file );
        }else{
            log_info(logger_imongo, "SE REALIZO LA VALIDACION DE BLOQUES CON MD5 DEL FILE  %s, SE ENCONTRARON ERRORES", path_file );
        }


        if(resultado1 !=0 || resultado2 !=0 || resultado3 !=0 ){

            log_info(logger_imongo, "INICIANDO RESTAURACION DEL FILE  %s .....", path_file );

                RESTAURAR_FILE_RECURSO(path_file );    //ESTA VACIO---->>>>>>>>>>XX
        }





    config_destroy(config);
    files_destroy(RECURSO_SABOTAJE_config );
    free(lista_sabotaje);
    free(path_file);

	close(fd);

    return resultado;
}


int VALIDAR_BLOQUES_CON_BITMAP(t_files_config * RECURSO_SABOTAJE_config){

    int resultado =0;

    int index;

	t_bitarray * bitarray_sabotaje = malloc(sizeof(t_bitarray));
	
	bitarray_sabotaje = bitarray_create_with_mode(bitmap_leido, tamanio_bitmap_leido, MSB_FIRST);
	
	t_list_iterator* iterador;    


	iterador = list_iterator_create(RECURSO_SABOTAJE_config->blocks);


	while(list_iterator_has_next(iterador) ){

		index = (int)list_iterator_next( iterador);

	     if( bitarray_test_bit(bitarray_sabotaje, index) != 1 ){
            resultado = -1;
            break;
         }
	}

	
	bitarray_destroy(bitarray_sabotaje);

	free(iterador);

    return resultado;    
}

//-------------------CORREGIR -----------------------CREAR UN FILE LLENARLO DEL RECURSO Y CODIFICAR A MD5
int VALIDAR_RECURSO_CON_MD5_CHECKSUM(t_files_config * RECURSO_SABOTAJE_config, char * path_file,  char * nombre){

        char * md5check = "md5file.tmp";

        char * md5resultado = "resultmd5.temp";

        char * path_file_md5resultado;


        char *path_file_copia;

		path_file_copia = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_file_copia, "/Files/copia%s.temp", nombre);


        char *path_file_copia_md5check;

		path_file_copia_md5check = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_file_copia_md5check, "/Files/%s", md5check);



		path_file_md5resultado = string_duplicate(punto_de_montaje_GLOBAL);

		string_append_with_format(&path_file_md5resultado, "/Files/%s", md5resultado);



        char * system_command = string_new();

		string_append_with_format(&system_command, "cp %s  %s", path_file , path_file_copia); //BASH DONDE ME RETORNA EL ARCHIVO CON EL MD5
        
        system(system_command);   //trabajamos con la copia



		t_config * t_config_files = config_create(path_file_copia);


        char * md5 = strdup(config_get_string_value(t_config_files, "MD5_ARCHIVO"));


			config_set_value(t_config_files, "MD5_ARCHIVO", "" );

			config_save(t_config_files);



            //   \"   para comilla
        char * system_command2 = string_new();

		string_append_with_format(&system_command2, "md5sum '%s'  | cut -c  -32  >  %s ", path_file_copia,  path_file_copia_md5check); //BASH DONDE ME RETORNA EL ARCHIVO CON EL MD5
        
        system(system_command2);   //trabajamos con la copia



        int fd_result;
        
        	if((fd_result=open( path_file_copia_md5check, O_RDWR  , S_IRUSR|S_IWUSR)) == -1)
				{
						log_error(logger_imongo, "No se pudo abrir el archivo: %s", path_file );						
				}

            char * lectura = malloc(sizeof(char) * 32);

            int resultado = 0;

            	read(fd_result, lectura, sizeof(char) * 32  );

                lectura[32] = '\0';


                if(strcmp(lectura, md5 ) != 0 ){
                    resultado = -1;
                }


                free(lectura);

                free(md5);

                free(system_command);

                free(system_command2);

               // free(system_command3);

                config_destroy(t_config_files);


    return resultado;
}

void RESTAURAR_FILE_RECURSO(char * path){

        t_list * listaNueva = list_create();

        t_config* config = config_create(path);

      
    uint32_t sizef_actual = config_get_int_value(config, "SIZE");

    char * caracter = malloc(sizeof(char));
    caracter  =  config_get_string_value(config, "CARACTER_LLENADO");


    int fd = open( path_blocks_ims, O_RDWR, S_IRUSR|S_IWUSR);

    uint32_t total = fsck_Block_size * fsck_Blocks;

    char * buffer = malloc(total);

    read(fd, buffer, total);

    int i;

    int counter=0;  int contador_de_block=0; int base=0;
    int actual=-1; int nuevo=0;

    for(i=0; i<total;i++){


        if(i>= ((base+1)*fsck_Block_size) ){
           
            base++;           
        }

        if(counter>=sizef_actual){
            break;
        }

        if(buffer[i] == caracter[0] && ( actual != base) ){
            actual = base;
          list_add( listaNueva , base );  

            counter++;
        }
        
    }
    //---------
       printf("-------%s--------", serializar_t_lista(listaNueva) );      // >>>>>>>>>>>>>>>>>>>>>>CORREGIR<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<z



		list_destroy_and_destroy_elements(listaNueva, file_RECURSO_clave_destroyer);


    free(buffer);

    config_destroy(config);
// recorrer el blocks ims y controlar que el caracter sea tal
}

void file_RECURSO_clave_destroyer(void* elemento){
//	char* bb = (char*) elemento;
//	free( bb );
}

void SABOTAJE_EN_FILES(){}

int CONTROL_EN_FILES_BLOCKS_CAMBIO_LUGAR_RECURSO(char * NOMBRE_RECURSO){

    /// EL MD5  ME DICE QUE LA LISTA FUE SABOTEADA PERO NO ME DICE QUE SABOTAJE FUE:  DE ORDEN O DE AGREGADO DE VALOR(O SACADO)

        int resultado =0;

        char *path_file;

        path_file = string_duplicate(punto_de_montaje_GLOBAL);

        string_append_with_format(&path_file, "/Files/%s.ims", NOMBRE_RECURSO);


        t_config* config = config_create(path_file);        


        char** ppLista= config_get_array_value(config, "BLOCKS");

        int index=0;

        t_list * lista  = list_create();

        while(ppLista[index] != NULL){

            char * element =   malloc(sizeof(ppLista[index]));
            
            element = ppLista[index];     

            list_add(lista, atoi(element) );

            index++;
        }

        t_list * lista_ordenada = list_duplicate(lista);

        char * ff = serializar_t_lista(lista);


      //  printf("-----%s-----", ff);   ///---------->>>>>>>>CORREGIR


        list_sort(lista_ordenada,(void*)ordenarDeMenorAMayor);

        char * fff = serializar_t_lista(lista_ordenada);   


     ///   printf("-----%s-----", fff);    ///---------->>>>>>>>CORREGIR


        if(list_size(lista) != list_size(lista_ordenada) ){   //HA SIDO AGREGADO UN VALOR A LA LISTA    
             resultado = -1;  
             //return resultado;     ----->>>>>>>>>  CORREGIR DONDE SE REALIZA LA REPARACION??       

        }else{  // NO HA SIDO AGREGADO  --->>>  COMPROBAR SI SE HA DESORDENADO LA LISTA

            	       
        	t_list_iterator* iterador = list_iterator_create(lista);

            int counter=0;
            while(list_iterator_has_next(iterador) ){

                    int i = (int)list_iterator_next( iterador);

                    if( i != list_get(lista_ordenada, counter ) ){

                            resultado = 1;  //RESULTADO  1 ES LISTA DESORDENADA
                    }

                    counter++;
            }
        }

     //  printf("--RESULTADO:-----%d-------" , resultado);
        
        //LA REPARACION ACA......
	    if(resultado == 1 ){

                log_info(logger_imongo, "LISTA BLOCKS DEL FILE %s DESORDENADA DETECTADA......", NOMBRE_RECURSO );

                log_info(logger_imongo, "INICIANDO REPARACION DEL FILE %s", NOMBRE_RECURSO );

                char * lista_ordenada_serializada = serializar_t_lista(lista_ordenada);

               // config_remove_key
                config_set_value(config, "BLOCKS", lista_ordenada_serializada);

                config_save(config); 

                log_info(logger_imongo, "COMPLETADA REPARACION DEL FILE %s", NOMBRE_RECURSO );

        }else{

                  log_info(logger_imongo, "NO SE DETECTO LISTA DE BLOCKS DEL FILE %s DESORDENADA", NOMBRE_RECURSO );
        }

        free(ff);
        free(fff);

        config_destroy(config);

    return resultado;   // -1 se agrego un valor adicional, 1 la lista ha sido cambiada de lugar, 0 esta bien el resultado
}

int ordenarDeMenorAMayor(int a, int b ){
		return ( a < b );
}


int CONTROL_EN_FILES_SIZE_RECURSO(char * NOMBRE_RECURSO){    
        // cambiar el valor del campo SIZE DEL archivo file
        //recorrer todos los bloques del archivo y obtener y asumir como correcto el tamaño encontrado dentro de los bloques recorriendolos en orden
       // NO CONTROLA SI ES UN SIMPLE CAMBIO DE LUGAR DE LA LISTA    --------->>>>>>>CORREGIR<<<<<<<<<-----------


        int resultado =0;

        char *path_file;

        path_file = string_duplicate(punto_de_montaje_GLOBAL);

        string_append_with_format(&path_file, "/Files/%s.ims", NOMBRE_RECURSO);


        t_list * lista = list_create();

        t_config* config = config_create(path_file);

        
        uint32_t sizef_actual = config_get_int_value(config, "SIZE");

        char * caracter = malloc(sizeof(char));

        caracter  =  config_get_string_value(config, "CARACTER_LLENADO");


        char** ppLista= config_get_array_value(config, "BLOCKS");

        int index=0;

            while(ppLista[index] != NULL){

                        char * element =   malloc(sizeof(ppLista[index]));
                        
                        element = ppLista[index];     

                        list_add(lista, atoi(element) );

                        index++;
                    }
 
 
           //  t_list * lista = list_create();
       

            int fd = open( path_blocks_ims, O_RDWR, S_IRUSR|S_IWUSR);

            uint32_t total = fsck_Block_size * fsck_Blocks;

            char * buffer = malloc(total);

            read(fd, buffer, total);   

            int i=0;

            int counter=0;

            t_list_iterator* iterador;

            int tam = list_size(lista);

            iterador = list_iterator_create(lista);

            int j=0; 

            while(list_iterator_has_next(iterador) ){

                index = (int)list_iterator_next( iterador);

                    for(j=0; j<5;j++){

                        i = index * fsck_Block_size +j ;

                            if(buffer[i] == caracter[0]  ){            
                        
                                counter++;
                            }                                   

                        }
            }


            //   printf("-counter-------%d--------", counter);   //-----------TESTFUNCION  ->>>>>>>>>>>>>>>CORREGIR<<<<<<<<<<<<
        if( sizef_actual != counter  ){

            return 1;
        }


        free(buffer);

        free(iterador);

        config_destroy(config);

    return resultado;
}

void CONTROL_EN_FILES_BLOCK_COUNT(void){
            // contar la cantidad de bloques que hay en blocks [ 0, 1..... n ]
            //actualizar el valor de Blocks_count a base de la lista de Blocks



}

int CONTROL_FILE_RECURSO_BLOCKS_MAXIMO(t_files_config * RECURSO_SABOTAJE_config){

    int MAX_BLOQUE =fsck_Blocks;

    int i;

    int resultado =0;

	t_list_iterator* iterador;

	int tam = list_size(RECURSO_SABOTAJE_config->blocks);

	iterador = list_iterator_create(RECURSO_SABOTAJE_config->blocks);


	while(list_iterator_has_next(iterador) ){

		i = (int)list_iterator_next( iterador);

		if( i >= MAX_BLOQUE  ){    //EN MI SISTEMA EL PRIMER BLOQUE ES EL 0
        resultado = -1;

		}

	}

    free(iterador);
    
    return resultado;
}

void INICIAR_REPARACION_SABOTAJES_EN_SUPERBLOQUE(){}

void INICIAR_REPARACION_BITMAP(){}

void INICIAR_REPARACION_SABOTAJE_EN_FILES(){}



int CONTROL_Cantidad_de_bloques_superbloque(){

        log_info( logger_imongo, "CONTROLANDO QUE LA CANTIDAD DE BLOQUES DEL SUPERBLOQUE.IMS SEA LA CORRECTA");
       
    
       int result = fsck_Block_size * fsck_Blocks;

       if(  result !=  estado_blocks_ims->st_size  ){    
           log_info( logger_imongo, "LA CANTIDAD EL BLOCKS SIZE DEL ARCHIVO Superbloque.ims ES INCORRECTA");       
           return -1;     
        }
    log_info( logger_imongo, "LA CANTIDAD EL BLOCKS SIZE DEL ARCHIVO Superbloque.ims ES CORRECTA");

    return 0;
}

int CONTROL_Bitmap(){

               // int totalEstimado = fsck_Blocks;

             
                file_bitmap_config = malloc(sizeof(t_file_bitmap_config));

                file_bitmap_config->list_posiciones_sabotaje = malloc(sizeof(t_list));

                lista_bitmap = malloc(sizeof(t_list));

                lista_bitmap = list_create();
                
                 file_bitmap_config->list_posiciones_sabotaje = list_create();
             
              // printf( " tamlista--- %d --\n" , list_size( lista_bitmap  )  );


                DIR *dir_files;
                DIR *dir_bitacoras;
             
                struct dirent *ent;
                
                char * path_files = "/home/utnso/Escritorio/sync/Files"; //------->>>>>>> ARREGLAR  <<<<<<<<<<<<<<<<<<

                dir_files = opendir (path_files);  

                if (dir_files == NULL){
                     log_error(logger_imongo,"NO SE PUDO ABRIR EL DIRECTORIO %s", path_files); 
                }
             
                while ((ent = readdir (dir_files)) != NULL)
                    {
                        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
                        {
                            procesoArchivo(ent->d_name, path_files );
                        }
                    }
                closedir (dir_files);

                
                struct dirent *ent2;

                char * path_bitacora = "/home/utnso/Escritorio/sync/Files/Bitacoras";   //------->>>>>>> ARREGLAR  <<<<<<<<<<<<<<
                dir_bitacoras = opendir (path_bitacora);

                if (dir_bitacoras == NULL){
                    log_error(logger_imongo,"NO SE PUDO ABRIR EL DIRECTORIO %s", path_bitacora);
                }
                
                while ((ent2 = readdir (dir_bitacoras)) != NULL)
                    {
                        if ( (strcmp(ent2->d_name, ".")!=0) && (strcmp(ent2->d_name, "..")!=0) )
                        {
                            procesoArchivo(ent2->d_name, path_bitacora);
                        }
                    }

                closedir (dir_bitacoras);



                int tamanioBitarray=fsck_Blocks/8;

                if(fsck_Blocks % 8 != 0){
                    tamanioBitarray++;  //cantidad de bites que tiene el bitmap
                }

              
                bitmap_construido =malloc(tamanioBitarray);

                memset(bitmap_construido, 0, tamanioBitarray );


                t_bitarray * bitarray_auxiliar = malloc(sizeof(t_bitarray));    //INICIALIZO bitarray_global
                

                bitarray_auxiliar = bitarray_create_with_mode(bitmap_construido, tamanioBitarray, MSB_FIRST);

                t_list_iterator* iterador;


                iterador = list_iterator_create(  file_bitmap_config->list_posiciones_sabotaje  );

                int i;


                while(list_iterator_has_next(iterador) ){    //PRIMERO EL BITMAP GLOBAL

                    i = (int)list_iterator_next( iterador);
            
                    bitarray_set_bit(bitarray_auxiliar, i );                      
                }
      
      
                int resultado=0;                

                for(i=0;i<tamanio_bitmap_leido; i++){

                       if(  bitarray_auxiliar->bitarray[i] != bitmap_leido[i]   ) {

                            resultado=1;
                            break;
                       }
                }

                if(resultado==0){
                      log_info( logger_imongo, "CONTROL DEL BITMAP DEL SUPERBLOQUE:  LOS BITS DEL BITMAP DEL SUPERBLOQUE.IMS SON LOS CORRECTOS");
                }else{ 
                    log_info( logger_imongo, "CONTROL DEL BITMAP DEL SUPERBLOQUE:  LOS BITS DEL BITMAP DEL SUPERBLOQUE.IMS --NO-- SON LOS CORRECTOS" );
                    log_info( logger_imongo, "INICIANDO REPARACION DEL BITMAP DEL SUPERBLOQUE........." );

                    
                    lseek(fd_superbloque_ims,sizeof(uint32_t) + sizeof(uint32_t) , SEEK_SET );
                    if( write(fd_superbloque_ims, bitarray_auxiliar->bitarray , tamanio_bitmap_leido ) != -1 ){
                        log_info( logger_imongo, "SE REESCRIBIO Y REPARO EL BITMAP DEL SUPERBLOQUE" );
                    }
                    
                     return -1;
                }




          //  free( bitmap_buffer_auxiliar );

    return 0;
}

void procesoArchivo(char *archivo, char * path)
{

        char * ruta;
	
		ruta = string_duplicate(path);

		string_append_with_format(&ruta, "/%s", archivo);


        char * extension = ".ims";

    	if(string_ends_with( archivo, extension) == 1){


            t_config * config = config_create(ruta);


            char** ppLista= config_get_array_value(config, "BLOCKS");

            int index=0;

            t_list * listado = malloc(sizeof(t_list));
            
            listado = list_create();

            while(ppLista[index] != NULL){

                char * element =   malloc(sizeof(ppLista[index]));
                
                strcpy(element, ppLista[index] );   

                list_add(listado, atoi(element) );
           
                index++;
            }


            if( list_size( listado ) != 0){
    
                list_add_all( file_bitmap_config->list_posiciones_sabotaje , listado );

                 log_info( logger_imongo, "SE AGREGARON LOS DATOS DE LISTAS DEL ARCHIVO ACTUAL LEIDO-- %s --" , archivo );

            }else{

                 log_info( logger_imongo, "EL ARCHIVO LEIDO ACTUAL -- %s -- NO CONTIENE DATOS PARA AGREGAR A LA LISTA(ESTA VACIO DE LISTAS)" , archivo);
            }
        
            liberadorArrays( ppLista);


           config_destroy(config);
    }            
}


void CONTROL_size_bloques(){}

void CONTROL_Block_count(){}

void CONTROL_BLocks(){}



void REPARAR_Cantidad_de_bloques(){
  
    int nuevoTamanioBlocks = estado_blocks_ims->st_size / fsck_Block_size;

    truncate( path_blocks_ims, nuevoTamanioBlocks);  
}

/*
// ----SOLUCION---- CON BASH Y ARCHIVOS md5sum file1 | cut -c -32 > temp.md5
char * MD5_CHECKSUM(char * path_file ){

        char * path_md5_file = string_duplicate(punto_de_montaje_GLOBAL);

        string_append_with_format(&path_md5_file , "/Files/temp.md5" );


        char * system_command = string_new();


		string_append_with_format(&system_command, "md5sum %s | cut -c -32 > %s", path_file , path_md5_file); //BASH DONDE ME RETORNA EL ARCHIVO CON EL MD5
        
        system(system_command);   

        struct stat estado;
    		
		stat(path_md5_file , &estado); 
                    
        char * buffer =  malloc(sizeof(char)* estado.st_size);

        int fd = open( path_md5_file, O_RDWR, S_IRUSR|S_IWUSR);


	    read(fd, buffer, sizeof(char)*32 );

        //------->>>>>>>>   BORRAR EL TEMP FILE <<<<<<<<<<<<-------------
        remove(path_md5_file );

        
        close(fd);

      //  free(estado);
        free(system_command);
        free(path_md5_file);
        

    return buffer;
}*/

char * MD5_CHECKSUM(char * path_file ){  //OBTIENE DEL ARCHIVO YA GUARDADO EL MD5




        t_config* config = config_create(path_file);

        uint32_t size   = config_get_int_value(config, "SIZE");

        char** ppLista= config_get_array_value(config, "BLOCKS");   //REFACTORING DEBE CARGAR INT NO CHARS

        int index=0;

        t_list * listaFile  = list_create();

        int in = ppLista[index];

        while(ppLista[index] != NULL){

                char * nuevo =   malloc(sizeof(ppLista[index]));

                nuevo = ppLista[index];              

                list_add(listaFile, atoi(nuevo)  );

            index++;
        }

        int tam_lista = list_size(listaFile);


        int fd = open( path_blocks_ims, O_RDWR, S_IRUSR|S_IWUSR);

        uint32_t total = fsck_Block_size * fsck_Blocks;

        char * buffer = malloc(total);

        read(fd, buffer, total);

        
        t_list_iterator * iterador = list_iterator_create(listaFile);
        int counter =0;

        int tam_buffer_md = tam_lista* fsck_Block_size;
        char * buffer_md5;


      //  char * md5;
        
        
         int i;

        if(tam_lista != 0){

                buffer_md5 = malloc(tam_buffer_md);

              
                while(list_iterator_has_next(iterador) ){

                    index = (int)list_iterator_next( iterador);
                  
                    i = index * fsck_Block_size ;

                    memcpy(buffer_md5, buffer + i, 5);

                 }

                buffer_md5[tam_buffer_md] = '\0';

        }else{

            buffer_md5 = "";            
        }


            char * path_md5_file = string_duplicate(punto_de_montaje_GLOBAL);

            string_append_with_format(&path_md5_file , "/Files/temp.md5" );


            char * path_md5_file_nuevo = string_duplicate(punto_de_montaje_GLOBAL);

            string_append_with_format(&path_md5_file_nuevo , "/Files/md5.res" );




            int fd_nuevo =open( path_md5_file, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR);

             write(fd_nuevo, buffer_md5, strlen(buffer_md5) );



            char * system_command = string_new();

            string_append_with_format(&system_command, "md5sum %s | cut -c -32 > %s", path_md5_file , path_md5_file_nuevo); //BASH DONDE ME RETORNA EL ARCHIVO CON EL MD5


        system(system_command);   

        struct stat estado;
    		
		stat(path_md5_file_nuevo , &estado); 
                    
        char * md5 =  malloc(sizeof(char)* estado.st_size);

        int fd_md5 = open( path_md5_file_nuevo, O_RDWR, S_IRUSR|S_IWUSR);


	    read(fd_md5, md5, sizeof(char)*32 );

        //------->>>>>>>>   BORRAR EL TEMP FILE <<<<<<<<<<<<-------------
       remove(path_md5_file );
       remove(path_md5_file_nuevo);

        
        close(fd);

        close(fd_md5);

        close(fd_nuevo);



/// "md5sum <<<%s | cut -c -32 >  %s",



   /*      free(buffer_md5 );

        free( md5 );

        free(buffer);
*/
    return md5;
}


void REPARAR_size_bloques(){}

void REPARAR_Block_count(){}


void NOTIFICAR_SABOTAJE(){}


void fsck_finally(){

        free(path_superbloque);

        free(path_blocks_ims); 
        
        free( bitmap_leido );
}
