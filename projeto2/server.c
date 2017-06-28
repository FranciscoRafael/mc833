#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>

#define SERVER_PORT 6000
#define MAX_CLIENTS 50
#define FALSE 0
#define TRUE 1
#define MAX_PENDING 5
#define MAX_LINE 256
#define UDP_PORT 7800
#define MSG_UDP_TAM 256


typedef struct Client { 

    char tipo; 
    char ID; 
    int pos_inicial; 
    char direcao; 
    int tamanho; 
    int velocidade; 

}Carro;

int existe_carro_oposto(Carro *carro, int n, char D); 
int existe_carro(Carro *carro, char ID, int n); 
int re_index(Carro *carro, char ID, int n); 
void imprime(Carro* carro, int n);  
int calc_colisoes(Carro *carro, int n, char ID); 
void chamada_udp(); 

int n = 50; 
Carro carro[50]; 
int count_car = 0; 

int main() { 


    int sockfd, len, new_sockfd, i, act, r, size_table; 
    int k, opt = TRUE, len_info, q; 
    struct sockaddr_in server, info, client; 
    char buf[MAX_LINE]; 
    char tam[INET_ADDRSTRLEN];
    int resp; 
    fd_set fds, c_fds; 
    int count = 0; 

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

					if(existe_carro(carro, buf[1], count_car) == 0) {                     
				    	if(buf[0] == 'S') { 
					    	carro[count_car].tipo = buf[0]; 
					    	carro[count_car].ID = buf[1];
                            carro[count_car].pos_inicial = (buf[2] - '0');
					    	carro[count_car].direcao = buf[3];
					    	carro[count_car].tamanho = (buf[4] - '0');
					    	carro[count_car].velocidade = (buf[5] - '0'); 	
					    	count_car++; 
				    	}
	            	}
                    else { 

                        q = re_index(carro, buf[1], count_car);  
                        carro[q].pos_inicial = (buf[2] - '0'); 
                        carro[q].velocidade = (buf[5] - '0'); 

                    }
                    int pos = (buf[2] - '0');
                    if(buf[0] == 'S' && pos < 9) {
                        resp = calc_colisoes(carro, count_car, buf[1]); 
                        if(resp == 1) {
                            bzero(buf, MAX_LINE); 
                            strcpy(buf, "Acelere");
                        }
                        else if(resp == 0) {
                            bzero(buf, MAX_LINE); 
                            strcpy(buf, "Freie"); 
                        }
                        else if(resp == 2) {
                            bzero(buf, MAX_LINE); 
                            strcpy(buf, "Continue"); 
                        }
                        else if(resp == 3) { 
                            bzero(buf, MAX_LINE); 
                            strcpy(buf, "Ambulancia"); 
                        }
                    }
                    else if(pos > 9) {
                        bzero(buf, MAX_LINE); 
                        strcpy(buf, "Fim"); 
                    }

                    printf("\n"); 
					imprime(carro, count_car);
                    write(i, buf, strlen(buf));
                    printf("Mensagem Enviada\n\n");
                    bzero(buf, MAX_LINE);
                    if(count == 0) {
                        //chamada_udp();
                        count++; 
                    }
                    

                }        

            }

        }


    }

    return 0; 

}

void chamada_udp()  {

    int udpSocket, nBytes;
    char msg_UDP[MSG_UDP_TAM];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int i;

    bzero(msg_UDP, MSG_UDP_TAM); 
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    addr_size = sizeof serverStorage;
    nBytes = recvfrom(udpSocket,msg_UDP,MSG_UDP_TAM,0,(struct sockaddr *)&serverStorage, &addr_size);
    if(msg_UDP[0] == 'E') {
        bzero(msg_UDP, MSG_UDP_TAM); 
        strcpy(msg_UDP, "Nova musica: Radiohead: There There "); 
    } 
    else if(msg_UDP[0] == 'C') {
        bzero(msg_UDP, MSG_UDP_TAM); 
        strcpy(msg_UDP, "Ar Condicionado Ligado");
    }
    sendto(udpSocket,msg_UDP,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
    bzero(msg_UDP, MSG_UDP_TAM);

}

int calc_colisoes(Carro *carro, int n, char ID) {

    int j = re_index(carro, ID, n); 
    double temp_carro; 
    double temp_tam_carro;  
    int dist_origem = abs(4 - carro[j].pos_inicial); 
    int i; 
    char op_d;
    double ntemp_carro;
    double ntempo_tam_carro; 
    int ndist_origem; 
    int ac = 2; 
    double temp_des = (double) carro[j].velocidade/ac;

    if(carro[j].direcao == 'X')
        op_d = 'Y'; 
    else if(carro[j].direcao == 'Y')
        op_d = 'X';


    if(carro[j].velocidade > 0) { 


        temp_carro = (double)( dist_origem/carro[j].velocidade);
        temp_tam_carro = (double)((carro[j].tamanho + dist_origem)/ carro[j].velocidade); 
    }

    if(existe_carro_oposto(carro, n, op_d)) {
    
        for(i = 0; i < n; i++) {
            if(carro[i].direcao == op_d && (carro[i].velocidade > 0)) {
                ndist_origem = abs(4- carro[i].pos_inicial);
                ntemp_carro = (double)(dist_origem/carro[i].velocidade); 
                ntempo_tam_carro = (double)((carro[i].tamanho + ndist_origem)/carro[i].velocidade);
                if(temp_carro <= ntempo_tam_carro && temp_carro >= ntemp_carro) { 
                    if(temp_des >= ntemp_carro) { 
                        return 3;
                    }
                    else if (temp_des < ntemp_carro) {
                        return 0; 
                    }
                }
                else if(ntemp_carro <= temp_tam_carro && ntemp_carro >= temp_carro) { 
                    if(temp_des >= ntemp_carro) { 
                        return 3;
                    }
                    else if (temp_des < ntemp_carro) {
                        return 0; 
                    } 
                }
                else { 
                    return 2; 
                }
           }
        }
    }
    else if(existe_carro_oposto(carro, n, op_d) == 0) {

        return 1; 
    }
    return 2;

}

int existe_carro_oposto(Carro *carro, int n, char D) {
    int i; 

    for(i = 0; i < n; i++) {
        if(carro[i].direcao == D) {
            return 1; 
        }
    }
    return 0; 
}
void imprime(Carro* carro, int n) { 

	for(int i = 0; i < n; i++) {
		printf("ID: %c\n", carro[i].ID);
        printf("posicao: %d\n", carro[i].pos_inicial); 
		printf("direcao: %c\n", carro[i].direcao);
		printf("Tamanho: %d\n", carro[i].tamanho);
		printf("Velocidade: %d k/h\n", carro[i].velocidade);
		printf("\n"); 
	}
    printf("%d\n", n); 
	printf("--------------------------------------------------\n"); 
}


int existe_carro(Carro *carro, char ID, int n) {
	int i;

	for (i = 0; i < n; i++) { 
	    if(carro[i].ID == ID) { 
	        return 1; 
        }
    }
    
    return 0; 
}

int re_index(Carro *carro, char ID, int n) {

    int i; 
    for(i =0; i < n; i++) { 
        if(carro[i].ID == ID) {
            return i;
        }
    }

    return 0; 
}
