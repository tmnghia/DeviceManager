#ifndef __DEVMANC_H__
#define __DEVMANC_H__

#include "devman.h"

DEVMAN_RET getDeviceInformation(clientInfo *info);
DEVMAN_RET getName(char *name);
DEVMAN_RET getCPU(char *cpu);
DEVMAN_RET getMemory(char *mem);

#endif /* __DEVMANC_H__ */
