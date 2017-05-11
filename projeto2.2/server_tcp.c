#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_PORT 6500
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {

    struct sockaddr_in socket_address, client, info;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s, aux = 0, r, len_info; 
    char tam[INET_ADDRSTRLEN];
	fd_set todos_descritores, desc_aux; 
	int maxfd, n_cliente; 
	int cliente[FD_SETSIZE], rd, i, sockfd; 


    bzero((char *)&socket_address, sizeof(socket_address));

    // criacao de um socket do servidor 
    s = socket(AF_INET, SOCK_STREAM, 0);
       

   	// verifica se o socket foi criado corretamente 
    if(s == -1) { 
        printf("Erro ao criar o socket\n");
        exit(1); 
    }
    else { 
        printf("Socket criado com sucesso\n"); 
    }

    //inicializacao das estruturas de dados do sockaddr do servidor 
    socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(LISTEN_PORT); 	


    // processo de bind, verifica se pode associar o socket a porta
    aux = bind(s,(struct sockaddr *)&socket_address , sizeof(socket_address));


    if(aux < 0) {
        printf("Erro no binding\n"); 
       		exit(1); 
    }
    else {
       	printf("Bind efetuado com sucesso\n"); 
    }

    // habilita o socket para receber conexoes
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
	len_info = sizeof(struct sockaddr_in);
	maxfd = s; 	
	n_cliente = -1; 
	for(i = 0; i < FD_SETSIZE; i++) {
		cliente[i] = -1; 
	}

	FD_ZERO(&todos_descritores); 
	FD_SET(s, &todos_descritores); 

	while (1) { 
		desc_aux = todos_descritores; 
		r = select(maxfd+1, &desc_aux, NULL, NULL, NULL); 
		if(r < 0) {
			printf("Erro no select\n");
			exit(1);	 
		}
		if(FD_ISSET(s, &desc_aux)) {
			new_s = accept(s, (struct sockaddr *)&socket_address, &len); 
			if(new_s < 0) { 
				printf("Erro ao aceitar o cliente\n"); 
				exit(1);
			}
			for(i = 0; i < FD_SETSIZE; i++) {
				if(cliente[i] < 0) { 
					cliente[i] = new_s; 
					break;
				}
			}
			if(i == FD_SETSIZE) { 
				printf("Limite máximo de clientes atingidos\n");
				exit(1);
			}
			FD_SET(new_s, &todos_descritores); 
			if(new_s < maxfd) 
				maxfd = new_s;
			if(i > n_cliente)		
				n_cliente = i;
			if(--r <= 0)
				continue;	
											
		}
		for(i = 0; i <= n_cliente; i++) {
			sockfd = cliente[i]; 
			if(sockfd < 0) 
				continue; 
			if(FD_ISSET(sockfd, &desc_aux)) {
				bzero(buf, MAX_LINE); 
				rd = recv(sockfd, buf, sizeof(buf), 0); 
				if(rd == 0) {
					close(sockfd);
					FD_CLR(sockfd, &todos_descritores); 
					cliente[i] = -1; 
				}
				else { 
					getpeername(new_s, (struct sockaddr *)&info, (socklen_t*)&len_info);
					inet_ntop(AF_INET, &info.sin_addr, tam, sizeof(tam));
					printf("IP - Remoto: %s\nPorta - Remota: %i\n\n", tam, ntohs(info.sin_port));
					printf("Cliente: %s, %i\n", tam, ntohs(info.sin_port));
					printf("Mensagem: ");
					for(int i = 0; i < strlen(buf); i++) { 
						printf("%c", buf[i]); 	
					}
					printf("\n"); 
					write(new_s, buf, strlen(buf));
					printf("Eco enviado\n\n");
					bzero(buf, MAX_LINE);
				}
				if(--r <= 0) 
					break; 
			}
		}								
	}	
}		

