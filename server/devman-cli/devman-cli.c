#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>
#include "devman-cli.h"

#define PORT 8080
#define MAX 50

char buffer[256]; 
char *ok = "OK"; 
char *show = "show";

int main(int argc, char const *argv[]) 
{ 
	info cli[MAX];
	int sfd, valread, i, j, number;
    	struct sockaddr_in svaddr;  
	sfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sfd < 0) {	
        	perror("ERROR socket");
        	exit(EXIT_FAILURE);
	}
    	memset(&svaddr, '0', sizeof(svaddr)); 
    	svaddr.sin_family = AF_INET; 
    	svaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    	svaddr.sin_port = htons(PORT); 
	
    	if (connect(sfd, (struct sockaddr *)&svaddr, sizeof(svaddr)) < 0) { 
        	perror("ERROR connect");
        	exit(EXIT_FAILURE);
    	} 
    	write(sfd, show, strlen(show)); 
    	valread = read(sfd, &number, sizeof(number));
    	number = ntohl(number);
    	for(i=0; i<number; i++) {
    		cli[i].id = Recv_int(sfd, cli[i].id);
    		Recv_char(sfd, cli[i].user);
    		Recv_char(sfd, cli[i].cpu);
    		Recv_char(sfd, cli[i].memory);
    		Recv_char(sfd, cli[i].ip);
    		cli[i].port = Recv_int(sfd, cli[i].port);
    	}
    	
	if (strcmp(argv[1], "show") == 0) {
		if (argc == 2) {
			printf("Socket ID\tDevice Name\tIP Address\tPort\n");
			printf("-----------+-----------------+---------------+-------------\n");
			for (i=0; i<MAX; i++)
			{
				if((strlen(cli[i].user) != 0) && (strlen(cli[i].ip) != 0))
				{
					printf("    %d\t\t%s\t\t%s\t%d\n", cli[i].id, cli[i].user, cli[i].ip, cli[i].port);
				}
			}
		}
		else {
			for (j=2; j<argc; j++) {
				for (i=0; i<MAX; i++) {
					if (strcmp(cli[i].user, argv[j]) == 0) {
						printf("Device name     :         %s\n", cli[i].user);
						printf("Processor       : %s\n", cli[i].cpu);
						printf("Memory          : %s\n", cli[i].memory);
						printf("IP Address      :         %s\n", cli[i].ip);
						printf("Connected port  :         %d\n\n", cli[i].port);
					}
				}	
			}
		}
	}
    	close(sfd);
    	return 0; 
} 

void Recv_char (int sfd, char *data) {
	write(sfd, ok, strlen(ok));
    	read(sfd, data, 255);
}

int Recv_int (int sfd, int ret) {
	write(sfd, ok, strlen(ok));
    	read(sfd, &ret, sizeof(ret));
    	ret = ntohl(ret);
    	return ret;
}
