#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 6000
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {

    struct sockaddr_in socket_address, client; 
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s, aux = 0, r; 

    bzero((char *)&socket_address, sizeof(socket_address));


    s = socket(AF_INET, SOCK_STREAM, 0);
       
    if(s == -1) { 
        printf("Erro ao criar o socket\n");
        exit(1); 
    }
    else { 
        printf("Socket criado com sucesso\n"); 
    }

    socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(LISTEN_PORT); 	

    aux = bind(s,(struct sockaddr *)&socket_address , sizeof(socket_address));

    if(aux < 0) {
        printf("Erro no binding\n"); 
       		exit(1); 
    }
    else {
       	printf("Bind efetuado com sucesso\n"); 
    }


    aux = listen(s, MAX_PENDING); 
    if(aux < 0) { 
    	printf("Erro no listen\n"); 
		exit(1);		
	}
	else { 
		printf("Listen realizado com sucesso\n");
	}

	printf("Esperando conexao\n");

	len = sizeof(struct sockaddr_in); 

	new_s= accept(s,(struct sockaddr *)&client,(socklen_t*)&len);

	if(new_s < 0) {
		printf("Erro ao aceitar o cliente"); 
		exit(1);
	}
       	
	if(new_s > 0) {
		printf("Conxeao aceita com sucesso\n"); 
	}

	bzero(buf, MAX_LINE);

	while( (r = recv(new_s, buf, MAX_LINE, 0)) > 0) {

		printf("Mensagem recebida: ");
		for(int i = 0; i < strlen(buf); i++) { 
			printf("%c", buf[i]); 	
		}
		printf("\n"); 

		write(new_s, buf, strlen(buf));

		printf("Eco enviado\n\n");

		bzero(buf, MAX_LINE);
	}

	close(s); 
	close(new_s);

}		

