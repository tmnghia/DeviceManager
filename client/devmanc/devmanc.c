#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "devmanc.h"

int sockfd          = -1;
char srvIP[128]     = {0};
int srvPort         = DEVMAN_SERVER_PORT;
DEVMANC_OPER oper   = DEVMANC_OPER_INFO;

DEVMAN_RET handleArgsOption(int argc, char **argv)
{
    int option          = 0;

    // Getting argument options
    while ((option = getopt (argc, argv, ":h:p:s")) != -1) {
        switch (option)
        {
            case 'h':
                sprintf(srvIP, "%s", optarg);
                break;
            case 'p':
                srvPort = strtol(optarg, NULL, 10);
                break;
            case 's':
                oper = DEVMANC_OPER_SHOW;
                break;
            default:
                fprintf(stderr, "Invalid option: %c\n", option);
                return DEVMAN_FAILURE;        
        }
    }

    return DEVMAN_SUCCESS;
}

DEVMAN_RET main(int argc, char **argv)
{
    sprintf(srvIP, "%s", DEVMAN_SERVER_ADDRESS);

    // Handle the command arguments
    if (handleArgsOption(argc, argv) != DEVMAN_SUCCESS) {
        fprintf(stderr, "Error when getting argument options\n");
        return DEVMAN_FAILURE;
    }

    printf("Server IP: %s\n", srvIP);
    printf("Server Port: %d\n", srvPort);

    // Init socket connection
    if (initSocket(&sockfd, srvIP, srvPort) != DEVMAN_SUCCESS) {
        fprintf(stderr, "Error when initialzing socket\n");
        return DEVMAN_FAILURE;
    }

    // Getting client list from server then show out
    if (oper == DEVMANC_OPER_SHOW) {
        handleClientShow();
    } 
    // Connecting to the server then send the device information
    else if (oper == DEVMANC_OPER_INFO) {
        handleClientInfo();
    } 
    // Invalid operation, putting error then exit.
    else {
        fprintf(stderr, "Invalid operation %d\n", oper);
        return DEVMAN_FAILURE;
    }

    return DEVMAN_SUCCESS;
}

DEVMAN_RET handleClientShow(void)
{
    devManMsg *msg      = NULL;
    int sent, sentBytes = 0;

    msg = (devManMsg *)calloc(1, sizeof(devManMsg));
    if (msg == NULL) {
        printf("Error when allocating memory for client information\n");
        return DEVMAN_FAILURE;
    }

    msg->msgID = DEVMAN_MSG_REQ_CLIENTS;
    msg->payloadLen = 0;
    sentBytes = sizeof(devManMsg);

    printf("sent msgID: %d\n", msg->msgID);
    printf("sent payloadLen: %d\n", msg->payloadLen);

    sent = send(sockfd, msg, sentBytes, 0);
    if (sent < sentBytes) {
        fprintf(stderr, "Error when sending message.\n");
        return DEVMAN_FAILURE;
    }

    if (msg != NULL)
        free(msg);

    return DEVMAN_SUCCESS;
}

DEVMAN_RET handleClientInfo(void)
{
    clientInfo *info    = NULL;
    devManMsg *msg      = NULL;
    int sent, sentBytes = 0;

    info = (clientInfo *)calloc(1, sizeof(clientInfo));
    if (info == NULL) {
        printf("Error when allocating memory for client information\n");
        return DEVMAN_FAILURE;
    }

    msg = (devManMsg *)calloc(1, sizeof(devManMsg));
    if (msg == NULL) {
        printf("Error when allocating memory for client information\n");
        if (info != NULL)
            free(info);

        return DEVMAN_FAILURE;
    }

    // Getting client information
    if (getDeviceInformation(info) != DEVMAN_SUCCESS ) {
        printf("Error when getting device information\n");
        if (info != NULL)
            free(info);
        if (msg != NULL)
            free(msg);

        return DEVMAN_FAILURE;
    }

    printf("Device Name: %s\n", info->name);
    printf("Device CPU: %s\n", info->cpu);
    printf("Device Memory: %s\n", info->memory);

    // Sending client information
    msg->msgID = DEVMAN_MSG_CLIENT_INFO;
    msg->payloadLen = sizeof(clientInfo);
    memcpy(msg->payload, info, sizeof(clientInfo));

    clientInfo *sentInfo = NULL;
    sentInfo = (clientInfo *)msg->payload;
    sentBytes = sizeof(devManMsg) + sizeof(clientInfo);

    printf("sent msgID: %d\n", msg->msgID);
    printf("sent payloadLen: %d\n", msg->payloadLen);
    printf("sent Device Name: %s\n", info->name);
    printf("sent Device cpu: %s\n", info->cpu);
    printf("sent Device memory: %s\n", info->memory);

    sent = send(sockfd, msg, sentBytes, 0);
    if (sent < sentBytes) {
        fprintf(stderr, "Error when sending message.\n");
        return DEVMAN_FAILURE;
    }

    if (info != NULL)
        free(info);

    if (msg != NULL)
        free(msg);

    return DEVMAN_SUCCESS;
}

void removeLeadingSpaces(char* str)
{
    static char tmp[DEVMAN_MAX_STRING_LEN];
    int count = 0, j, k;
  
    // Iterate String until last leading space character
    while (str[count] == ' ') {
        count++;
    }
  
    // Putting string into the tmp string after
    // removing leading white spaces
    for (j = count, k = 0; str[j] != '\0'; j++, k++) {
        tmp[k] = str[j];
    }
    tmp[k] = '\0';

    // Putting string back to the old string.
    snprintf(str, strlen(tmp) + 1, "%s", tmp);
}

void removeNewline(char * str)
{
    int count = 0;

    printf("%c", str[strlen(str)]);
    for (count = 0; count < strlen(str); count++) {
        if (str[count] == '\n')
            str[count] = '\0';
    }
}

DEVMAN_RET initSocket(int *sockfd, char *srvIP, int port)
{
    struct sockaddr_in serv_addr;
    int rc;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        fprintf(stderr, "Error when creating socket\n");
        return DEVMAN_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // Convert the addresses from text to binary form
    if (inet_pton(AF_INET, srvIP, &serv_addr.sin_addr) < 1) {
        fprintf(stderr, "Invalid address: %s\n", srvIP);
        return DEVMAN_FAILURE;
    }

    rc = connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (rc < 0)
    {
        // fprintf(stderr, "Error when connecting to the server: %s\n", DEVMAN_SERVER_ADDRESS);
        perror("Error when connecting to the server");
        return DEVMAN_FAILURE;
    }

    return DEVMAN_SUCCESS;
}

DEVMAN_RET getDeviceInformation(clientInfo *info)
{
    DEVMAN_RET rc = DEVMAN_FAILURE;

    rc = getName(info->name);
    if (rc != DEVMAN_SUCCESS) {
        printf("Error when getting device name\n");
        return DEVMAN_FAILURE;
    }

    rc = getCPU(info->cpu);
    if (rc != DEVMAN_SUCCESS) {
        printf("Error when getting device CPU\n");
        return DEVMAN_FAILURE;
    }

    rc = getMemory(info->memory);
    if (rc != DEVMAN_SUCCESS) {
        printf("Error when getting device memory\n");
        return DEVMAN_FAILURE;
    }

    return DEVMAN_SUCCESS;
}

DEVMAN_RET getName(char *user)
{
    FILE *shell         = NULL;
    char *op            = NULL;
 
    shell = popen("whoami", "r");
    if (shell == NULL) {
        fprintf(stderr, "%s():%d - Error when getting information from linux shell\n", __FUNCTION__, __LINE__);
        return DEVMAN_FAILURE;
    }

    op = fgets(user, DEVMAN_MAX_STRING_LEN, shell);
    if (op == NULL) {
        fprintf(stderr, "Error when getting device name\n");
        return DEVMAN_FAILURE;
    }
    // Removing the newline if any
    removeNewline(user);

    // Removing leading whitespaces if any
    removeLeadingSpaces(user);

    pclose(shell);

    return DEVMAN_SUCCESS;
}

DEVMAN_RET getCPU(char *cpu)
{
    FILE *shell     = NULL;
    char *op        = NULL;
 
    shell = popen("lshw -c cpu 2>/dev/null | grep product | awk '{$1=\"\"; print $0}'", "r");
    if (shell == NULL) {
        fprintf(stderr, "%s():%d - Error when getting information from linux shell\n", __FUNCTION__, __LINE__);
        return DEVMAN_FAILURE;
    }

    op = fgets(cpu, DEVMAN_MAX_STRING_LEN, shell);
    if (op == NULL) {
        fprintf(stderr, "Error when getting device cpu\n");
        return DEVMAN_FAILURE;
    }

    // Removing the newline if any
    removeNewline(cpu);

    // Removing leading whitespaces if any
    removeLeadingSpaces(cpu);

    pclose(shell);

    return DEVMAN_SUCCESS;
}

DEVMAN_RET getMemory(char *memory)
{
    FILE *shell = NULL;
    char *op = NULL;
 
    shell = popen("lshw -c memory 2>/dev/null | grep size | awk '{print $2}'", "r");
    if (shell == NULL) {
        fprintf(stderr, "%s():%d - Error when getting information from linux shell\n", __FUNCTION__, __LINE__);
        return DEVMAN_FAILURE;
    }

    op = fgets(memory, DEVMAN_MAX_STRING_LEN, shell);
    if (op == NULL) {
        fprintf(stderr, "Error when getting device memory\n");
        return DEVMAN_FAILURE;
    }

    // Removing the newline if any
    removeNewline(memory);

    // Removing leading whitespaces if any
    removeLeadingSpaces(memory);

    pclose(shell);

    return DEVMAN_SUCCESS;
}
