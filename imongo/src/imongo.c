#include "imongo.h"
#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>
#include <commons/string.h>
#include <pthread.h>
#include <unistd.h>
#include "archivos.h"
#include "bitacoras.h"
#include <signal.h>


int main( int argc, char *argv[] ){   
     
    
    signal(SIGUSR1, &procesar_signal);
  
    logger_imongo =  iniciar_logger();

    imongo_init();   

    init_file_system( punto_de_montaje_GLOBAL);  

    sincronizar_file_system(); 

    pthread_t thread2; 
    pthread_create(&thread2, NULL, (void*)tareaSincronizar, NULL);    
  	pthread_detach(thread2);

     
    pthread_t thread3; 
    pthread_create(&thread3, NULL, (void*)tareaServer, NULL);    
  	pthread_join(thread3, NULL);
      
    iniciar_servidor( "127.0.0.1", "5003" , handle_server);  //------->>>FORMATO LINEA


      // CONTROL_Bitmap();



      // MD5_CHECKSUM( "/home/utnso/Escritorio/sync/Files/Comida.ims" );

      //  CONSUMIR_OXIGENO(29);
     
      
      // TRIPULANTE_GENERAR_TAREA(1 , "TAREA_PARAMETROS;POSX;POSy;TIEMPO;" );


      // TRIPULANTE_GENERAR_TAREA(2 , "SE_MUEVE;10;12;150;" );

      // printf(" ----%s---", OBTENER_BITACORA(2));

      // RESTAURAR_FILE_RECURSO("/home/utnso/Escritorio/sync/Files/Oxigeno.ims" );


      // INICIAR_FSCK();

      // GENERAR_OXIGENO(14 );

      // GENERAR_COMIDA(10); 

      //  INICIAR_FSCK();

      // GENERAR_OXIGENO(10);

    //  GENERAR_OXIGENO(7);

    //  GENERAR_BASURA(2);

     // GENERAR_COMIDA(10);

     //   CONSUMIR_OXIGENO(6);

     //INICIAR_FSCK();
   //CONTROL_EN_FILES_SIZE_RECURSO("Oxigeno");

     //    CONSUMIR_COMIDA(4);

      //   CONSUMIR_BASURA(3);
      //CONSUMIR_OXIGENO(20);
      // DESCARTAR_BASURA();

   
   

   // imprimir_bits_bitarray_global();
      
   // sleep(10);

    file_system_finally();

    archivos_finally();

    imongo_finally(imongo_config, logger_imongo);    //siempre al fondo

 //  pthread_join(thread1, NULL);   //ESPERA QUE TERMINE EL HILO THR1  el NULL es el lugar donde le hilo RETORNA algun VALOR
    
    return 0;

}
         

t_log* iniciar_logger(void){

    logger_imongo = malloc(sizeof(t_log));

     logger_imongo =    init_logger("./cfg/imongo.log", "IMONGO",  LOG_LEVEL_INFO);

    log_info(logger_imongo, "Soy el imongo! %s  Iniciando Logger Imongo File System.....", mi_funcion_compartida());


      return logger_imongo;
}



void tareaSincronizar(){

    while(1)
   {		 
    	sleep( imongo_config->tiempo_sincronizacion  );
         
      mappear_file_system();   

   }

}

void tareaServer(){
    //while ?

     iniciar_servidor( "127.0.0.1", "5003" , handle_server);   

}

void conexionDiscordiador(){



}

void procesar_signal(int signal){

  INICIAR_FSCK();

}