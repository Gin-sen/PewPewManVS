#include <stdlib.h>	//atoi
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc , char *argv[])
{
    (void) argc;
	int sock;
	int port = atoi(argv[1]);
    struct sockaddr_in server;
	char message[1000] , server_reply[2000];
    int i = 0;
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
        close(sock);
        return (0);
    }
    puts("Server says :");
    puts(server_reply);
	//keep communicating with server
	while(1)
	{
        while (server_reply[i] != '\0') {
            server_reply[i] = ' ';
            i++;
        }
        i = 0;
		printf("Enter message : ");
		scanf("%s" , message);
		
		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		puts("Server reply :");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}