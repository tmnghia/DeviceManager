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

int main(int argc, char const *argv[]) 
{ 
	info client;
	int sfd;
    	struct sockaddr_in svaddr;  
    	char *ok = "OK"; 
    	char *data = "data"; 
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
    	write(sfd, data, strlen(data));
    	GetInfo(client.user, client.cpu, client.memory);
    	Send_char(sfd, client.user);
    	Send_char(sfd, client.cpu);
    	Send_char(sfd, client.memory);
    	while(1) {
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
	for(i=0; i<len; i++) {
		if((s[i]==s1[0]) && (s[i+len1-1]==s1[len1-1])) {
			int j;
			for(j=i; j<len; j++) {
				s[j]=s[j+len1];
			}
			s[j+len1]='\0';
			break;
		}
	}
}

char *GetCPU(char *cpu)
{   
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(cpu, 0, sizeof(cpu));
    	FILE *file = popen("lshw -c cpu","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    
    	while(NULL != fgets(buffer, sizeof(buffer), file)) {  
    		if (strstr(buffer,"product")!=NULL) {
            		strcat(cpu, buffer);
            	} 
    	}  
    	pclose(file);   
    	Delete(cpu,"product:");
    	Delete(cpu,"\n");
}

char *Getmemory(char *mem)
{     
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(mem, 0, sizeof(mem));
    	FILE *file = popen("lshw -c memory","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    
    	while(NULL != fgets(buffer, sizeof(buffer), file)) {  
    		if (strstr(buffer,"size")!=NULL) {
            		strcat(mem, buffer);
            	} 
    	}  
    	pclose(file);   
    	Delete(mem,"size:");
    	Delete(mem,"\n");
    	strcat(mem, " System memory");
}

char *Getusername(char *name)
{
    	memset(buffer, 0, sizeof(buffer)); 
    	memset(name, 0, sizeof(name));
    	FILE *file = popen("users","r"); 
    	fgets(buffer, sizeof(buffer), file);  
    	strcpy(name, buffer);
    	pclose(file);
    	Delete(name,"\n");
}

void GetInfo(char *name, char *cpu, char *mem)
{
	Getusername(name);
    	GetCPU(cpu);
    	Getmemory(mem);
}

void Send_char (int sfd, char *data)
{
	memset(buffer, 0, 256);
	read(sfd, buffer, 255);
	if (strcmp(buffer, "OK") == 0) {
		write(sfd , data , strlen(data));
	}
}
