#ifndef __DEVMAND_H__
#define __DEVMAND_H__

#include "devman.h"

DEVMAN_RET initServerSocket(int *srvSockFd, struct sockaddr_in *address);

DEVMAN_RET initSelectSet(fd_set *readFds, fd_set *writeFds, fd_set *exceptionFds);
DEVMAN_RET addFdToSelectSet(int fd, fd_set *fdSet);
DEVMAN_RET handleSelect(int srvSocket);

DEVMAN_RET handleNewConnection(int srvSockFd);
DEVMAN_RET handleIncomingMsg(int clientSockFd, int idx);

DEVMAN_RET addClientInfo(clientInfo *info);
DEVMAN_RET handleClientInfoRequest(int fd);

#endif