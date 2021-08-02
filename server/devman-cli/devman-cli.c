#include<conio.h>
#include<stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define MAX 50

struct info
{
	char ip[256];
	int port;
	char user[256];
	char cpu[256]; 
	char memory[256]; 
}cli[MAX];

int main()
{
	int key;
	char name[256];
	int i;
	printf("Nhap lua chon cua ban: ");
	scanf("%d", key);
	switch (key)
	{
		case 1:
			printf("Hien tat ca nguoi dung\n");
			printf("Socket ID      Device Name          IP Address     Port\n);
			printf("-----------+-------------------+----------------+-------------\n";
			for (i=0; i<MAX; i++)
			{
				printf("%d %s %s %d", i, cli[i].user, cli[i].ip, cli[i].port);
			}
			break;
		case 2:
			printf("Nhap ten nguoi dung: ");
			scanf("%s", name);
			for (i=0; i<MAX; i++)
			{
				if ((strcmp(cli[i].user,nam))==0)
				{
					printf("Device name     :         %s\n", cli[i].user);
					printf("Processor       : %s\n", cli[i].cpu);
					printf("Memory          : %s\n", cli[i].memory);
					printf("IP Address      :         %s\n", cli[i].ip);
					printf("Connected port  :         %d\n", cli[i].port);
				}
				break;
			}
			break;
}
