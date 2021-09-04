#ifndef MENSAJES_CONSOLA_H
#define MENSAJES_CONSOLA_H

#include "shared_utils.h"
#include "server.h"
#include "api.h"

typedef struct {
    t_modulo* modulo;
    char* mensaje_completo;
} t_struct_mensajes;

void leer_consola(t_log* logger,t_modulo* modulo);
void enviar_mensajes_por_consola(t_struct_mensajes* mensaje);

#endif