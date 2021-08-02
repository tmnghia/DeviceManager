#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "devmand.h"
	
#define PORT 8080
#define MAX 50

struct info cli[MAX];
char buffer[256];
char *ok = "OK";
	
int main(int argc , char *argv[])
{
	int opt = 1;
	int sfd, valread, client[MAX], addrlen, i, activity, newsfd, sd, max_sd, number;
	struct sockaddr_in svaddr;
	fd_set readfds;
		
	for (i = 0; i < MAX; i++)
	{
		client[i] = 0;
	}	
	if( (sfd = socket(AF_INET , SOCK_STREAM , 0)) < 0)
	{
		perror("ERROR socket");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0)
	{
		perror("ERROR setsockopt");
		exit(EXIT_FAILURE);
	}
	svaddr.sin_family = AF_INET;     
     	svaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
     	svaddr.sin_port = htons(PORT);
	if (bind(sfd, (struct sockaddr *)&svaddr, sizeof(svaddr))<0) 
    	{ 
        	perror("ERROR bind"); 
        	exit(EXIT_FAILURE); 
    	}
    	if (listen(sfd, 3) < 0) 
    	{ 
        	perror("ERROR listen"); 
        	exit(EXIT_FAILURE); 
    	} 	
	addrlen = sizeof(svaddr);
	
		
	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(sfd, &readfds);
		max_sd = sfd;
		
		for ( i = 0 ; i < MAX ; i++)
		{
			sd = client[i];
			if(sd > 0)
				FD_SET( sd , &readfds);
			if(sd > max_sd)
				max_sd = sd;
		}
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		
		if (FD_ISSET(sfd, &readfds))
		{
			if ((newsfd = accept(sfd, (struct sockaddr*)&svaddr, (socklen_t*)&addrlen))<0)
			{
				perror("ERROR accept");
				exit(EXIT_FAILURE);
			}
			memset(buffer, 0, 256);
			valread = read(newsfd, buffer, 255);
			if (strcmp(buffer, "show") == 0)
			{
				number = 0;
				for (i=0; i<MAX; i++)
				{
					if (client[i] != 0)
					{
						number = number + 1;
					}
				}
				write_int(newsfd, number);
				
				for (i=0; i<MAX; i++)
				{
					if (client[i] != 0)
					{
						
					}
				}
				close(newsfd);
			}
			else if (strcmp(buffer, "data") == 0)
			{
			for (i = 0; i < MAX; i++)
			{
				if( client[i] == 0 )
				{
					client[i] = newsfd;
					printf("Add client %d\n", i);	
					strcpy(cli[i].ip, inet_ntoa(svaddr.sin_addr));
					printf("IP Address      :         %s\n", cli[i].ip);
					cli[i].port = ntohs(svaddr.sin_port);
					printf("Connected port  :         %d\n", cli[i].port);
					break;
				}
			} 
			valread = read(newsfd, cli[i].user, 255);
			printf("Device name     :         %s\n", cli[i].user);
			memset(buffer, 0, 256);
			valread = read(newsfd, cli[i].cpu, 255);
			printf("Processor       : %s\n", cli[i].cpu);
			memset(buffer, 0, 256);
			valread = read(newsfd, cli[i].memory, 255);
			printf("Memory          : %s\n", cli[i].memory);
			}
		}
			
		for (i = 0; i < MAX; i++)
		{
			sd = client[i];	
			if (FD_ISSET( sd , &readfds))
			{
				memset(buffer, 0, 256);
				if ((valread = read( sd, buffer, 255)) == 0)
				{
					printf("Client %d disconnect\n",i);
					close(sd);
					client[i] = 0;
				}	
				else
				{
    					printf("Receive from client %d: %s",i,buffer);
    					write(sd , ok , strlen(ok));
    				}
			}
		}
	}	
	return 0;
}

void write_int (int sfd, int number)
{
	int converted_number = htonl(number);
	write(sfd, &converted_number, sizeof(converted_number));
}
