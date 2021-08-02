#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUF_SIZE 1024
#define SIZE 1024

struct inform 
{
   char  *cpu;
   char  *memory;
   char  *user;
} client;



void error_handling(char *message);
void getusername();
void getCPU();
void getMemory();




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
		printf("Connected...........");
	  
	  
	 getusername();
	 getCPU();
	 getMemory(); 
	        
	 while(1)
	 {	 
	       
		
               send (sock,&client, sizeof(struct inform),0);
              //write(sock, message, strlen(message));
               printf ("Inform sent to Server\n");    
        } 
	close(sock);
	return 0;
}


void getCPU()
{
	FILE *file;      
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.cpu, 0, sizeof(client.cpu));
        
         if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
  	}	
 
    	file = popen("lshw -c cpu","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    
    	while(NULL != fgets(buffer, sizeof(buffer), file)) 
    	{  
    		if (strstr(buffer,"product")!=NULL)
    		{
            		strcat(client.cpu, buffer);
            	} 
    	}  
    	pclose(file);   
    	
    	return 0;
    	
}



void getMemory()
{      

	FILE *file;
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.memory, 0, sizeof(client.memory));
  
   	if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
  	}	
 
    	file = popen("lshw -c memory","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    
    	while(NULL != fgets(buffer, sizeof(buffer), file)) 
    	{  
    		if (strstr(buffer,"size")!=NULL)
    		{
            		strcat(client.memory, buffer);
            	} 
    	}  
    	pclose(file);   
    	return 0;
}

void getusername()
{
	FILE *file = NULL;      
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.user, 0, sizeof(client.user));
  
    	file = popen("users","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    	strcpy(client.user, buffer);
    	pclose(file);   
    	return 0;
    	
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
