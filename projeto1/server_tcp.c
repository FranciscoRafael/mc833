************************** 
******** Esqueleto 1 *****
************************** 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 12345
#define MAX_LINE 256

int main(int argc, char * argv[]) {
        struct hostent *host_address;
        struct sockaddr_in socket_address;
        char *host;
        char buf[MAX_LINE];
        int s;
        int len;


	/* verificaÃ§Ã£o de argumentos */
	(...)

	/* traduÃ§Ã£o de nome para endereÃ§o IP */
	(...)

        /* criaÃ§Ã£o da estrutura de dados de endereÃ§o */
        bzero((char *)&socket_address, sizeof(socket_address));
	(...)

        /* criaÃ§Ã£o de socket ativo*/
	(...)

	/* estabelecimento da conexÃ£o */
	(...)

        /* ler e enviar linhas de texto, receber eco */
	(...)

}

