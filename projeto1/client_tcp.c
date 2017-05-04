#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

#define LISTEN_PORT 5400
#define MAX_LINE 256

int main(int argc, char * argv[]) {
	
	struct hostent *host_address;
  	struct sockaddr_in socket_address;
   	char buf[MAX_LINE], server_msg[MAX_LINE]; 
   	int socket_fd; 
   	int conn; 


   	// criação de um Socket do cliente
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(argc <= 1)  {
		printf("Não foi possível obter o hostname\n"); 
		exit(1);
	}

	// verificacao se o socket foi possivel
	if (socket_fd == -1) { 
		printf("Erro ao abrir socket\n");
		exit(1);
	}
	else if(socket_fd > 0) { 
		printf("Correto\n"); 
	}

	// e pego nome do servidor local - no caso o hostname
	host_address = gethostbyname(argv[1]); 


	// a estrutura de dados do sockaddr é inicializada com os valorees, da porta, endereço e familia
  	bzero((char *)&socket_address, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    bcopy((char *) host_address->h_addr, (char *) &socket_address.sin_addr.s_addr, host_address->h_length);
    socket_address.sin_port = htons(LISTEN_PORT);  


    // tenta a conexao o servidor. 
    conn = connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));

    if(conn == -1) {
    	printf("Nao foi possivel conectar ao servidor\n");
    	exit(1);

    }
    else { 
    	printf("Conexao realizada com sucesso\n"); 
    }


    // loop infinito no qual as mensagens sao enviadas e recebidas do servidor
   	while(1) {
    	printf("Enter message : ");
        gets(buf);

		send(socket_fd , buf , strlen(buf), 0);
        recv(socket_fd, server_msg , MAX_LINE , 0);
       	printf("Server eco: ");
       	for(int i = 0; i < strlen(buf); i++) {
       		printf("%c", buf[i]);
       	}
       	printf("\n\n");
       	bzero(buf, MAX_LINE);

    }
     
    close(socket_fd);
    return 0;
}


