#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>
#include "devmanc.h"

#define PORT 8080

char buffer[1024]; 
FILE *file = NULL;
struct info client;

int main(int argc, char const *argv[]) 
{ 
	int sfd;
    	struct sockaddr_in svaddr;  
    	char *ok = "OK"; 
    	char *data = "data"; 
	sfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sfd < 0) 
        {	
        	perror("ERROR socket");
        	exit(EXIT_FAILURE);
	}
    	memset(&svaddr, '0', sizeof(svaddr)); 
    	svaddr.sin_family = AF_INET; 
    	svaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    	svaddr.sin_port = htons(PORT); 
	
    	if (connect(sfd, (struct sockaddr *)&svaddr, sizeof(svaddr)) < 0) 
    	{ 
        	perror("ERROR connect");
        	exit(EXIT_FAILURE);
    	} 
    	write(sfd, data, strlen(data));
    	Getusername();
    	write(sfd, client.user, strlen(client.user));
    	GetCPU();
    	write(sfd, client.cpu, strlen(client.cpu)); 
    	Getmemory();
    	write(sfd, client.memory, strlen(client.memory));
    	
    	while(1)
    	{
             	printf("Send: ");
             	memset(buffer, 0, sizeof(buffer));
        	fgets(buffer, sizeof(buffer), stdin);
              	write(sfd, buffer, strlen(buffer)); 
    	}
    	
    	close(sfd);
    	return 0; 
} 

void Delete(char s[], char s1[])
{
	int len = strlen(s);
	int len1 = strlen(s1);
	int i;
	for(i=0; i<len; i++)
	{
		if((s[i]==s1[0]) && (s[i+len1-1]==s1[len1-1]))
		{
			int j;
			for(j=i; j<len; j++)
			{
				s[j]=s[j+len1];
			}
			s[j+len1]='\0';
			break;
		}
	}
}

void GetCPU()
{
	FILE *file = NULL;      
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.cpu, 0, sizeof(client.cpu));
  
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
    	Delete(client.cpu,"product:");
    	Delete(client.cpu,"\n");
}

void Getmemory()
{     
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.memory, 0, sizeof(client.memory));
  
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
    	Delete(client.memory,"size:");
    	Delete(client.memory,"\n");
    	strcat(client.memory, " System memory");
}

void Getusername()
{
	FILE *file = NULL;      
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(client.user, 0, sizeof(client.user));
  
    	file = popen("users","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    	strcpy(client.user, buffer);
    	Delete(client.user,"\n");
}
