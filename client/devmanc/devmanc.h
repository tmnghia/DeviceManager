#ifndef _DEVMANC_H_
#define _DEVMANC_H_

struct info
{
	char cpu[256]; 
	char user[256];
	char memory[256];
};

void GetCPU();
void Getmemory();
void Getusername();
void Delete(char s[], char s1[]);

#endif
