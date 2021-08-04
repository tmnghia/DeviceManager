#ifndef _DEVMAND_H_
#define _DEVMAND_H_

typedef struct {
	char ip[256];
	int port;
	char user[256];
	char cpu[256]; 
	char memory[256]; 
}info;

void Write_int (int sfd, int number);
void Send_char (int sfd, char *data);
void Send_int (int sfd, int ret);

#endif
