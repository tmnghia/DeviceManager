#ifndef __DEVMAND_H__
#define __DEVMAND_H__

#include "devman.h"

DEVMAN_RET initServerSocket(int *srvSockFd, struct sockaddr_in *address);

DEVMAN_RET initSelectSet(fd_set *readFds, fd_set *writeFds, fd_set *exceptionFds);
DEVMAN_RET addFdToSelectSet(int fd, fd_set *fdSet);

#endif