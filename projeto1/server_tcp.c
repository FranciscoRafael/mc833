/*************************** /
/********* Esqueleto 1 *****/
/***************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 5400
#define MAX_LINE 256

int main(int argc, char * argv[]) {
      
	struct hostent *host_address;
 	struct sockaddr_in socket_address, client_address; 
   	char *host;
   	char buffer[MAX_LINE];
   	int len_client, client_acc; 
	int socket_fd, bind_r, rc, w; 


	socket_fd= socket(AF_INET, SOCK_STREAM, 0);
	
	if (socket_fd == -1) { 
		printf("Erro ao abrir socket\n");
		exit(1);
	}
	else if(socket_fd > 0) { 
		printf("Correto\n"); 
	}

	bzero((char *)&socket_address, sizeof(socket_address));

	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(SERVER_PORT); 
	socket_address.sin_addr.s_addr = INADDR_ANY; 

	 
	bind_r = bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));

	if(bind_r == -1) { 
		printf("Erro bind\n"); 
		exit(1);
	}
	else { 
		printf("Bind correto\n"); 
	}
	if(listen(socket_fd, 3)  == -1) { 
		printf("Erro listen\n"); 
		exit(1); 
	}
	else { 
		printf("Correto listen\n"); 
	}

	len_client = sizeof(struct sockaddr_in); 
	


	while (1) { 
		client_acc = accept(socket_fd, (struct sockaddr*)&client_address, (socklen_t *)&client_acc); 

 		if(client_acc == -1) {
			printf("Cliente não aceito\n"); 
			exit(1); 
		}
		else { 
			printf("Cliente aceito\n"); 
		}
		
		bzero(buffer, MAX_LINE);
		
		rc = read(client_acc, buffer, MAX_LINE); 
		
		if(rc == 0) {
			printf("Conexão Terminada\n"); 
			close(client_acc);
		}

		if(rc == -1) { 
			printf("Erro ao receber a mensagem do cliente\n"); 
			exit(1); 
		}
		else { 
			printf("Mensagem: %s\n", buffer);
		
		}
		
		w = write(client_acc, buffer, MAX_LINE); 	
		if(w == -1) {
			printf("Erro de escrita no socket\n"); 
			exit(1); 
		}
			
	}

	close(socket_fd); 
	
	return 0;


}

