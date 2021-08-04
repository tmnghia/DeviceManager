#ifndef __DEVMANC_H__
#define __DEVMANC_H__

#include "devman.h"

typedef enum {
    DEVMANC_OPER_INFO  = 0,
    DEVMANC_OPER_SHOW  = 1
} DEVMANC_OPER;

DEVMAN_RET getDeviceInformation(clientInfo *info);
DEVMAN_RET getName(char *name);
DEVMAN_RET getCPU(char *cpu);
DEVMAN_RET getMemory(char *mem);

DEVMAN_RET initSocket(int *sockfd, char *srvIP, int port);

DEVMAN_RET handleClientShow(void);
DEVMAN_RET handleClientInfo(void);

#endif /* __DEVMANC_H__ */
