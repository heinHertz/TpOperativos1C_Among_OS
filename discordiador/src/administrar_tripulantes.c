#include "administrar_tripulantes.h"

void hilo_imprime_texto(void *ubicaciones)
{    char* texto = (char *)ubicaciones;
     printf( "%s\n", texto);
}

void hilo_tripulante(t_tripulante* tripulante)
{    // agrego el tripulante nuevo a la lista de tripulantes
     printf("Ubicacion X: %d\n",tripulante->posicion_X);
     printf("Ubicacion Y: %d\n", tripulante->posicion_Y);
     printf("Archivo de tareas: %s\n",tripulante->archivo_tareas);
     printf("Id de tripulante: %d\n", tripulante->id_tripulante );
     printf ("El tripulante pertenece a la patota: %d \n",id_patota_de_tripulante(tripulante));
     
     char* id_patota = string_itoa(id_patota_de_tripulante(tripulante));
     char* id_tripulante = string_itoa(tripulante->id_tripulante);
     char* posicion_Y = string_itoa(tripulante->posicion_Y);
     char* posicion_X = string_itoa(tripulante->posicion_X);

    // enviar_mensaje_iniciar_tripulante(modulo_memoria,id_patota,id_tripulante,posicion_X,posicion_Y);
     
     char* tarea = proxima_tarea_a_realizar(tripulante);
     tripulante->tarea = procesar_proxima_tarea(tarea);
     tripulante->estado = READY;
    
     pthread_mutex_lock(&mutex_cola_ready);
     list_add(cola_ready,tripulante);   //----------->>>>>>>> SE PODRIA UTILIZAR t_queue * si es una COLA
     pthread_mutex_unlock(&mutex_cola_ready);
     // variable mutex
     sem_post(&s_cola_ready_con_items);
     
     sem_wait(&s_procesos_en_exec);
     sem_wait(&tripulante->s_tripulante);
     
     ejecutar(tripulante);
     
     sem_post(&s_procesos_en_exec);
}

void crear_patota(t_modulo* modulo,char* cantidad_tripulantes,char* path_tareas,char** ubicaciones){

   if( path_tareas == NULL || cantidad_tripulantes == NULL)
   { printf("Faltan parametros \n");
     return;}

char* string_tareas = leer_archivo_y_devolver_contenido(path_tareas);

   if (string_equals_ignore_case(string_tareas, "FAIL"))
   { log_error (logger,"Error al leer el archivo %s", path_tareas);
     return;}

t_tripulante* tripulante = malloc(sizeof(t_tripulante));
t_patota* patota = malloc(sizeof(t_patota));
patota->id_patota = asignar_id_patota();
log_info(logger, "El id de la patota a crear es %d\n", patota->id_patota);

int i = atoi(cantidad_tripulantes);
log_info(logger, "Y la cantidad de hilos tripulantes a crear es : %d \n",i);

char** string_ubicaciones = NULL;
char* ubicacion_actual;

char* id_patota = string_itoa(patota->id_patota);
char* respuesta = enviar_mensaje_iniciar_patota(modulo,id_patota,string_tareas); //inicia patota en miram
   
   if ( string_equals_ignore_case(respuesta,"FAIL") || string_equals_ignore_case(string_tareas,"FAIL"))
   {
     log_error(logger, "No se pudo crear la patota %d en MIRAMHQ", patota->id_patota);
     free(patota);
     return;}

else {
     list_add(lista_patotas,patota);
     patota->lista_tripulantes = list_create();

     for(int hilos_por_tripulante=0; hilos_por_tripulante<i; hilos_por_tripulante++)
     
     {ubicacion_actual = ubicaciones[hilos_por_tripulante+3]; // primer ubicacion en el string pasado
      string_ubicaciones = string_split(ubicacion_actual, "|");
      tripulante->posicion_X = atoi(string_ubicaciones[0]);
      tripulante->posicion_Y = atoi(string_ubicaciones[1]);
      tripulante->id_tripulante = asignar_id_tripulante();
      tripulante->archivo_tareas = path_tareas;
      tripulante->contador_tarea = 0;
      tripulante->estado = NEW;
      
      sem_init(&tripulante->s_tripulante,0,0);
      list_add(patota->lista_tripulantes,tripulante);
      
      log_info(logger, "\nCreo hilo con datos del tripulante...\n");
      pthread_t hilo;
      pthread_create(&hilo, NULL,(void*)hilo_tripulante, tripulante);
      pthread_join(hilo,NULL);
     }
   }
}

int asignar_id_tripulante(void)
{ sem_wait(&mutex_id_tripulantes);
  id_tripulantes++;
  sem_post(&mutex_id_tripulantes);
  return id_tripulantes;
}

int asignar_id_patota(void)
{ id_Patotas++;
  return id_Patotas;
}

int id_patota_de_tripulante(t_tripulante* tripulante)
{  int id_patota;
   t_patota* patota = malloc(sizeof(t_patota));
   t_tripulante* tripulante_a_comparar = malloc(sizeof(t_tripulante));
   
   for (int i=0;i < list_size(lista_patotas);i++)
   { patota = list_get(lista_patotas,i);
      for (int j=0;j < list_size(patota->lista_tripulantes);j++)
       { tripulante_a_comparar = list_get(patota->lista_tripulantes,j);
       if( tripulante_a_comparar->id_tripulante == tripulante->id_tripulante)
       { id_patota = patota->id_patota; }
     }
   }

   return id_patota;
}

void listar_tripulantes_por_consola(void)
{ t_tripulante* tripulante = malloc(sizeof(t_tripulante));
  t_patota* patota = malloc(sizeof(t_patota));
  
  log_info(logger,"Tripulantes actuales:");
   for (int i=0;i < list_size(lista_patotas);i++) // recorro la lista de patotas
   {   patota = list_get(lista_patotas,i);
       for(int j=0;j < list_size(patota->lista_tripulantes);j++) //en cada patota obtengo el tripulante
        { tripulante = list_get(patota->lista_tripulantes,j);
          log_info(logger,"Patota: %d    Tripulante: %d    Estado: %d"
          ,patota->id_patota,tripulante->id_tripulante,tripulante->estado);}
   }
}

char* leer_archivo_y_devolver_contenido(char* path_archivo)
{
 FILE* archivo;
   archivo = fopen(path_archivo, "r");
    if (archivo == NULL)
    {
    return "FAIL"; }

 long medida;

 char * texto;

 fseek (archivo , 0 , SEEK_END);
 medida = ftell (archivo);
 rewind (archivo);
 texto = (char*) malloc (sizeof(char)*medida);
 fread(texto, medida+1, 1, archivo);

 fclose(archivo);
 return texto;
}

t_tarea* procesar_proxima_tarea(char* tarea_a_procesar)
{ t_tarea* nueva_tarea= malloc(sizeof(t_tarea));
  char** string_tareas =  NULL;
  char** string_tareas2 = NULL;
  string_tareas = string_split(tarea_a_procesar,";"); // DORMIR 1;2;3;4
  
  nueva_tarea->posicion_destino_X =  atoi(string_tareas[1]);
  nueva_tarea->posicion_destino_Y = atoi(string_tareas[2]);
  nueva_tarea->tiempo = atoi(string_tareas[3]);

  string_tareas2 = string_split(string_tareas[0]," ");

  nueva_tarea->nombre = string_tareas2[0];
  
  if (string_tareas2[1] != NULL) nueva_tarea->parametro = atoi(string_tareas2[1]);

  return nueva_tarea;
}

char* proxima_tarea_a_realizar(t_tripulante* tripulante){
  int i;
  char* txt = leer_archivo_y_devolver_contenido(tripulante->archivo_tareas);
  
  char** array_tareas = string_split(txt,"\n");

  char* proxima_tarea = array_tareas[tripulante->contador_tarea];
  
  tripulante->contador_tarea = tripulante->contador_tarea + 1;
  
  log_info(logger,"%s", proxima_tarea);
  
  return proxima_tarea;
}

//void iniciar_planificador(void)
//   {  sem_post(&s_iniciar_planificacion); }
