#ifndef IMONGO_H
#define IMONGO_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <stdbool.h>
#include <string.h>
#include <commons/string.h>
#include "shared_utils.h"
#include "imongo_config.h"
#include "imongo_mensajes.h"
#include "filesystem.h"
#include "recursos.h"

#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>
#include <commons/string.h>
#include <pthread.h>
#include <unistd.h>



void tareaServer(void);

void tareaSincronizar(void);

t_log* iniciar_logger(void);


void procesar_signal(int signal);


#endif