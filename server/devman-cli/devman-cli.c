#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUF_SIZE 1024
#define SIZE 1024

typedef struct inform 
{
   char  cpu[128];
   char  memory[128];
   char  user[128];
}clientInfo;

char buffer[1024];
void error_handling(char *message);


  

int main(int argc, char *argv[])
{	

	int n=0;
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
	

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		printf("Connected...........\n");
	   
	    clientInfo *client;
            client = (clientInfo*) calloc(1, sizeof(clientInfo));
           str_len=read(sock, message, BUF_SIZE-1);
	    message[str_len]=0;
            printf("Message from server: %s", message); 
            printf ("Helllo\n");
	    
	    printf("cpu: %s\n", client->cpu);
	    
	    printf("User: %s\n", client->user);

	     printf("memory: %s\n", client->memory);


	close(sock);
	free(client);
	return 0;
	
}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
