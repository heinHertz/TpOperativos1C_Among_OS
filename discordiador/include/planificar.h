#ifndef PLANIFICAR_H
#define PLANIFICAR_H

#include "shared_utils.h"
#include "server.h"
#include "api.h"
#include "administrar_tripulantes.h"

void ejecutar(t_tripulante* tripulante);
void caminar(t_tripulante* tripulante);
void avanzar_X(int final, t_tripulante* tripulante);
void avanzar_Y(int final, t_tripulante* tripulante);
int32_t proxima_tarea_code(char* tarea);
void realizar_tarea(t_tripulante* tripulante);
void tarea_normal(char* nombre_tarea,int tiempo_tarea);
void tarea_io(char* nombre_tarea,int tiempo_tarea,int parametro_tarea);

#endif