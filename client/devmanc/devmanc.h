#ifndef _DEVMANC_H_
#define _DEVMANC_H_

typedef struct
{
	char cpu[256]; 
	char user[256];
	char memory[256];
}info;

char *GetCPU(char *cpu);
char *Getmemory(char *mem);
char *Getusername(char *name);
void GetInfo(char *name, char *cpu, char *mem);
void Delete(char s[], char s1[]);
void Send_char (int sfd, char *data);

#endif
