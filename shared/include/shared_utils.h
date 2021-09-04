#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include <stdlib.h>


char* mi_funcion_compartida();


t_log* init_logger(char* path_logger, char* module_name, t_log_level log_level);


#endif