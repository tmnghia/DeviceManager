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
int  getMemory(char *mem);
int  getCPU(char *cpu);
int  getUser(char *user);

  

int main(int argc, char *argv[])
{	

	
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
	   
	    clientInfo *client = NULL;

            client = (clientInfo*) calloc(1, sizeof(clientInfo));
	    getCPU(client->cpu);
	    printf("cpu: %s\n", client->cpu);
	    
	    getUser(client->user);
	    printf("User: %s\n", client->user);

	    getMemory(client->memory);
	    printf("memory: %s\n", client->memory);

	    
	    

	 while(1)
	 {	 
	       
               send (sock,client, sizeof(struct inform),0);
               //printf ("Inform sent to Server\n");          
        } 
	close(sock);
	free(client);
	return 0;
	
}


int  getCPU(char *cpu)
{	
	char buffer[1024]  = {0};
   	char *op        = NULL;
	FILE *file	= NULL;      
    	int rc = 0; 
    	
    	file = popen("lshw -c cpu 2>null | grep size", "r");
    	 op = fgets(buffer, sizeof(buffer), file);
    	if (op == NULL) {
        printf("failed");
    	}
    	sprintf(cpu, "%s", buffer);
    	
    	pclose(file);
    	return 0;
}



int  getMemory(char *mem)
{      

	char buffer[1024]  = {0};
    	char *op        = NULL;
    	FILE *file      = NULL;
    	int rc = 0;

    	file = popen("lshw -c memory 2>null | grep size", "r"); 
    	op = fgets(buffer, sizeof(buffer), file);
    if (op == NULL) {
        printf("failed");
    }	
   	 sprintf(mem, "%s", buffer);
    	
    	pclose(file);   
    	return 0;
}

int  getUser(char *user)
{
	
	char buffer[1024]  = {0};
    	char *op        = NULL;
    	FILE *file      = NULL;
    	int rc = 0;
    	file = popen("users","r"); 
    	op = fgets(buffer, sizeof(buffer), file);
    	 if (op == NULL) {
        printf("failed\n");
    }
       sprintf(user, "%s", buffer);

    	pclose(file);  
    	return 0; 
    	
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
