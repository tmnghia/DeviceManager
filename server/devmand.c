#include "unistd.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <errno.h>

#include "devmand.h"

DEVMAN_RET main()
{
    int srvSockFd = 0;
    int newSockFd = 0;
    int opt = 1;
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    int addrlen = sizeof(clientAddress);
    char buffer[DEVMAN_MAX_BUFFER_SIZE] = {0};
    char clientName[DEVMAN_MAX_STRING_LEN] = {0};
    int clientSocketList[DEVMAN_MAX_CLIENT_NUM] = {0};
    int activity = 0;
	int maxSockFd = 0;
    int clientSockFd = 0;
    int i = 0;
    fd_set readFds;

    // address = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEVMAN_SERVER_PORT);

    // Init the server socket
    if ( initServerSocket(&srvSockFd, &address) != DEVMAN_SUCCESS) {
        fprintf(stderr, "Error when initializing the server socket\n");
        return DEVMAN_FAILURE;
    }

    // Select to handle multiple connection.
    while(1) {
        // Init the set
        initSelectSet(&readFds, NULL, NULL);

        // Add server socket to the set
        addFdToSelectSet(srvSockFd, &readFds);
        maxSockFd = srvSockFd;

        // Add client socket to the set
        for (i = 0; i < DEVMAN_MAX_CLIENT_NUM; i++) {
            clientSockFd = clientSocketList[i];
            
            if (clientSockFd > 0)
                addFdToSelectSet(clientSockFd, &readFds);

            // Update highest file descriptor
            if (clientSockFd > maxSockFd)
                maxSockFd = clientSockFd;
        }

        // Waiting for an activity
        activity = select(maxSockFd + 1, &readFds, NULL, NULL, NULL);
        if ((activity < 0) && (errno!=EINTR)) {
            perror("Error when selecting ready socket");
            return DEVMAN_FAILURE;
        }

        // New connection
        if (FD_ISSET(srvSockFd, &readFds)) {
            newSockFd = accept(srvSockFd, (struct sockaddr *)&clientAddress, (socklen_t*)&addrlen);
            if (newSockFd < 0) {
                fprintf(stderr, "Error when accepting connection\n");
                return DEVMAN_FAILURE;
            }

            inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName));
            printf("Incoming connection from %s:%d sockfd: %d\n", clientName, clientAddress.sin_port, newSockFd);

            // Adding client socket to the set.
            for (i = 0; i < DEVMAN_MAX_CLIENT_NUM; i++) {
                if (clientSocketList[i] == 0) {
                    clientSocketList[i] = newSockFd;
                    printf("Added socket %d to list index: %d\n", newSockFd, i);
                    break;
                }
            }
        }
        else { // IO from the client socket.
            for (i = 0; i < DEVMAN_MAX_CLIENT_NUM; i++) {
                clientSockFd = clientSocketList[i];
                if (FD_ISSET(clientSockFd, &readFds)) {
                    // Client connection is closing.
                    if (0 == recv(clientSockFd, buffer, sizeof(clientInfo), 0)) {
                        printf("Socket %d is closing\n", clientSockFd);
                        getpeername(clientSockFd , (struct sockaddr*)&clientAddress , (socklen_t*)&addrlen);
                        inet_ntop(AF_INET, &clientAddress.sin_addr, clientName, sizeof(clientName));
                        printf("Client %s:%d sockfd: %d disconnected\n", clientName, clientAddress.sin_port, clientSockFd);

                        // Cleaning up
                        close(clientSockFd);
                        clientSocketList[i] = 0;
                    }
                    else { // Incoming message from client
                        clientInfo info;
                        memcpy(&info, buffer, sizeof(clientInfo));
                        printf("\nClient information:\n");
                        printf("name: %s\n", info.name);
                        printf("name: %s\n", info.cpu);
                        printf("name: %s\n", info.memory);
                        printf("\n");
                    }
                }
            }
        }
    }

    return DEVMAN_SUCCESS;
}

DEVMAN_RET initSelectSet(fd_set *readFds, fd_set *writeFds, fd_set *exceptionFds)
{
    // Initializes the file descriptor set fdset to have zero bits for all file descriptors
    if (readFds != NULL)
        FD_ZERO(readFds);

    if (writeFds != NULL)
        FD_ZERO(writeFds);

    if (exceptionFds != NULL)
        FD_ZERO(exceptionFds);

    return DEVMAN_SUCCESS;
}

DEVMAN_RET addFdToSelectSet(int fd, fd_set *fdSet)
{
    if (fdSet == NULL) {
        fprintf(stderr, "fdSet is NULL\n");
        return DEVMAN_FAILURE;
    }

    if (fd < 0) {
        fprintf(stderr, "Invalid file descriptor: %d", fd);
        return DEVMAN_FAILURE;
    }

    FD_SET(fd, fdSet);
    printf("Added file descriptor %d to the set\n", fd);

    return DEVMAN_SUCCESS;
}

DEVMAN_RET initServerSocket(int *srvSockFd, struct sockaddr_in *address)
{
    int opt;
    int rc = 0;

    *srvSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (*srvSockFd < 0)
    {
        fprintf(stderr, "Error when creating socket.\n");
        return DEVMAN_FAILURE;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(*srvSockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        fprintf(stderr, "Error when setting socket option.\n");
        return DEVMAN_FAILURE;
    }

    rc = bind(*srvSockFd, (struct sockaddr *)address, sizeof(*address));
    if (rc < 0) {
        fprintf(stderr, "Error when binding socket.\n");
        return DEVMAN_FAILURE;
    }

    if (listen(*srvSockFd, DEVMAN_MAX_CLIENT_NUM) < 0)
    {
        fprintf(stderr, "Error when listening");
        return DEVMAN_FAILURE;
    }
    printf("Listening on port %d ...\n", DEVMAN_SERVER_PORT);

    return DEVMAN_SUCCESS;
}

