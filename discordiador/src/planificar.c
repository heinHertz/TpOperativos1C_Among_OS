#include "planificar.h"

void ejecutar(t_tripulante* tripulante){
    
     log_info(logger, "Planificando hilo del tripulante %d\n",tripulante->id_tripulante);

     realizar_tarea(tripulante);

     char* tarea = proxima_tarea_a_realizar(tripulante);
    
     while ( tarea != NULL && tripulante->estado != BLOCKED){
    
      tripulante->tarea = procesar_proxima_tarea(tarea);
    
      realizar_tarea(tripulante);
    
      tarea = proxima_tarea_a_realizar(tripulante);
    }
}

void caminar(t_tripulante* tripulante)
{
    int final_X = tripulante->tarea->posicion_destino_X;
    int final_Y = tripulante->tarea->posicion_destino_Y;

    avanzar_X(final_X,tripulante);
    avanzar_Y(final_Y,tripulante); 
}

void avanzar_X(int final, t_tripulante* tripulante)
{    
   if ( final > tripulante->posicion_X)
   { while ( final != tripulante->posicion_X)
     {
         sleep(discordiador_config->retardo_ciclo_cpu);
         
         tripulante->posicion_X += 1;
         log_info( logger, "El tripulante %i se movio a la posicion %i|%i\n"
         ,tripulante->id_tripulante, tripulante->posicion_X,tripulante->posicion_Y );
         //----------ACA ES DONDE CORRESPONDE PONER EL MENSAJE A IMONGO? -----
          mandar_a_file_system_bitacora_tripulante(  tripulante);
     }
   } 
   else if (final < tripulante->posicion_X)
   { while ( final != tripulante->posicion_X)
     {
         sleep(discordiador_config->retardo_ciclo_cpu);
        
         tripulante->posicion_X -= 1;
         log_info( logger, "El tripulante %i se movio a la posicion %i|%i\n"
         ,tripulante->id_tripulante, tripulante->posicion_X,tripulante->posicion_Y );
          //----------ACA ES DONDE CORRESPONDE PONER EL MENSAJE A IMONGO? -----
          mandar_a_file_system_bitacora_tripulante(  tripulante);
     }
    }
}

void avanzar_Y(int final, t_tripulante* tripulante)
{    
   if ( final > tripulante->posicion_Y)
   { while ( final != tripulante->posicion_Y)
     {
         sleep(discordiador_config->retardo_ciclo_cpu);
        
         tripulante->posicion_Y += 1;
         log_info( logger, "El tripulante %i se movio a la posicion %i|%i\n"
         ,tripulante->id_tripulante, tripulante->posicion_X,tripulante->posicion_Y );
          //----------ACA ES DONDE CORRESPONDE PONER EL MENSAJE A IMONGO? -----
         mandar_a_file_system_bitacora_tripulante(  tripulante);
     }
   } 
   else if (final < tripulante->posicion_Y)
   { while ( final != tripulante->posicion_Y)
     {
         sleep(discordiador_config->retardo_ciclo_cpu);
         
         tripulante->posicion_Y-= 1;
         log_info( logger, "El tripulante %i se movio a la posicion %i|%i\n"
         ,tripulante->id_tripulante, tripulante->posicion_X,tripulante->posicion_Y );
          //----------ACA ES DONDE CORRESPONDE PONER EL MENSAJE A IMONGO? -----
         mandar_a_file_system_bitacora_tripulante(  tripulante);
     }
    }
}

void realizar_tarea(t_tripulante* tripulante){
  t_tarea* proxima_tarea = tripulante->tarea;
  
  int32_t tarea_string = proxima_tarea_code(proxima_tarea->nombre);
  
  tripulante->estado = EXEC;
  
  caminar(tripulante); 

  switch(tarea_string){
    case 0: tripulante->estado = BLOCKED;
            
            tarea_io(proxima_tarea->nombre,proxima_tarea->tiempo,proxima_tarea->parametro);

            // falta pedir proxima tarea

            list_add(cola_blocked, tripulante);
            
            sem_post(&s_cola_bloqueados_con_items);
    break;
  
    default: tarea_normal(proxima_tarea->nombre, proxima_tarea->tiempo);
    
    break;
  }
}

void tarea_normal(char* nombre_tarea,int tiempo_tarea)
{
    log_info(logger,"Realizando la tarea %s por %d ciclos de CPU\n",nombre_tarea,tiempo_tarea);
    
    sleep(discordiador_config->retardo_ciclo_cpu);
    
    for(int i=0 ; tiempo_tarea > i ; i++)

    { log_info(logger,"Realizando la tarea %s\n",nombre_tarea);
    
      sleep(discordiador_config->retardo_ciclo_cpu); }

}

void tarea_io(char* nombre_tarea,int tiempo_tarea,int parametro_tarea)
{
    log_info(logger,"Realizando la tarea %s por %d ciclos de CPU\n",nombre_tarea,tiempo_tarea);
    
    char* parametro_enviar = string_itoa(parametro_tarea);
    
    enviar_mensaje_tarea(modulo_file_system,nombre_tarea,parametro_enviar);

    sleep(discordiador_config->retardo_ciclo_cpu);
    
    for(int i=0 ; tiempo_tarea > i ; i++)

    { log_info(logger, "Realizando la tarea %s\n" ,nombre_tarea);
    
     sleep(discordiador_config->retardo_ciclo_cpu); }
}

int32_t proxima_tarea_code(char* tarea){

    if(string_equals_ignore_case(tarea, "GENERAR_OXIGENO") 
    || string_equals_ignore_case(tarea, "CONSUMIR_OXIGENO")
    || string_equals_ignore_case(tarea, "GENERAR_COMIDA")
    || string_equals_ignore_case(tarea, "CONSUMIR_COMIDA")
    || string_equals_ignore_case(tarea, "GENERAR_BASURA")
    || string_equals_ignore_case(tarea, "CONSUMIR_BASURA"))
    {
    return TAREA_IO;}
	
    return TAREA_ESPECIAL;
}


void mandar_a_file_system_bitacora_tripulante( t_tripulante* tripulante){

      char * bitacora = string_new(); 

      string_append_with_format( &bitacora, "El tripulante %i se movio a la posicion %i|%i\n"
      ,tripulante->id_tripulante, tripulante->posicion_X, tripulante->posicion_Y );

      enviar_mensaje_bitacora_tripulante(modulo_file_system,  string_itoa( tripulante->id_tripulante ), bitacora );

      free(bitacora);


}