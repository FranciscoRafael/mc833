#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 5400
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {
	
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
        /* criaÃ§Ã£o da estrutura de dados de endereÃ§o */
  	bzero((char *)&socket_address, sizeof(socket_address));


        /* criaÃ§Ã£o de socket passivo */


	/* Associar socket ao descritor */


	/* Criar escuta do socket para aceitar conexÃµes */


        /* aguardar/aceita conexÃ£o, receber e imprimir texto na tela, enviar eco */

	}

}




