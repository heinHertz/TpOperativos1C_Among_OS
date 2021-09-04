#include "filesystem.h"



void init_file_system(char * punto_de_montaje){   //el path punto de montaje

	log_info(logger_imongo, "INICIALIZANDO FILE SYSTEM...");

//	punto_de_montaje_GLOBAL = string_duplicate(punto_de_montaje);

	instanciar_directorios_punto_de_montaje(punto_de_montaje);
	
	instanciar_archivos_punto_de_montaje(punto_de_montaje);

	instanciar_archivos_recursos();
}


void sincronizar_file_system(void){ 

//----------------SINCRO DEL FILE BLOCKS.IMS --------------------------------------------
	int fd_bloks = open(path_del_block_ims, O_RDWR,  S_IRUSR | S_IWUSR );
		if(fd_bloks==-1){
			printf("error al abrir %s ", path_del_block_ims);
			exit(-1);
		}

		bloques_mapper = (char *) mmap(0, blocks_cantidad * Block_size,  PROT_READ | PROT_WRITE, MAP_SHARED, fd_bloks ,0);
		if(bloques_mapper==MAP_FAILED){
			log_error(logger_imongo, "ERROR AL MAPEAR EL BINARIO BLOCKS"  );
			exit(-1);
		}
	
	//---------------INICIALIZAR EL MAPPER SI ES NUEVO OR CARGADO -----------------

		memcpy(bloques_mapper, bloques, blocks_cantidad * Block_size );
	//-------------------------------------------------------

		msync(bloques_mapper, blocks_cantidad * Block_size, MS_SYNC);

	close( fd_bloks  );

//--------------SINCRO DEL FILE SUPERBLOQUE.IMS ------------------------------------------------ 

	int fd_super = open(path_del_superbloque, O_RDWR,  S_IRUSR | S_IWUSR );

		if(fd_super==-1){
			printf("ERROR AL SINCRONIZAR EL SUPERBLOQUE %s" ,path_del_superbloque);
			exit(-1);
		}


		superbloque_bits_mapper = (char *) mmap(0, sizeof(uint32_t) + sizeof(uint32_t) +  Block_size,  PROT_READ | PROT_WRITE, MAP_SHARED, fd_super, 0);

		if(superbloque_bits_mapper==MAP_FAILED){
			log_info(logger_imongo, "ERROR AL MAPEAR EL BINARIO SUPERBLOQUE"  );
			exit(-1);
		}

	//---------------INICIALIZAR EL MAPPER SI ES NUEVO OR CARGADO -----------------
		memcpy(superbloque_bits_mapper, &(Block_size), sizeof(uint32_t ) ); 
		memcpy(superbloque_bits_mapper + sizeof(uint32_t),	&(blocks_cantidad), sizeof(uint32_t) );
		memcpy(superbloque_bits_mapper +  sizeof(uint32_t) +  sizeof(uint32_t) , bloques, tamanio_Superbloque_GLOBAL );
	//---------------------------------------------------------------------------

		msync(superbloque_bits_mapper,  sizeof(uint32_t) + sizeof(uint32_t) +  Block_size , MS_SYNC);

	close( fd_super );
}


void mappear_file_system( void ){	//

    msync(superbloque_bits_mapper, sizeof(uint32_t) + sizeof(uint32_t) +  tamanio_bytes_bitmap_global , MS_SYNC); ///  MAPPEO

    msync(bloques_mapper, blocks_cantidad * Block_size, MS_SYNC); ///  MAPPEO

}


char * recuperar_recursos_del_file_system(t_list * lista){   //le das una lista y te retorna un char con los datos


		int i;

		t_list_iterator* iterador;



		iterador = list_iterator_create(lista);

		char *test;

		char *buffer = malloc(Block_size) ;


		while(list_iterator_has_next(iterador) ){   

			i = (int)list_iterator_next( iterador);
	
			memcpy(buffer, bloques_mapper + i * Block_size , Block_size);


			string_append_with_format(&test, "%s", buffer) ;

		 }


	return test;
}


void agregar_tarea_al_file_system(void){} 

void agregar_caracter_al_file_system(char *letra){}

void recuperar_file_system(void) {}


void instanciar_archivos_recursos(void){

	crear_archivos_recursos_files();
}



int instanciar_directorios_punto_de_montaje(char * punto_de_montaje){ // devuelve 0 si existe el archivo/directorio, y -1 si no existe el archivo/directorio

	if( verificar_existencia_directorio_PUNTO_DE_MONTAJE(punto_de_montaje) != 0 ){  //si es no ,no existe directorio punto de montaje
		
			if( crear_directorio( punto_de_montaje)  == 0 ){
					log_info(logger_imongo, "SE CREO EL DIRECTORIO PUNTO DE MONTAJE:  %s", punto_de_montaje );
				}else{
					log_info(logger_imongo, "NO SE PUDO CREAR EL DIRECTORIO PUNTO DE MONTAJE : %s", punto_de_montaje );
					return(-1);
				}

	}
		
	if( verificar_existencia_directorio_FILES(punto_de_montaje)  != 0 ){   //puede que exista el punto de montaje pero NO el files

				if( crear_directorio_FILES( punto_de_montaje)  == 0 ){
					log_info(logger_imongo, "SE CREO EL DIRECTORIO FILES DE LA CARPETA %s", punto_de_montaje );
				}else{
					log_info(logger_imongo, "NO SE PUDO CREAR EL DIRECTORIO FILES DE LA CARPETA %s", punto_de_montaje );
					return(-1);
				}

	}
	
	if( verificar_existencia_directorio_BITACORAS(punto_de_montaje)  != 0 ){   //puede que exista el punto de montaje pero NO el bitacoras

				if( crear_directorio_BITACORAS( punto_de_montaje)  == 0 ){
					log_info(logger_imongo, "SE CREO EL DIRECTORIO BITACORAS DE LA CARPETA %s", punto_de_montaje );
				}else{
					log_info(logger_imongo, "NO SE PUDO CREAR EL DIRECTORIO BITACORAS DE LA CARPETA %s", punto_de_montaje );
					return(-1);
				}

	}

	return 0;
}


void instanciar_archivos_punto_de_montaje(char * punto_de_montaje){

	 abrirSuperbloque(punto_de_montaje);  

	inicializarBlocks(punto_de_montaje , superbloque_global);
}


void crear_directorio_punto_de_montaje(char *path){

	if(verificar_existe_directorio(path) != 0 ){

		crear_directorio(path);
	}		
}


int crear_directorio(char *path){
		int res = mkdir(path, (mode_t) 0777);
			if(res == -1){
				log_error(logger_imongo,"No se pudo crear la carpeta %s",path);
				exit(-1);
			}
		return 0;
}


int verificar_existe_directorio(char *path){

		struct stat estado;

		int resultado  = stat(path , &estado);  // devuelve 0 si existe el archivo/directorio, y -1 si no existe el archivo/directorio


		return  resultado;
}


int verificar_existencia_directorio_BITACORAS(char *path_punto_montaje){
		
 	char *pathBuffer = string_duplicate(path_punto_montaje);

	char *agregado = "/Files/Bitacoras";

	string_append_with_format(&pathBuffer, "%s", agregado);


	int resultado = verificar_existe_directorio(pathBuffer);

	free(pathBuffer);

	return resultado;	
}


int verificar_existencia_directorio_FILES(char *path_punto_montaje){
		
 	char *pathBuffer = string_duplicate(path_punto_montaje);

	char *agregado = "/Files";

	string_append_with_format(&pathBuffer, "%s", agregado);

	int resultado = verificar_existe_directorio(pathBuffer);

	free(pathBuffer);

	return resultado;
}


int verificar_existencia_directorio_PUNTO_DE_MONTAJE(char *path_punto_montaje){
		
		int resultado =  verificar_existe_directorio(path_punto_montaje);
	
	return resultado;
}


void comprobar_estado_archivo(char * path){
	
	struct stat *estado = malloc(sizeof(struct stat));

	int fd = open(path, O_RDWR, (mode_t) 0777);
	
	int resultado = fstat(fd, estado);	

	if(resultado ==-1){
		log_error(logger_imongo, "FALLA AL LEER ARCHIVO CON fstat");
	}
	else{
		log_info(logger_imongo,"CANTIDAD DE BLOQUES: %d, TAMANIO BLOQUES: %d, ",estado->st_blocks,estado->st_blksize);
	}
		
	close(fd);	
}


int crear_directorio_FILES(char *path_punto_montaje){
		
 	char *pathBuffer = string_duplicate(path_punto_montaje);

	char *agregado = "/Files";

	string_append_with_format(&pathBuffer, "%s", agregado);

	crear_directorio(pathBuffer);

	int resultado = verificar_existe_directorio(pathBuffer);

	free(pathBuffer);

	return resultado;
}



int crear_directorio_BITACORAS(char *path_punto_montaje){

	char *pathBuffer = string_duplicate(path_punto_montaje);

	char *agregado = "/Files/Bitacoras";

	string_append_with_format(&pathBuffer, "%s", agregado);

	crear_directorio(pathBuffer);

	int resultado = verificar_existe_directorio(pathBuffer);

	free(pathBuffer);

	return resultado;
}


void  crearBlocks(t_superbloque * super){

	uint32_t bytes_por_bloque,  cantBloques;

	bytes_por_bloque = super->block_size;

	cantBloques = super->blocks;

	bloques = malloc(bytes_por_bloque * cantBloques);

	memset(bloques, 0, bytes_por_bloque * cantBloques);

	log_info(logger_imongo, "Se Inicializo un nuevo Blocks.ims " );	
}


void inicializarBlocks(char *path ,  t_superbloque* super){

		int fd;

		char *test;

		test = string_duplicate(path);

		char *agregado =  "Blocks.ims";
		
		string_append_with_format(&test, "/%s", agregado);

	
		path_del_block_ims = string_duplicate(test);
 

		if(  verificar_existe_archivo(test) == 0 ){ // existe 

			if ((fd=open( test, O_RDWR  , S_IRUSR|S_IWUSR)) == -1)   //lo crea
				{
					log_info(logger_imongo, "No se pudo abrir el archivo Blocks.ims" );    //	QUE PASA SI FALLA?
				}

				bloques = malloc (  sizeof(char) * Block_size * blocks_cantidad );    //INICIALIZO BLOQUES

				read(fd, bloques, Block_size * blocks_cantidad );

				log_info(logger_imongo, "SE COMPRUEBA Y SE CARGA QUE EXISTE ARCHIVO Blocks.ims" );				// RUTINA DE CARGA DE ARCHIVO BLOCKS

		close (fd);

		}else{  // no existe blocks.ims


				if ((fd=open( test, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)   //lo crea
					{
						log_info(logger_imongo, "No se pudo abrir el archivo Blocks.ims" );    //	QUE PASA SI FALLA?
					}

		    crearBlocks( super );  

				if (  write(fd, bloques, Block_size* blocks_cantidad)  == -1 )    //la idea es cambiarle el tamanio se sepuede usar truncate
					{
								log_info( logger_imongo, "ERROR AL ESCRIBIR EL ARCHIVO Blocks.ims");
					}else{
								log_info( logger_imongo, "SE  ESCRIBIO CON EXITO EL ARCHIVO Blocks.ims");								
					}

					close(fd);	
			}			
		
		free(test);
}


void destroySuperbloque(t_superbloque * super){

	free(super->bitmap);

	free(super);
}


void guardarBitArray(char * path, t_bitarray* bitarray, uint32_t bytes_por_bloque, uint32_t cantBloques ){

		int fd;

		char *test = string_duplicate(path);

		string_append_with_format(&test, "/%s", "Blocks.ims" );


			
		if ((fd=open( test, O_RDWR |O_CREAT, S_IRUSR|S_IWUSR)) == -1)
		{
			fprintf(stderr, "NO SE PUDO GUARDAR EL ARCHIVO Blocks.ims \n");
			log_info( logger_imongo, "ERROR AL GUARDAR ARCHIVO Blocks.ims");
		
		}else{
				log_info( logger_imongo, "SE ABRIO EL ARCHIVO Blocks.ims");
		}

		t_superbloque* superbloque = malloc(sizeof(t_superbloque));		
		
		superbloque = crear_t_superbloque( bitarray,  bytes_por_bloque,  cantBloques);

		void * magic = serializar_paquete_t_superbloque(superbloque);

		if (  write(fd, magic, tamanio_bytes_superbloque(superbloque)) == -1 ){

					log_info( logger_imongo, "ERROR AL ESCRIBIR EL ARCHIVO Blocks.ims");
		}else{
					log_info( logger_imongo, "SE  ESCRIBIO CON EXITO EL ARCHIVO Blocks.ims");

		}


		free(superbloque->bitmap);
	

		free(superbloque);	

		free(test); 

		free(magic);

		close(fd);
}


bool inicializarSuperbloque(char *path){   //en caso de que no exista superbloque hay que ingresar por consola los datos

	log_info( logger_imongo, "Procediendo a crear Archivo SuperBloque.ims");
	printf("Inicializando Superbloque...\n" );	

		int numero, numero2;
		
		printf("\nEl Usuario debera ingresar los Valores Iniciales de Block_size y Cantidad de Bloques\n");
		printf("Block_size: Ingrese el valor del tamaño en bytes de cada bloque:  \n");

		if(scanf("%d", &numero)==1){
		   Block_size = numero;

		   	   log_info( logger_imongo,"EL Usuario ingreso el valor Block size ");
	   }
		else{

 				log_info( logger_imongo,"usuario ingreso el valor Block size incorrecto");
			   printf("Error al ingresar numero\n");
			   return(false);
		}

	   printf("Blocks: Ingrese la cantidad de bloques del File System \n");

	   if(scanf("%d", &numero2)==1){
	   		   blocks_cantidad = numero2;

	   		   	   log_info( logger_imongo,"El usuario ingreso el cantidad de bloques del FileSystem");
	   	   }
	   		else{
	   			   printf("No es número\n");
					log_info( logger_imongo,"usuario ingreso el valor Block incorrecto");
					return(false);
	   		}

		log_info( logger_imongo, "SE EXTRAJERON LOS DATOS DE INICIALIZACION DESDE CONSOLA");

		return(true);

}


void abrirSuperbloque(char * path){     

		int fd;
	

		uint32_t bytes_por_bloque;

		char *test = string_duplicate(path);

		string_append_with_format(&test, "/%s", "SuperBloque.ims" );


		path_del_superbloque = string_duplicate(test);

  		log_info( logger_imongo,"Abriendo el Archivo SuperBloque.... ");
	  


		if( verificar_existe_archivo(test) == 0 ){ // devuelve 0 si existe el archivo/directorio, y -1 si no existe el archivo/directorio


		if ((fd=open( test, O_RDWR  , S_IRUSR|S_IWUSR)) == -1)   // si existe lo crear e inicializa
		{
			fprintf(stderr, "NO SE PUDO ABRIR ARCHIVO SuperBloque.ims \n");
			log_info( logger_imongo, "ERROR AL ABRIR ARCHIVO SuperBloque.ims");
		}else{
			log_info( logger_imongo, "ARCHIVO ABIERTO: SuperBloque.ims");
		}

		read(fd, &Block_size, sizeof(uint32_t) );

		read(fd, &blocks_cantidad, sizeof(uint32_t) );

		bytes_por_bloque = Block_size;

		int cantidadBloques = blocks_cantidad;

		//tamanioSuperbloque tiene el tamanio en BYTES del bloque 8 bits son 1 BYTE de superbloque
		int tamanioSuperbloque=cantidadBloques/8;

				if(cantidadBloques % 8 != 0){
					tamanioSuperbloque++;
				}

	// --- 	SET SUPERBLOQUE -------------------------

		superbloque_global = malloc( sizeof(t_superbloque) );

		superbloque_global->block_size = bytes_por_bloque;

		superbloque_global->blocks= blocks_cantidad;

		superbloque_global->bitmap = malloc( tamanioSuperbloque  );

		memset( superbloque_global->bitmap , 0 , tamanioSuperbloque );


		bitmap_global = malloc ( tamanioSuperbloque );    //INICIALIZO BITMAP_GLOBAL

		read(fd, bitmap_global, tamanioSuperbloque );
	
		memcpy(superbloque_global->bitmap,  bitmap_global , tamanioSuperbloque);
	
		tamanio_bytes_bitmap_global = tamanioSuperbloque;

		
		bitarray_global = malloc(sizeof(t_bitarray));    //INICIALIZO bitarray_global   
		bitarray_global = bitarray_create_with_mode(bitmap_global, tamanioSuperbloque, MSB_FIRST);

		} else{    // no existe

		inicializarSuperbloque(path);

		if ((fd=open( test, O_RDWR | O_CREAT , S_IRUSR|S_IWUSR)) == -1)   // si existe lo crear e inicializa
			{
				fprintf(stderr, "NO SE PUDO CREAR EL ARCHIVO SuperBloque.ims \n");
				log_info( logger_imongo, "ERROR AL CREAR EL ARCHIVO SuperBloque.ims");
				exit(-1);
			}

				
		bitarray_global = malloc(sizeof(t_bitarray));    //INICIALIZO bitarray_global
		
		bitarray_global = crearBitArray( blocks_cantidad);   

		superbloque_global = malloc( sizeof(t_superbloque) );

		superbloque_global = crear_t_superbloque( bitarray_global,  Block_size, blocks_cantidad);

		void * magic = serializar_paquete_t_superbloque(superbloque_global);

		if (  write(fd, magic, tamanio_bytes_superbloque(superbloque_global)) == -1 ){

						log_info( logger_imongo, "ERROR AL ESCRIBIR EL ARCHIVO SuperBloques.ims");
			}else{
						log_info( logger_imongo, "SE ESCRIBIO CON EXITO EL ARCHIVO SuperBloque.ims");

		}	
	
		free(magic);
	} 	
	

	close(fd);

	free(test);	
}

t_superbloque* crear_t_superbloque(t_bitarray* result, uint32_t bytes_por_bloque, uint32_t cantidadBloques){


	t_superbloque* superbloque = malloc( sizeof(t_superbloque) );

	superbloque->block_size = bytes_por_bloque;

	superbloque->blocks= cantidadBloques;

	superbloque->bitmap = malloc( tamanio_bytes_char_del_bitArray(result)  );

	tamanio_bytes_bitmap_global = tamanio_bytes_char_del_bitArray(result);

	//strcpy( superbloque->bitmap,  result->bitarray );

	memcpy(superbloque->bitmap,  result->bitarray , tamanio_bytes_char_del_bitArray(result));

	return superbloque;
}



void* serializar_paquete_t_superbloque(t_superbloque* result)
{
	//---PROTOCOLO:    4 bytes de Block_size  +  4 bytes de Blocks  +  size del tamanio del char*

	void * magic = malloc(tamanio_bytes_superbloque(result));

	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(result->block_size), sizeof(uint32_t) );
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(result->blocks), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, result->bitmap, tamanio_bytes_char_del_superbloque(result) );
	desplazamiento+= tamanio_bytes_char_del_superbloque(result);

	return magic;
}



int tamanio_bytes_superbloque(t_superbloque* bitmap){

	int cantidadBloques = bitmap->blocks;

	int tamanioSuperbloque=cantidadBloques/8;

		if(cantidadBloques % 8 != 0){
			tamanioSuperbloque++;
		}

	return tamanioSuperbloque + sizeof(uint32_t) + sizeof(uint32_t);
}



int tamanio_bytes_char_del_superbloque(t_superbloque* bitmap){

	int cantidadBloques = bitmap->blocks;

	int tamanioSuperbloque=cantidadBloques/8;

		if(cantidadBloques % 8 != 0){
			tamanioSuperbloque++;
		}

	return tamanioSuperbloque;
}



int tamanio_bytes_bitArray(t_bitarray * bitmap){

	uint32_t cantBloques = bitmap->size;

	return cantBloques + sizeof(uint32_t) + sizeof(uint32_t);
}



uint32_t tamanio_bytes_char_del_bitArray(t_bitarray * bitmap){

	uint32_t cantBloques = bitmap->size;

	return cantBloques;
}



t_bitarray* crearBitArray(uint32_t cantBloques){   //SETTEA EL bitmap global usar cuando no existe el bitmap global

	int tamanioBitarray=cantBloques/8;

	if(cantBloques % 8 != 0){
	tamanioBitarray++;
	}

	char* bits=malloc(tamanioBitarray);

	memset(bits, 0, tamanioBitarray );

	t_bitarray * bitarray = malloc(sizeof(t_bitarray));
	
	bitarray = bitarray_create_with_mode(bits, tamanioBitarray, MSB_FIRST);

	bitmap_global = malloc(tamanioBitarray);

	memcpy(bitmap_global, bitarray->bitarray, tamanioBitarray );    

	
	free(bits);


	return bitarray;
}


void imprimir_bits_bitarray_global( void ){   //-----> Test Function

 		t_superbloque*	superbloque = malloc( sizeof(t_superbloque) );

		superbloque->block_size = Block_size;

		superbloque->blocks= blocks_cantidad;

		superbloque->bitmap = malloc( tamanio_bytes_bitmap_global  );

	
		memcpy( superbloque->bitmap  , bitmap_global ,tamanio_bytes_bitmap_global );
			
		imprimir_bloques_superbloque(superbloque);   /// no sacar es parte de la funcion


		free(superbloque);

}



void imprimir_bloques(t_bitarray* bitmap, uint32_t cantBloques){

	int i=0, j=0, k;

	uint32_t index=0;

		j = cantBloques/8;

		if( cantBloques % 8 != 0){
		j++;
		}

		for(k=0; k<j; k++){
			for(i=0; i<8; i++){

				if(index<cantBloques)printf("%d", bitarray_test_bit(bitmap, index));

				index++;
			}
			printf("\n");
		}
		printf("\n");
}



void imprimir_bloques_superbloque(t_superbloque* superbloque){

	t_bitarray* bitmap = bitarray_create_with_mode(superbloque->bitmap, tamanio_bytes_char_del_superbloque(superbloque)  ,MSB_FIRST);


	int i=0, j=0, k;
	uint32_t index=0;
	int cantBloques=superbloque->blocks;

		j = cantBloques/8;

		if( cantBloques % 8 != 0){
		j++;
		}

		for(k=0; k<j; k++){
			for(i=0; i<8; i++){

				if(index<cantBloques){ printf("%i", bitarray_test_bit(bitmap, index)); }

				index++;
			}
			printf("\n");
		}
		printf("\n");

}



void setters_bit_1_al_bitmap_global(int index){    // uno solo tiene mucho overhead el code 

	char* bits=malloc(tamanio_bytes_bitmap_global);

	memset(bits, 0, tamanio_bytes_bitmap_global );

	memcpy(bits, bitmap_global, tamanio_bytes_bitmap_global );     ///-----

	t_bitarray * bitarray = malloc(sizeof(t_bitarray));
	
	bitarray = bitarray_create_with_mode(bits, tamanio_bytes_bitmap_global, MSB_FIRST);
	
	bitarray_set_bit(bitarray, index);

	memcpy(bitmap_global, bitarray->bitarray , tamanio_bytes_bitmap_global );     //---


	free(bits);
}



void setters_bit_0_al_bitmap_global(int index){    // uno solo tiene mucho overhead el code 

	char* bits=malloc(tamanio_bytes_bitmap_global);

	memset(bits, 0, tamanio_bytes_bitmap_global );

	memcpy(bits, bitmap_global, tamanio_bytes_bitmap_global ); 

	t_bitarray * bitarray = malloc(sizeof(t_bitarray));
	
	bitarray = bitarray_create_with_mode(bits, tamanio_bytes_bitmap_global, MSB_FIRST);
	
	bitarray_clean_bit(bitarray, index);
	
	memcpy(bitmap_global, bitarray->bitarray , tamanio_bytes_bitmap_global ); 

	free(bits);
}



t_list * lista_bloques_vacios(int cantidad_bloques){    //PONER SIEMPRE DESPUES DE INICIALIZAR EL IMONGO   RETORNA UNA LISTA DE INT CANTIDAD DE BLOQUES QUE ESTEN VACIOS (LIBRES)
																// esta funcion derberia llevar MUTEX  

		t_list *lista_int_bloques = list_create();
	
		int k;

		uint32_t index=0;

		int cantBloques=blocks_cantidad;

			for(k=0; k<cantBloques; k++){	   
				
					if(index<cantidad_bloques){	 

						int nuevo = malloc(sizeof(int));
						nuevo = k;
						if( 0 == (int)bitarray_test_bit(bitarray_global, k ) ){

							list_add(lista_int_bloques, nuevo);
							index++;						
						}

					}else{   
						if(index==cantidad_bloques){ break;}   //BREAK FOR EVITAR ESPERA ACTIVA
						}
				
			}


	return  lista_int_bloques;
}


//---------->>>>>>>>test funcion
void imprimir_lista(t_list * lista){  

	list_iterate(lista, (void*)print_list);
}


// ---------<<<<<<<<
void print_list(int elemento){

	printf("elemento: %d " , elemento );
}

// FALLA: no le pasa el size del archivo
void persistir_en_bitmap_global_lista(t_list * lista, char * buffer, int tamanioBuffer ){     //la funcion es usada para guardar usando lista 		PERSISTE EL RESTO TMB
//una solucion a la lista mas chica que el actual es mandar la lista actual
		int i;

		int inicioBloque=0;    //<<<<<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---

		int sizebuffer = tamanioBuffer; //strlen(buffer);  

		t_list_iterator* iterador;


		iterador = list_iterator_create(lista);

		int contador =0;

		while(list_iterator_has_next(iterador) ){    //PRIMERO EL BITMAP GLOBAL  

			i = (int)list_iterator_next( iterador);		
	
			bitarray_set_bit(bitarray_global, i );


			char * llenador;
			

			if( sizebuffer >= Block_size) { //llena el ultimo
					
					llenador = (char *)malloc(sizeof(char) * Block_size);
					
				//	llenador = string_repeat( caracter, Block_size);

					memcpy(llenador  , buffer + contador * Block_size, Block_size ); 

					memcpy(bloques_mapper + Block_size * i  , llenador, Block_size ); 

					tamanioBuffer=tamanioBuffer-Block_size;

					sizebuffer = sizebuffer - Block_size;

					free(llenador);

			}else{  
							
					llenador = (char *)malloc(sizeof(char) * sizebuffer);

				//	llenador = string_repeat( caracter, tamanioBuffer);

					memcpy(llenador  , buffer + contador * Block_size, sizebuffer ); 


					memcpy(bloques_mapper + Block_size *i  , llenador, sizebuffer ); 

					//NO HARIA FALTA MODIFICAR EL SIZEBUFFER PORQUE ACA SERIA EL ULTIMO EN PERSISTIR
					free(llenador);
				}		
			contador++;
		}

	//	memcpy(bloques_mapper + Block_size*first  , buffer, sizebuffer ); // EN EL BLOCKS.IMS  + 1  PARA METER     ------XXXXXX MODIFICADO mapea a partir del primero
		memcpy(superbloque_bits_mapper + sizeof(uint32_t) + sizeof(uint32_t)  , bitarray_global->bitarray, tamanio_bytes_bitmap_global ); // EN EL SUPERBLOQUE

		mappear_file_system();
}

// FALLA: no le pasa el size del archivo
void persistir_en_bitmap_global_listaAUXILIAR(t_list * lista, char * buffer, int tamanioBuffer, int inicioBloque ){     //la funcion es usada para guardar usando lista 		PERSISTE EL RESTO TMB
//una solucion a la lista mas chica que el actual es mandar la lista actual
		int i;

		//int inicioBloque=0; // MAXIMO BLOCKS_SIZE<<<<<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---<<-----<<<---

		 int flag=0;   
		int sizebuffer = tamanioBuffer; //strlen(buffer);  

		t_list_iterator* iterador;


		iterador = list_iterator_create(lista);   //LA LISTA TIENE QUE INCLUIR SI O SI LA ULTIMA

		int contador =0; 	int RESTO;

		while(list_iterator_has_next(iterador) ){    //PRIMERO EL BITMAP GLOBAL  

			i = (int)list_iterator_next( iterador);		
	
			bitarray_set_bit(bitarray_global, i );
			//int tamanioInicial;

			char * llenador;
			
			if(flag == 0  ){
				//if(tamanioBuffer<=Block_size){          }

				llenador = (char *)malloc(sizeof(char) * Block_size);

				 RESTO = Block_size - inicioBloque;  //inicioBloque siempre es >= que  Block_size

				memcpy(llenador  , buffer , RESTO ); 

				memcpy(bloques_mapper + Block_size * i  + (inicioBloque), llenador, RESTO ); 

				tamanioBuffer=tamanioBuffer - RESTO;

				sizebuffer = sizebuffer - RESTO;

				free(llenador);

				//flag=1;

				contador=0;
			}

			if( flag != 0 ){
				if( sizebuffer >= Block_size  ) { //llena el ultimo
						
						llenador = (char *)malloc(sizeof(char) * Block_size);
						
					//	llenador = string_repeat( caracter, Block_size);

						memcpy(llenador  , buffer + (contador * Block_size ) + RESTO, Block_size ); 

						memcpy(bloques_mapper + Block_size * i  , llenador, Block_size ); 

						tamanioBuffer=tamanioBuffer-Block_size;

						sizebuffer = sizebuffer - Block_size;

						free(llenador);

				}else{  
								
						llenador = (char *)malloc(sizeof(char) * sizebuffer);

					//	llenador = string_repeat( caracter, tamanioBuffer);

						memcpy(llenador  , buffer + (contador)* Block_size + RESTO, sizebuffer ); 


						memcpy(bloques_mapper + Block_size *i  , llenador, sizebuffer ); 

						//NO HARIA FALTA MODIFICAR EL SIZEBUFFER PORQUE ACA SERIA EL ULTIMO EN PERSISTIR
						free(llenador);
					}
					//---<<<<<<<<<  CONTADOR?
					contador++;
			}		

				flag=1;
			
		}

	//	memcpy(bloques_mapper + Block_size*first  , buffer, sizebuffer ); // EN EL BLOCKS.IMS  + 1  PARA METER     ------XXXXXX MODIFICADO mapea a partir del primero
		memcpy(superbloque_bits_mapper + sizeof(uint32_t) + sizeof(uint32_t)  , bitarray_global->bitarray, tamanio_bytes_bitmap_global ); // EN EL SUPERBLOQUE

		mappear_file_system();
}


void  persistir_en_bitmap_global(char * buffer ){    //PERSITENCIA A MEMORIA de bits

	memcpy(superbloque_bits_mapper  + sizeof(uint32_t) + sizeof(uint32_t), buffer, tamanio_bytes_bitmap_global ); 
}



void persistir_en_BLOCKS_IMS_bitmap(char * buffer, int inicial, int tamanio){    //  ACHTUNG WIEDERSEHEN! PERSISTE EN DONDE ESTE EL PUNTO DE MONTAJE DEL ARCHIVO CONFIG

		memcpy(bloques + inicial, buffer, tamanio ); 
}


t_list * almacenar_en_memoria_del_file_system( t_files_config * RECURSOS_config, char * buffer, int * correcto  ){  //PROBLEMA NO CONTEMPLA LOS BYTES AL FIN DE LA LISTA

		int tamanio = strlen(buffer);  int restoAMandar=0;

		char  * bufferAuxiliar;

		int tamLista =0;  int parte1; int parterRestante;

		int restante=0;  //restante de todo lo que hay que escribir
	//	int restanteBlock =0;   // restante del ultimobloque anterior a escribir

		tamLista = list_size( RECURSOS_config->blocks );
		
		int ultimo_bloque=0;
		int contador=0;
		int cantidad_bloques_necesarios;

		
//restante sin aplicar 
//	if(tamLista > 0){
		contador = RECURSOS_config->size  - (( tamLista - 1) * Block_size );   //cantidad de tiene el ultimo bloque
		restante = Block_size * tamLista  - RECURSOS_config->size;   //restante:  FALLA: LA LISTA PUEDE ESTAR VACIA. RESTANTE DE ULTIMA LISTA
		restoAMandar = tamanio - restante;
		
//		restanteBlock =  Block_size * tamLista  - RECURSOS_config->size;

//		}else{  //tamLista == 0
			 
//			restante = Block_size;// * tamLista  + RECURSOS_config->size; 
//			contador = RECURSOS_config->size;
	//	}


	//	if( (tamanio -restante) >= 0 ){ 

	//		cantidad_bloques_necesarios=0;

	//		listaLibre  ------>>>>>>  pasar ultimo bloque  o lista null, porque significa que eya esta set el bit de bloque

	//		}else{

			cantidad_bloques_necesarios=( tamanio - restante ) /Block_size;    

			if( (tamanio -restante ) % Block_size != 0 ){

				cantidad_bloques_necesarios++;
			}
	//	}
    t_list * listaLibre = malloc(sizeof(t_list));

	

	 if ( (tamanio - restante) <= 0 ) {
			listaLibre = list_create();
			//listaLibre ( ) // --->> poner el ultimo bloque   y assignar a 
			cantidad_bloques_necesarios =0;

	 }else{    //lista nueva + agregar el ultimo + listaLibre + mandar inicio

			listaLibre = lista_bloques_vacios(cantidad_bloques_necesarios);  //pido una lista de bloques vacios AL BITMAP (BITS) EL PROBLEMA ES EL OVERHEAD, HAY MUCHO PARA UNA SIMPLE PERSISTENCIA
		
			if( list_size( listaLibre )  != cantidad_bloques_necesarios){    // && cantidad_bloques_necesarios != 0   ----->>>>>>>

				log_error(logger_imongo,"ERROR : NO HAY ESPACIO DE BLOQUES SUFICIENTE EN EL IMONGO FILE SYSTEM");
				*correcto = -1;
				return NULL;
			}


	 }  

    
		if(tamLista>0){      //que tiene lista previa o sea puede ser nuevo(vacio) o que ya tenga lista y tengo que hacer la carga

		/*	int ultimo_bloque, i;
			int contador=0;
			int diferencia=0;
*/
			ultimo_bloque = list_get( RECURSOS_config->blocks , tamLista - 1);  //  ultimo  bloque es el numero de bloque
/*
			restante = Block_size * tamLista  - RECURSOS_config->size;

			contador = RECURSOS_config->size  - (( tamLista - 1) * Block_size );   //cantidad de tiene el ultimo bloque

			*/
			if( restante != 0  ){ //indica cuantos le faltan 	------->>>>>>>>  podria ser < 0  y un else			
					

					if ( (tamanio - restante) > 0 ){

							bufferAuxiliar = malloc( sizeof(char) * ( tamanio - restante)   );	

							memcpy(bufferAuxiliar, buffer + restante ,  tamanio - restante ); //ok al bitmap_global marcar los que no incluyan el ultimo bloque  --->>>>>>>>>>>>>>>CORREGIR
										
							int val =  ( Block_size * ultimo_bloque ) + contador;
		
							memcpy(bloques_mapper + val, buffer, restante ); //al bloques_mapper escribo aca    ------>>>>>>>>>>CORREGIR 

					}else{

							bufferAuxiliar = malloc( sizeof(char) * ( tamanio- restante)   );	

							memcpy(bufferAuxiliar, buffer,  tamanio ); //ok al bitmap_global marcar los que no incluyan el ultimo bloque  --->>>>>>>>>>>>>>>CORREGIR
									
							int val =  ( Block_size * ultimo_bloque ) ;
	
							memcpy(bloques_mapper + val, buffer, tamanio -restante);	
					}
				

			}else{
					bufferAuxiliar = string_duplicate(buffer);  //ESCRIBIR RESTANTE				
			}


		}else{   	//NO TIENE LISTA PREVIA  ---> RESTANTE IGUAL CALCULA LA CATINDAD ACTUAL  // TRABAJA SOBRE LA ULTIMA LISTA


			//	bufferAuxiliar = string_duplicate(buffer);	
			if(tamanio>Block_size){  parte1=Block_size; parterRestante=tamanio-Block_size;}else{
				parte1=tamanio;  parterRestante=0;
			}

			 restoAMandar=tamanio;   //porque es la primera vez
					bufferAuxiliar = malloc( sizeof(char) * ( parte1)   );	

				memcpy(bufferAuxiliar, buffer,  parte1 ); //ok al bitmap_global marcar los que no incluyan el ultimo bloque  --->>>>>>>>>>>>>>>CORREGIR
								
					int val =  ( Block_size * ultimo_bloque ) + RECURSOS_config->size;
 
					memcpy(bloques_mapper + val, buffer, parte1 );	

				//	list_remove(listaLibre , 0);
					//el bitmap ya estaria marcado asi que no hace falta persistirlo
		}

/* 
		int cantidad_bloques_necesarios=( tamanio - restante ) /Block_size;    
 
		if( (tamanio -restante ) % Block_size != 0 ){
			cantidad_bloques_necesarios++;
		}

    t_list * listaLibre = malloc(sizeof(t_list));

	listaLibre = lista_bloques_vacios(cantidad_bloques_necesarios);  //pido una lista de bloques vacios AL BITMAP (BITS) EL PROBLEMA ES EL OVERHEAD, HAY MUCHO PARA UNA SIMPLE PERSISTENCIA

   
    if( list_size( listaLibre )  != cantidad_bloques_necesarios){

        log_error(logger_imongo,"ERROR : NO HAY ESPACIO DE BLOQUES SUFICIENTE EN EL IMONGO FILE SYSTEM");
		return -1;
    }
    */
   if(list_size(listaLibre) != 0 ){
		persistir_en_bitmap_global_lista( listaLibre , bufferAuxiliar, restoAMandar );  // bitmap son los bit de ESCRITURA del superbloque  PRIMERO EL BITMAP GLOBAL REFACTORIZAR INCLUIR EL MAPPER
   }
 

	free(bufferAuxiliar);

    return listaLibre;
     
}   //ALMACENAJE DIRECTO NO VERIFICA SI HAY UN ELEMENTO AL FINAL DE UN BLOQUE


t_list * almacenar_en_memoria_del_file_systemAUXILIAR( t_files_config * RECURSOS_config, char * buffer, int * correcto  ){  //PROBLEMA NO CONTEMPLA LOS BYTES AL FIN DE LA LISTA

		int tamanio = strlen(buffer); 
		correcto = 0;


		int tamLista =0;  int parte1; int parterRestante;

		int restante=0;  //restante de todo lo que hay que escribir
	
		tamLista = list_size( RECURSOS_config->blocks );
		
		int ultimo_bloque=0;
		int contador=0;
		int cantidad_bloques_necesarios;

			
		contador = RECURSOS_config->size  - (( tamLista - 1) * Block_size );   //cantidad de tiene el ultimo bloque
		restante = Block_size * tamLista  - RECURSOS_config->size;   //restante:  FALLA: LA LISTA PUEDE ESTAR VACIA. RESTANTE DE ULTIMA LISTA
	
		
		int ocupado;

		if( tamLista > 0  ){   //cantidad de tiene el ultimo bloque

				if( RECURSOS_config->size > Block_size ){

						contador = RECURSOS_config->size  - (( tamLista - 1) * Block_size );   //contador tienen el ultimo bloque
						
				}else{
						contador = RECURSOS_config->size;   //contador tienen el ultimo bloque
						
				}

		}else{
				contador =0;			
		}

		restante = Block_size - contador;

		if(tamLista>0){   
		
						if(tamanio>restante){

							cantidad_bloques_necesarios=( tamanio - restante ) /Block_size;    

							if( (tamanio -restante ) % Block_size != 0 ){

								cantidad_bloques_necesarios++;
							}

						}else{
								cantidad_bloques_necesarios =0;
						}

					}else{

							if(tamanio>restante){

								cantidad_bloques_necesarios=( tamanio  ) /Block_size;    

								if( (tamanio  ) % Block_size != 0 ){

									cantidad_bloques_necesarios++;
								}

							}else{
									cantidad_bloques_necesarios =1;
							}
							
					}

//--------------------------COMPROBADOR DE FIN DE FILE SYSTEM-----------------------------------


		t_list * listaNuevaComprobacion = lista_bloques_vacios(cantidad_bloques_necesarios);
 
	if( list_size( listaNuevaComprobacion ) != cantidad_bloques_necesarios  && tamLista > 0 )
	{
				correcto = 1;
				log_error( logger_imongo, " NO SE PUEDEN ESCRIBIR MAS BLOQUES EN EL FILE SYSTEM" );
				return NULL;
	}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
    t_list * listaLibre;
	 t_list * listaLibreAUXILIAR;

				if(tamLista > 0 ){   // que tiene lista anterior				

						if(cantidad_bloques_necesarios >0 ){

							listaLibre = list_create();	

							list_add(listaLibre, list_get(  RECURSOS_config->blocks , tamLista-1  ));

							listaLibreAUXILIAR = malloc(sizeof(t_list));

							listaLibreAUXILIAR =   lista_bloques_vacios(cantidad_bloques_necesarios);

							  //LIST ADD agrega un ELEMENTO, no agrega LISTA
							list_add_all(listaLibre, listaLibreAUXILIAR);

						}else{	


							listaLibre = list_create();				
							///list devuelve el valor de la lista

							list_add(listaLibre, list_get(  RECURSOS_config->blocks , tamLista-1  ));

							listaLibreAUXILIAR = list_create();
						}						


				}else{   //no tiene lista anterior  


						if(cantidad_bloques_necesarios >1 ){   //si es nuevo siempre necesitaria  una lista nueva?

							listaLibreAUXILIAR =   lista_bloques_vacios(cantidad_bloques_necesarios);

							listaLibre = list_duplicate(listaLibreAUXILIAR);


						}else{ // == 1 
						
							listaLibreAUXILIAR = malloc(sizeof(t_list));
							
							listaLibreAUXILIAR =   lista_bloques_vacios(cantidad_bloques_necesarios);

							listaLibre = list_duplicate(listaLibreAUXILIAR);

						}
				}


   if(list_size(listaLibre) != 0 ){
		persistir_en_bitmap_global_listaAUXILIAR( listaLibre , buffer, tamanio, contador );  // bitmap son los bit de ESCRITURA del superbloque  PRIMERO EL BITMAP GLOBAL REFACTORIZAR INCLUIR EL MAPPER
   }   

//	free(bufferAuxiliar);

    return listaLibreAUXILIAR;
     
}  


///---------------------------------------------------


void liberar_memoria_del_file_system(t_list * lista){    //AL FILESYSTEM

		int i;
        t_list_iterator* iterador;

        iterador = list_iterator_create(lista);

        
         char* bufferProvisorio = (char *)malloc(sizeof(char) * Block_size);

        bufferProvisorio = string_repeat(0 , Block_size);



        while(list_iterator_has_next(iterador) ){

            i = (int)list_iterator_next( iterador);

        	memcpy(bloques_mapper + i*Block_size  , bufferProvisorio, Block_size );
                      
        }

		liberar_en_bitmap_global_lista(lista);

		free(bufferProvisorio);

        free(iterador);

}


void liberar_en_bitmap_global_lista(t_list * lista){    

		int i;

		t_list_iterator* iterador;

		iterador = list_iterator_create(lista);


		while(list_iterator_has_next(iterador) ){    //PRIMERO EL BITMAP GLOBAL

		i = (int)list_iterator_next( iterador);
 
	 	bitarray_clean_bit(bitarray_global, i );

		}

		memcpy(superbloque_bits_mapper + sizeof(uint32_t) + sizeof(uint32_t)  , bitarray_global->bitarray, tamanio_bytes_bitmap_global ); // EN EL SUPERBLOQUE


	//	mappear_file_system();     -------------------------------->>>>>>>>>>>>ESTO DEPENDE DE QUE FEEDBACK NOS DAN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}









void file_system_finally(){

		munmap(bloques,   blocks_cantidad * Block_size );   

		munmap(superbloque_bits_mapper,   blocks_cantidad * Block_size );   


		bitarray_destroy(bitarray_global);
		
		free(superbloque_global);

		//free( );

		free(path_del_block_ims);

		free(path_del_superbloque);

		free(bloques);
}
