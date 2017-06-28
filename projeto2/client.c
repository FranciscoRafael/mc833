#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/time.h>

#define LISTEN_PORT 6000
#define MAX_LINE 256
#define UDP_PORT 7800
#define MAX_TAM_UDP 50

void client_udp (char *l, int argc, char *argv[]) {

    int clientUDPSocket, nBytes;
    char buf[MAX_TAM_UDP];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    clientUDPSocket = socket(PF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    addr_size = sizeof serverAddr;
    sendto(clientUDPSocket,l,strlen(l) + 1,0,(struct sockaddr *)&serverAddr,addr_size);
    recvfrom(clientUDPSocket,buf,MAX_TAM_UDP,0,NULL, NULL);
    printf("Mensagem do Servidor: %s\n",buf);

}
void client_tcp (char *l, int argc, char * argv[]) { 

    struct hostent *host_address;
    struct sockaddr_in socket_address, info; 
    char buf[MAX_LINE], server_msg[MAX_LINE]; 
    int socket_fd; 
    int conn, len_info;
    char tam[INET_ADDRSTRLEN];
    struct timeval tv1, tv2, diff; 
    long delta, total = 0; 

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

        gettimeofday(&tv1, NULL);
    
        send(socket_fd , l , strlen(l), 0);
        recv(socket_fd, server_msg , MAX_LINE , 0);

        gettimeofday(&tv2, NULL);
        
        diff.tv_sec = tv2.tv_sec - tv1.tv_sec;
        diff.tv_usec = tv2.tv_usec + (1000000 - tv1.tv_usec);

        while(diff.tv_usec > 1000000) {
            diff.tv_sec++;
            diff.tv_usec -= 1000000;
        }
        delta = diff.tv_usec; 
        char buffer[20]; 
        sprintf(buffer, "%ld", delta); 

        printf("Atraso do Servidor %ld \n", delta); 
        total += delta; 
        printf("Total de Atraso do Servidor %ld\n", total);
        printf("Mensagem do Servidor: ");

        for(int i = 0; i < strlen(server_msg); i++) {
            printf("%c", server_msg[i]);
        }

        int mul = 1; 
        if(strcmp(server_msg, "Acelere") == 0) { 
            mul = 2; 
        }
        else if(strcmp(server_msg, "Freie") == 0) {
            l[5] = '0';

        }
        else if(strcmp(server_msg, "Continue") == 0) {
            mul = 1; 

        }
        else if(strcmp(server_msg, "Fim") == 0) {
            sleep(1000000); 
        }
        printf("\n\n");

        int vel = (l[5] - '0'); 
        int pos = (l[2] - '0'); 
        pos = pos + vel*mul;
        l[2] = pos + '0';
        bzero(server_msg, MAX_LINE); 

        sleep(3); 
           

    }
     
    close(socket_fd);



} 
int main(int argc, char * argv[]) {
    
    struct hostent *host_address;
    struct sockaddr_in socket_address, info; 
    char buf[MAX_LINE], server_msg[MAX_LINE]; 
    int socket_fd; 
    int conn, len_info;
    char tam[INET_ADDRSTRLEN];
	struct timeval tv1, tv2, diff; 
	long delta, total = 0; 
    char *l; 
    char c; 
    int tam_l;

    tam_l = argc - 2;
    l = calloc(tam_l, sizeof(char)); 
    l[0] = argv[2][0];
    l[1] = argv[3][0];
    l[2] = argv[4][0];
    l[3] = argv[5][0];
    l[4] = argv[6][0];
    l[5] = argv[7][0];

	// Tipo, ID, POSICAO INICIAL, DIRECAO (0 - X, 1 - Y), TAMANHO, VELOCIDADE

    if(l[0] == 'S') {
        client_tcp(l, argc, argv);
    }
    else if(l[0] == 'E' || l[0] == 'C') {
        client_udp(l, argc, argv); 
    }

    
}
