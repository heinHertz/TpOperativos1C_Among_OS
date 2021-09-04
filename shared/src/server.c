#include "server.h"


void iniciar_servidor(char* IP, char* PUERTO, void (*f)(t_result*))
{
	int socket_servidor = abrir_socket_servidor(IP, PUERTO);

	escuchar_socket(&socket_servidor, f);

}

int abrir_socket_servidor(char* IP, char* PUERTO) {

	printf("Abriendo socket en %s:%s\n", IP, PUERTO );

	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next)
	{
		if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		int activado = 1;
		//int desactivado = 0;
		setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
		//setsockopt(socket_servidor, SOL_SOCKET, SOCK_NONBLOCK, &desactivado, sizeof(desactivado));

		if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_servidor);
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo);

	printf("Abierto servidor en %s:%s con el numero de socket %d\n", IP, PUERTO, socket_servidor);

	return socket_servidor;
}

void escuchar_socket(int* socket_servidor, void (*f)(t_result*)){

	printf("Estoy escuchando el socket %d:\n", *socket_servidor);

	int respuesta = listen(*socket_servidor, SOMAXCONN);

	if(respuesta == -1){
		//perror("ERROR DE ACCEPT:");
		printf("ERROR DE LISTEN: %s | NUMERO DE ERRNO: %d\n", strerror(errno), errno);
		printf("Listen falló estrepitosamente\n");
		return;
	}	

	while(1)
		esperar_cliente(*socket_servidor, f);
}

void esperar_cliente(int socket_servidor, void (*f)(t_result*))
{
	pthread_t thread;

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, (void*) &tam_direccion);

	if (socket_cliente == -1){
		//perror("ERROR DE ACCEPT:");
		printf("ERROR DE ACCEPT: %s | NUMERO DE ERRNO: %d\n", strerror(errno), errno);
		return;
	}

	t_parameter* parametro = malloc(sizeof(t_parameter));

	parametro->socket = socket_cliente;
	parametro->f = f;

	pthread_create(&thread,NULL,(void*)serve_client, parametro);
	pthread_detach(thread);

}

void serve_client(t_parameter* parametros)
{
	int cod_op;
	if(recv(parametros->socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, parametros);
}

void process_request(int cod_op, t_parameter* parametros) {

	t_result* resultado = malloc(sizeof(t_result));
	resultado->identificador_cliente = receive_message(parametros->socket);
	recv(parametros->socket, &cod_op, sizeof(int), MSG_WAITALL);
		switch (cod_op) {
		case MENSAJE:
			resultado->operacion = cod_op;
			resultado->mensaje = receive_message(parametros->socket);
			resultado->socket = parametros->socket;
			parametros->f(resultado);
			//liberar_conexion(resultado->socket);
			free(resultado->mensaje);
			free(resultado);

			break;
		case MENSAJES:
			resultado->operacion = cod_op;
			resultado->mensajes = receive_messages(parametros->socket);
			resultado->socket = parametros->socket;
			parametros->f(resultado);
			for(int i = 0; i < *resultado->mensajes->size; i++){
				free(resultado->mensajes->mensajes[i]);
			}
			free(resultado->mensajes->size);
			free(resultado->mensajes);
			free(resultado);

			break;
		case 0:
			liberar_conexion(parametros->socket);
			free(parametros);
			free(resultado);
			pthread_exit(NULL);
			break;
		case -1:
			liberar_conexion(parametros->socket);
			free(parametros);
			free(resultado);
			pthread_exit(NULL);
			break;
		default:
			liberar_conexion(parametros->socket);
			free(parametros);
			free(resultado);
			pthread_exit(NULL);
			break;

		}

}

void escuchar_socket_sin_conexion(int* socket_servidor, void (*f)(t_result*)){

	printf("Estoy escuchando el socket %d:\n", *socket_servidor);

	t_parameter* parametro = malloc(sizeof(t_parameter));
	parametro->socket = *socket_servidor;
	parametro->f = f;

	while(1)
		serve_client(parametro);
}

int send_message_and_return_socket(char* identificador, char* ip, char* puerto, char* mensaje) {

	int conexion;

	conexion = crear_conexion(ip, puerto, identificador);

	if (conexion == -1){
		return conexion;
	}

	send_message_socket(conexion, mensaje);

	return conexion;
}

int send_messages_and_return_socket(char* identificador, char* ip, char* puerto,
                                    char* mensajes[], int cantidadDeMensajes) {

	int conexion;

	conexion = crear_conexion(ip, puerto, identificador);

	if (conexion == -1){
		return conexion;
	}

	enviar_mensajes(cantidadDeMensajes, mensajes, conexion);

	return conexion;
}

int crear_conexion(char *ip, char* puerto, char* identificador)
{
	printf("Creando conexion con %s:%s \n", ip, puerto);
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if (socket_cliente == -1) {
		printf("Error al crear el socket del cliente %s:%s \n", ip, puerto);
		
		return socket_cliente;

	} else {
		if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
			liberar_conexion(socket_cliente);
			socket_cliente = -1;
			printf("Error al conectarse al cliente %s:%s \n", ip, puerto);

			return socket_cliente;
			
		} else {
			printf("Conexion establecida con %s:%s en el socket %d \n", ip, puerto, socket_cliente);
		}

	}
	
	freeaddrinfo(server_info);

	enviar_mensaje(identificador, socket_cliente);

	return socket_cliente;
}


void enviar_mensajes(int cantidadDeMensajes, char* mensajes[], int socket_cliente)
{

	printf("Enviando el mensaje: ");
	for (int i = 0; i<cantidadDeMensajes; i++){
		printf("%s ", mensajes[i]);
	}printf("\n");


	t_paquete_grande* paquete = malloc(sizeof(t_paquete_grande));
	int bytes = 0;
	paquete->codigo_operacion = MENSAJES;
	paquete->size = cantidadDeMensajes;
	bytes += (2 + paquete->size)*sizeof(int32_t);
	paquete->buffer = malloc(sizeof(t_buffer) * paquete->size);
	for (int i = 0; i < paquete->size; i++){
		paquete->buffer[i] = malloc(sizeof(t_buffer));
		paquete->buffer[i]->size = strlen(mensajes[i]) + 1;
		paquete->buffer[i]->stream = malloc(paquete->buffer[i]->size);
		memcpy(paquete->buffer[i]->stream, mensajes[i], paquete->buffer[i]->size);
		bytes += paquete->buffer[i]->size;
	}


	void* a_enviar = serializar_paquete_grande(cantidadDeMensajes,  paquete, bytes);


	int respuesta = send(socket_cliente, a_enviar, bytes, 0);

	if (respuesta == -1){
		printf("Error Al enviar el mensaje\n");
	}

	for (int i = 0; i < paquete->size; i++){
		free(paquete->buffer[i]->stream);
		free(paquete->buffer[i]);
	}

	free(a_enviar);
	free(paquete->buffer);
	free(paquete);
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{

	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void* serializar_paquete_grande(int size, t_paquete_grande* paquete, int bytes)
{

	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, &(paquete->size), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	for (int i = 0; i < size; i++){
		memcpy(magic + desplazamiento, &(paquete->buffer[i]->size), sizeof(int32_t));
		desplazamiento+= sizeof(int32_t);
		memcpy(magic + desplazamiento, paquete->buffer[i]->stream, paquete->buffer[i]->size);
		desplazamiento+= paquete->buffer[i]->size;
	}


	return magic;
}

void send_message_socket(int socket, char* mensaje) {

	enviar_mensaje(mensaje, socket);

}

char* receive_message(int socket_cliente)
{
	char* buffer;
	int* size = malloc(sizeof(int));

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);
	free(size);

	return buffer;
}

char* receive_simple_message(int socket_cliente){
	int cod_op;
	char* buffer = NULL;
	int size;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1){
		return buffer;
	}
	recv(socket_cliente, &size, sizeof(int), MSG_WAITALL);
	buffer = malloc(sizeof(char)*size);
	recv(socket_cliente, buffer, size, MSG_WAITALL);
	return buffer;
}


t_mensajes* receive_messages(int socket_cliente)
{
	int* size = malloc(sizeof(int));
	t_mensajes* mensajes;
	mensajes = malloc(sizeof(t_mensajes));
	mensajes->size = malloc(sizeof(int));
	recv(socket_cliente, mensajes->size, sizeof(int), MSG_WAITALL);
	mensajes->mensajes = malloc(sizeof(char*) * *mensajes->size);
	for(int i = 0; i < *mensajes->size; i++){
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		mensajes->mensajes[i] = malloc(*size);
		recv(socket_cliente, mensajes->mensajes[i], *size, MSG_WAITALL);
	}
	free(size);
	return mensajes;
}

t_mensajes* receive_simple_messages(int socket_cliente){
	int size;
	int cod_op;
	t_mensajes* mensajes = NULL;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1){
		return mensajes;
	}
	mensajes = malloc(sizeof(t_mensajes));
	mensajes->size = malloc(sizeof(int));
	recv(socket_cliente, mensajes->size, sizeof(int), MSG_WAITALL);
	mensajes->mensajes = calloc((*mensajes->size), sizeof(char*));
	for(int i = 0; i < *mensajes->size; i++){
		recv(socket_cliente, &size, sizeof(int), MSG_WAITALL);
		mensajes->mensajes[i] = calloc(size, sizeof(char));
		recv(socket_cliente, mensajes->mensajes[i], size, MSG_WAITALL);
	}

	return mensajes;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
};

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	printf("Enviando el mensaje %s al socket %d \n", mensaje, socket_cliente);
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int32_t);

	void* a_enviar = serializar_paquete(paquete, bytes);

	int respuesta = send(socket_cliente, a_enviar, bytes, 0);

	if (respuesta == -1){
		printf("Error Al enviar el mensaje\n");
	}

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void send_messages_socket(int socket, char* mensajes[], int cantidadDeMensajes) {

    enviar_mensajes(cantidadDeMensajes, mensajes, socket);

}

char* recibir_mensaje(int socket_cliente)
{
   void * buffer;
   int* size = malloc(sizeof(int32_t));
   int* codigoOperacion = malloc(sizeof(int32_t));

   recv(socket_cliente, codigoOperacion, sizeof(int32_t), MSG_WAITALL);

   recv(socket_cliente, size, sizeof(int32_t), MSG_WAITALL);
   buffer = malloc(*size);
   recv(socket_cliente, buffer, *size, MSG_WAITALL);

   free(size);
   free(codigoOperacion);

   return buffer;
}