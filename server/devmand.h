#ifndef _DEVMAND_H_
#define _DEVMAND_H_

struct info
{
	char ip[256];
	int port;
	char user[256];
	char cpu[256]; 
	char memory[256]; 
};

void write_int (int sfd, int number);

#endif
