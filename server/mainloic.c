#include "./include/myheader.h"
#include "./lib/mylib/include/mylib.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <pthread.h>

#ifndef __STRUCT__
#define __STRUCT__

typedef struct Character {
    SDL_Rect character_rect;
    char *name;
    SDL_Texture *image;
} Character;

typedef struct Client {
	int address;
	struct Character *character;
    struct Client *next;
} Client;

typedef struct Game {
	int start;
	int nb_player;
	Client *list_player;
} Game;

typedef struct program {
	int address;
	Game *game;
} program;

#endif

int my_strcmp(char *stra, char *strb)
{
	int i = 0;
	while ((stra[i] != '\0') && (strb[i] != '\0') && (stra[i] == strb[i])) 
	{
		i++;
	}
	return (stra[i] - strb[i]);
}


int my_strlen(char *str)
{
	int i = 0;
	while(str[i] != 0) {
		i++;
	}
	return i;
}

Client *add_client(int client_address,Client *list_client)
{
	Client *client;
	client = malloc(sizeof(Client*));
	client->address = client_address;
	client->next = list_client;
	return (client);
}

void remove_client(Client **list, int address)
{
	int present = 0;
	Client *old;
	Client **tracer = list;
	while((*tracer) && !(present = ((*tracer)->address == address ))) {
		if ((*tracer)->address == address )
			present = 1;
		else
			tracer = &(*tracer)->next;
	}
	if(present)
	{
		old = *tracer;
		*tracer = (*tracer)->next;
		free(old);
	}
}


Game *creation_game()
{
	Game *game;
	game = malloc(sizeof(Game*));
	game->nb_player = 0;
	game->start = 0;
	game->list_player = NULL;
	return (game);
}

int read_client_input(int client) {
    int n = 0; 
    char buff[255];

    if (client == -1) {
        return -1;
    }
    memset(buff, '\0', 255);
    while( (n = recv(client, buff, 255, 0)) >= 0) {
        if (n == 0) {
            return -1;
        }
        if (send(client, "OK\0", 3 , MSG_NOSIGNAL)) {
	        my_putstr(buff);
	        my_putchar('\n');
        } else {
        	perror("write()");
        }
        if(buff[n - 1] == '\n') {
            memset(buff, '\0', 255);
            break;
        }
        memset(buff, '\0', 255);
    }
    return 0;
}

int recup_input(int address,Game *game)
{
	fd_set readfs;
	FD_ZERO(&readfs);
	FD_SET(address, &readfs);
	
	select(address + 1, &readfs, NULL, NULL, NULL);
	if (FD_ISSET(address, &readfs)) {
        if (read_client_input(address) == -1 ) {
	        puts("client disconnected");
	        remove_client(&game->list_player,address);
	        close(address);
	        address = -1;
	        game->nb_player = game->nb_player -1;
	        printf("%i\n", game->nb_player);
        }
	}
	return (0);
}

void *client_thread(void *client)
{
	program *test = (program *)client;
	recup_input(test->address,test->game);
	return NULL;
}

void displayTest(Client *list_client)
{
	Client *client = list_client;
	while (client != 0) {
		printf("%s\n", "=========================");
		printf("%i\n", client->address);
		printf("%s\n", "=========================");
		client = client->next;
	}
}

int server(int port) 
{
	int sock;
	int new_client;
	int max_player_number = 4;
	socklen_t client_addr_len;
	struct sockaddr_in server;
	struct sockaddr_in client_addr;
	pthread_t client_messages;
	Game *game = creation_game();

	program *thread_arg = malloc(sizeof(program *));
	thread_arg->game = game;
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1)
		return (1);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );

	if (bind(sock,(struct sockaddr *)&server,sizeof(server)) < 0) {
		perror("bind()");
		return (1);
	}
	listen(sock , max_player_number); // int max_player_number ici c'est : The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
	while (game->start == 0) {
		if (game->nb_player < max_player_number) {
			new_client = accept(sock, (struct sockaddr *)&client_addr,&client_addr_len);
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
	free(game->list_player);
	free(game);
	free(thread_arg);
	return (0);
}




int main()
{
	server(2390);
	return (0);
}