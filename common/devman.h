#ifndef __DEVMAN_H__
#define __DEVMAN_H__

#define DEVMAN_MAX_STRING_LEN 128

typedef struct clientInfo
{
    char        name[128];
    char        cpu[128];
    char        memory[128];
} clientInfo;

typedef struct devManMsg {
    int         msgID;
    clientInfo  info;
} devManMsg;

typedef enum
{
    DEVMAN_SUCCESS = 0,
    DEVMAN_FAILURE = 1
} DEVMAN_RET;

#endif /* __DEVMAN_H__ */
