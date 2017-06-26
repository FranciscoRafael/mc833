#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

#define LISTEN_PORT 6000
#define MAX_LINE 256


int main(int argc, char * argv[]) {
    
    struct hostent *host_address;
    struct sockaddr_in socket_address, info; 
    char buf[MAX_LINE], server_msg[MAX_LINE]; 
    int socket_fd; 
    int conn, len_info;
    char tam[INET_ADDRSTRLEN];


    FILE *fp;
    char *l, aux;  

    fp = fopen("car1.txt", "r"); 
    int z = 0;  
    while(fscanf(fp, "%c", &aux) != EOF) { 
        z++; 
    }

    l = malloc(z*sizeof(char)); 
    z = 0; 
    rewind(fp);
    while(fscanf(fp, "%c", &aux) != EOF) {
        l[z] = aux; 
        z++; 
    } 

    printf("%c\n", l[0]); 
    printf("%c\n", l[2]); 
    printf("%c\n", l[4]); 
    printf("%c\n", l[6]); 
    printf("%c\n", l[8]); 


    fclose(fp); 
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



    len_info = sizeof(struct sockaddr_in);
    // tenta a conexao o servidor. 
    conn = connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if(conn == -1) {
        printf("Nao foi possivel conectar ao servidor\n");
        exit(1);

    }
    else { 
        printf("Conexao realizada com sucesso\n"); 
        getsockname(conn, (struct sockaddr *)&info, (socklen_t*)&len_info);
        inet_ntop(AF_INET, &info.sin_addr, tam, sizeof(tam));
        printf("IP - Local: %s\nPorta - Local: %i\n\n", tam, ntohs(info.sin_port));
    }


    // loop infinito no qual as mensagens sao enviadas e recebidas do servidor
    while(1) {
        printf("Enter message : ");
        gets(buf);

        send(socket_fd , l , strlen(l), 0);
        recv(socket_fd, server_msg , MAX_LINE , 0);
        printf("Server eco: ");
        for(int i = 0; i < strlen(l); i++) {
            printf("%c", l[i]);
        }
        printf("\n\n");

    }
     
    close(socket_fd);
    return 0;
}