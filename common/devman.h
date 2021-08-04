#ifndef __DEVMAN_H__
#define __DEVMAN_H__

#define DEVMAN_SERVER_ADDRESS   "127.0.0.1"
#define DEVMAN_SERVER_PORT      8800

#define DEVMAN_MAX_STRING_LEN   128
#define DEVMAN_MAX_CLIENT_NUM   5
#define DEVMAN_MAX_BUFFER_SIZE  1024

#define DEVMAN_MSG_CLIENT_JOIN  1
#define DEVMAN_MSG_GET_CLIENTS  2

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
