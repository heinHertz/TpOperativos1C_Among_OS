#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>

#include <stdlib.h>
#include <curses.h>
#include <commons/collections/list.h>

#define ASSERT_CREATE(nivel, id, err)                                                   \
    if(err) {                                                                           \
        nivel_destruir(nivel);                                                          \
        nivel_gui_terminar();                                                           \
        fprintf(stderr, "Error al crear '%c': %s\n", id, nivel_gui_string_error(err));  \
        return EXIT_FAILURE;                                                            \
    }

NIVEL* nivel;
int dibujado_mapa_miramhq(void);
void crearPersonajeEnMapa(char tid,uint32_t posicionX,uint32_t posicionY);
void moverPersonajeEnMapa(char tid,uint32_t posicionX,uint32_t posicionY);
void eliminarPersonajeDelMapa(uint32_t tid);