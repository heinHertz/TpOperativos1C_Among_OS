#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum
{
	MENSAJE = 1,
	MENSAJES = 2,
}op_code;

typedef struct
{
	int32_t size;
	char* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
	op_code codigo_operacion;
	int32_t size;
	t_buffer** buffer;
} t_paquete_grande;

typedef struct
{
	int32_t* size;
	char** mensajes;
} t_mensajes;

typedef struct
{
	op_code operacion;
	char* mensaje;
	t_mensajes* mensajes;
	int socket;
	char* identificador_cliente;
} t_result;

typedef struct
{
	int socket;
	void(*f) (t_result*);
} t_parameter;

void iniciar_servidor(char* IP, char* PUERTO, void (*f)(t_result*));
int abrir_socket_servidor(char* IP, char* PUERTO);
void escuchar_socket(int* socket_servidor, void (*f)(t_result*));
void esperar_cliente(int socket_servidor, void (*f)(t_result*));
void serve_client(t_parameter* parametros);
void process_request(int cod_op, t_parameter* parametros);
void escuchar_socket_sin_conexion(int* socket_servidor, void (*f)(t_result*));
int send_messages_and_return_socket(char* identificador, char* ip, char* puerto, char* mensajes[], int cantidadDeMensajes);
int crear_conexion(char *ip, char* puerto, char* identificador);
void enviar_mensajes(int cantidadDeMensajes, char* mensajes[], int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void* serializar_paquete_grande(int size, t_paquete_grande* paquete, int bytes);
void send_message_socket(int socket, char* mensaje);
void liberar_conexion(int socket_cliente);
char* receive_message(int socket_cliente);
t_mensajes* receive_messages(int socket_cliente);
void enviar_mensaje(char* mensaje, int socket_cliente);
void send_messages_socket(int socket, char* mensajes[], int cantidadDeMensajes);
char* recibir_mensaje(int socket_cliente);
int send_message_and_return_socket(char* identificador, char* ip, char* puerto, char* mensaje);
//void handle_client(t_result* result);
char* receive_simple_message(int socket_cliente);

t_mensajes* receive_simple_messages(int socket_cliente);
t_paquete* crear_paquete(void);
void crear_buffer(t_paquete* paquete);

#endif /* CONEXIONES_H_ */