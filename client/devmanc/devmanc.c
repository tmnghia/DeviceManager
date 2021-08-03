#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devmanc.h"

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

DEVMAN_RET main()
{
    clientInfo *info = NULL;

    info = (clientInfo *)calloc(1, sizeof(clientInfo));
    if (info == NULL) {
        printf("Error when allocating memory for client information\n");
        return DEVMAN_FAILURE;
    }

    if (getDeviceInformation(info) != DEVMAN_SUCCESS ) {
        printf("Error when getting device information\n");
        return DEVMAN_FAILURE;
    }

    printf("Device Name: %s\n", info->name);
    printf("Device CPU: %s\n", info->cpu);
    printf("Device Memory: %s\n", info->memory);

    free(info);

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

    return DEVMAN_SUCCESS;
}
