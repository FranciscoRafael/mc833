#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <errno.h>
#include <sys/types.h>




#define SERVER_PORT 5845
#define MAX_CLIENTS 50
#define FALSE 0
#define TRUE 1
#define MAX_PENDING 5
#define MAX_LINE 256

int main() { 


    int sockfd, len, new_sockfd, i, act, r, size_table; 
    int k, opt = TRUE, len_info;
    struct sockaddr_in server, info, client; 
    char buf[MAX_LINE]; 
    char tam[INET_ADDRSTRLEN];
    fd_set fds, c_fds; 


    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd == 0) {
        perror("Erro ao criar o socket"); 
        exit(1); 
    }     

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    k = bind(sockfd, (struct sockaddr *)&server, sizeof(server)); 
    if(k < 0) { 
        perror("Erro no binding"); 
        exit(1);           
    }
    k = listen(sockfd, MAX_PENDING);
    if(k < 0) { 
        perror("Erro no listen");
        exit(1); 
    }

    size_table = MAX_CLIENTS; 

    len = sizeof(client); 
    printf("Esperando Conexoes\n"); 
    

    FD_ZERO(&fds); 
    FD_SET(sockfd, &fds); 

    
    while(TRUE) {
        c_fds = fds;
        act = select(size_table, &c_fds, NULL, NULL, NULL); 
        if(act < 0) {
            perror("Erro no select"); 
            exit(1); 
        }

        if(FD_ISSET(sockfd, &c_fds)) {
            new_sockfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&len); 
            if(new_sockfd < 0) {
                perror("Erro ao aceitar clients"); 
                exit(1);
            }
            printf("Conxeao aceita com sucesso\n"); 
            getpeername(new_sockfd, (struct sockaddr *)&info, (socklen_t*)&len_info);
            inet_ntop(AF_INET, &info.sin_addr, tam, sizeof(tam));
            printf("IP - Remoto: %s\nPorta - Remota: %i\n\n", tam, ntohs(info.sin_port));

            if(new_sockfd < 0)
                continue; 

            FD_SET(new_sockfd, &fds); 
            continue;

        }

        for(i = 0; i < size_table; i++) { 
            if(i != sockfd && FD_ISSET(i, &c_fds)) { 
                bzero(buf, MAX_LINE);
                r = read(i, buf, MAX_LINE);
                if(r == 0) {
                    getpeername(i, (struct sockaddr *)&info, (socklen_t*)&len_info);
                    printf("Desconectou, IP %s, Porta %d \n",   inet_ntoa(info.sin_addr) , ntohs(info.sin_port)); 
                    close(i); 
                    FD_CLR(i, &fds);
                }
                else { 
                    getpeername(i, (struct sockaddr *)&info, (socklen_t*)&len_info);
                    printf("Cliente: %s, %i\n", tam, ntohs(info.sin_port));
                    printf("Mensagem: ");
                    for(k = 0; k < strlen(buf); k++) { 
                            printf("%c", buf[k]);   
                    }
                    printf("\n"); 
                    write(i, buf, strlen(buf));
                    printf("Eco enviado\n\n");
                    bzero(buf, MAX_LINE);

                }        

            }

        }


    }

    return 0; 

}