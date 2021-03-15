#include "./include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>


// int my_strcmp(char *stra, char *strb)
// {
// 	int i = 0;
// 	while ((stra[i] != '\0') && (strb[i] != '\0') && (stra[i] == strb[i])) 
// 	{
// 		i++;
// 	}
// 	return (stra[i] - strb[i]);
// }


// int my_strlen(char *str)
// {
// 	int i = 0;
// 	while(str[i] != 0) {
// 		i++;
// 	}
// 	return i;
// }

// Client *add_client(int client_address,Client *list_client)
// {
// 	Client *client;
// 	client = malloc(sizeof(Client*));
// 	client->address = client_address;
// 	client->next = list_client;
// 	return (client);
// }

// void remove_client(Client **list, int address)
// {
// 	int present = 0;
// 	Client *old;
// 	Client **tracer = list;
// 	while((*tracer) && !(present = ((*tracer)->address == address ))) {
// 		if ((*tracer)->address == address )
// 			present = 1;
// 		else
// 			tracer = &(*tracer)->next;
// 	}
// 	if(present)
// 	{
// 		old = *tracer;
// 		*tracer = (*tracer)->next;
// 		free(old);
// 	}
// }


// Game *creation_game()
// {
// 	Game *game;
// 	game = malloc(sizeof(Game*));
// 	game->nb_player = 0;
// 	game->start = 0;
// 	game->list_player = NULL;
// 	return (game);
// }

// int read_client_input(int client) {
//     int n = 0; 
//     char buff[255];

//     if (client == -1) {
//         return -1;
//     }
//     memset(buff, '\0', 255);
//     while( (n = recv(client, buff, 255, 0)) >= 0) {
//         if (n == 0) {
//             return -1;
//         }
//         if (send(client, "OK\0", 3 , MSG_NOSIGNAL)) {
// 	        my_putstr(buff);
// 	        my_putchar('\n');
//         } else {
//         	perror("write()");
//         }
//         if(buff[n - 1] == '\n') {
//             memset(buff, '\0', 255);
//             break;
//         }
//         memset(buff, '\0', 255);
//     }
//     return 0;
// }

// int recup_input(int address,Game *game)
// {
// 	fd_set readfs;
// 	FD_ZERO(&readfs);
// 	FD_SET(address, &readfs);
	
// 	select(address + 1, &readfs, NULL, NULL, NULL);
// 	if (FD_ISSET(address, &readfs)) {
//         if (read_client_input(address) == -1 ) {
// 	        puts("client disconnected");
// 	        remove_client(&game->list_player,address);
// 	        close(address);
// 	        address = -1;
// 	        game->nb_player = game->nb_player -1;
// 	        printf("%i\n", game->nb_player);
//         }
// 	}
// 	return (0);
// }

// void *client_thread(void *client)
// {
// 	program *test = (program *)client;
// 	recup_input(test->address,test->game);
// 	return NULL;
// }

// void displayTest(Client *list_client)
// {
// 	Client *client = list_client;
// 	while (client != 0) {
// 		printf("%s\n", "=========================");
// 		printf("%i\n", client->address);
// 		printf("%s\n", "=========================");
// 		client = client->next;
// 	}
// }

void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message, client_message[2000];
    int i = 0;
	
	//Send some messages to the client
	message = "Greetings! I am your connection handler\nNow type something and i shall repeat what you type \n";
	write(sock , message , strlen(message));
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
        while (client_message[i] != '\0') {
            client_message[i] = ' ';
            i++;
        }
        i = 0;
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}

int server(int port) 
{
	int sock;
    int c;
    int client_sock;
    int *new_sock;
	int max_player_number = 4;
	struct sockaddr_in server;
	struct sockaddr_in client;
//	pthread_t client_messages;
//	Game *game = creation_game();

//	program *thread_arg = malloc(sizeof(program *));
//	thread_arg->game = game;

	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1){
		printf("Could not create socket\n");
        return (1);
    }
	printf("Socket created\n");


	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );

	if (bind(sock,(struct sockaddr *)&server,sizeof(server)) < 0) {
		perror("Could not bind Socket\n");
		return (1);
	}
	printf("Bind done\n");

	if(listen(sock , max_player_number) < 0) // int max_player_number ici c'est : The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
	{
        perror("Failed to listen to socket\n");
		return (1);
    }
	printf("Socket Ready to accept connections\n");

    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}

    if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
/*    while (game->start == 0) {
		if (game->nb_player < max_player_number) {
			new_client = accept(sock, (struct sockaddr *)&client,&client_addr_len);
			game->nb_player = game->nb_player + 1;
			game->list_player = add_client(new_client,game->list_player);
			displayTest(game->list_player);
			printf("%i\n", game->nb_player);
			thread_arg->address = new_client;
			
			if(pthread_create(&client_messages, NULL, client_thread, (void *)thread_arg)) {
				my_putstr("echec\n");
			}
		}
	}

	my_putstr("partie lancé");
*/
	/*
	while (1) {
		FD_ZERO(&readfs);

		FD_SET(client, &readfs);

		select(client + 1, &readfs, NULL, NULL, NULL);

		if (FD_ISSET(client, &readfs)) {
			if (recv(client,client_message,100,0) > 0) {
				send(client , "OK" , 2 ,0);
			}
		}
	}*/
//	free(game->list_player);
//	free(game);
//	free(thread_arg);

	return (0);
}




int main()
{
	server(2390);
	return (0);
}