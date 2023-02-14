//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "Client.hpp"

#define PORT 8888

int opt = true;
int master_socket , addrlen , new_socket , client_socket[30], max_clients = 30 , activity, valread , sd;
int max_sd;
struct sockaddr_in address; //Structure describing an Internet socket address.
Client	clients[30];

char buffer[1025]; 
fd_set readfds;

std::string message = "Welcome to chatSocket\nUse --option: message\n Option:\n\t--nickname \"CustomNickName\"\r\n";

void    notify_all(int sender, String message){
	Client	forSender = clients[sender];
	std::cout << "Client " << sender << " sent a message" << std::endl;
	clients[sender].readMessage(message);
    for (int i = 0; i < max_clients; i++){
        Client forReceiver = clients[i];
        if (forSender.getSocket() != forReceiver.getSocket())
			forReceiver.sendMessage(forSender, message);
    }
}

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
		
	while(true){
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		for (int i = 0 ; i < max_clients ; i++){
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
			if( send(new_socket, message.c_str(), message.length(), 0) != message.length()){
				perror("send");
			}
			puts("Welcome message sent successfully");
			for (int i = 0; i < max_clients; i++){
				if( client_socket[i] == 0 ){
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}
		for (int i = 0; i < max_clients; i++){
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
