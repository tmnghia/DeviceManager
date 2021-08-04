#ifndef _DEVMAN_CLI_H_
#define _DEVMAN_CLI_H_

typedef struct {
	int id;
	char ip[256];
	int port;
	char user[256];
	char cpu[256]; 
	char memory[256]; 
}info;

void Recv_char (int sfd, char *data);
int Recv_int (int sfd, int ret);

#endif
