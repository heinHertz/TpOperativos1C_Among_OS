#include "shared_utils.h"


char* mi_funcion_compartida(){
    return "Hice uso de la shared!";
}

t_log* init_logger(char* path_logger, char* module_name, t_log_level log_level) {
    t_log* logger = log_create(path_logger, module_name, true, log_level);

    if (logger == NULL) {
        printf("\nERROR: the logger was not created\n");
        exit(1);
    }

return logger;
}