#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 545
#define MAX_LINE 256

int main(int argc, char * argv[]) {
	
	struct hostent *host_address;
  	struct sockaddr_in socket_address;
   	char buf[MAX_LINE], server_msg[MAX_LINE]; 
   	unsigned int len;
   	int socket_fd, new_s;
   	int binds, conn; 

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (socket_fd == -1) { 
		printf("Erro ao abrir socket\n");
		exit(1);
	}
	else if(socket_fd > 0) { 
		printf("Correto\n"); 
	}

	host_address = gethostbyname(argv[1]); 

  	bzero((char *)&socket_address, sizeof(socket_address));
    socket_address.sin_family = AF_INET;

    bcopy((char *) host_address->h_addr, (char *) &socket_address.sin_addr.s_addr, host_address->h_length);
    socket_address.sin_port = htons(LISTEN_PORT);  


    conn = connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));

      while(1)
    {
        printf("Enter message : ");
        scanf("%s" , buf);

		send(socket_fd , buf , strlen(buf), 0);
        recv(socket_fd, server_msg , strlen(buf) , 0);
        puts("Server reply :");
        puts(server_msg);
    }
     
    close(socket_fd);
    return 0;
}


