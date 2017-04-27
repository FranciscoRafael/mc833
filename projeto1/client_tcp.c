#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 5400
#define MAX_LINE 256

int main(int argc, char * argv[]) {
	
	struct hostent *host_address;
  	struct sockaddr_in socket_address;
   	char buf[MAX_LINE];
   	unsigned int len;
   	int socket_fd, new_s;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (socket_fd == -1) { 
		printf("Erro ao abrir socket\n");
		exit(1);
	}
	else if(socket_fd > 0) { 
		printf("Correto\n"); 
	}

		
	host_address = gethostbyname(argv[1]); 

	while(1) { 

  		bzero((char *)&socket_address, sizeof(socket_address));
				

		}

}




