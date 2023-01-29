//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "Client.hpp"
	
#define TRUE 1
#define FALSE 0
#define PORT 8888

int opt = TRUE;
int master_socket , addrlen , new_socket , client_socket[30], max_clients = 30 , activity, i , valread , sd;
int max_sd;
struct sockaddr_in address;
Client	clients[30];

    
char buffer[1025]; 
fd_set readfds;
    
char *message = "Welcome to chatSocket\nUse --option: message\n Option:\n\t--Loggin: Your \"NickName\"\r\n";

void    notify_all(int sender, String message){
	Client	forSender = clients[sender];
	std::cout << sender << std::endl;
	clients[sender].readMessage(message);
    for (int i = 0; i < max_clients; i++){
        Client forReceiver = clients[i];
        if (forSender.getSocket() != forReceiver.getSocket())
			forReceiver.sendMessage(forSender, message);
    }
}
auto lambda1 = [](String nickname) { std::cout << nickname << std::endl; };
int main(int argc , char *argv[])
{		
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	if (listen(master_socket, 3) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
		
	while(TRUE){
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		for ( i = 0 ; i < max_clients ; i++){
			sd = client_socket[i];
				
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			if(sd > max_sd)
				max_sd = sd;
		}

		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
		if ((activity < 0) && (errno!=EINTR)){
			printf("select error");
		}

		if (FD_ISSET(master_socket, &readfds)){
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			clients[new_socket].init(new_socket);
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
			if( send(new_socket, message, strlen(message), 0) != strlen(message) ){
				perror("send");
			}
			puts("Welcome message sent successfully");
			for (i = 0; i < max_clients; i++){
				if( client_socket[i] == 0 ){
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);	
					break;
				}
			}
		}
		for (i = 0; i < max_clients; i++){
			sd = client_socket[i];
			
			if (FD_ISSET( sd , &readfds)){
				if ((valread = read( sd , buffer, 1024)) == 0){
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
					close( sd );
					client_socket[i] = 0;
				} else{
					//if (String(buffer).find("--Loggin:"))
					buffer[valread] = '\0';
                    notify_all(sd, buffer);
				}
			}
		}
	}
		
	return 0;
}
