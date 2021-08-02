#ifndef _DEVMAN_CLI_H_
#define _DEVMAN_CLI_H_

struct info
{
	int id;
	char ip[256];
	int port;
	char user[256];
	char cpu[256]; 
	char memory[256]; 
};

void recv_char (int sfd, char *data);
int recv_int (int sfd, int ret);

#endif
