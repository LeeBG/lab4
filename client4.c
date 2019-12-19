#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define MAX_NAME    30
#define MAX_MSG    1024

typedef struct _client{
    int disc;
    int unum;
    int num;
    char client_name[MAX_NAME];
}Client;

int service;
int num;
pthread_mutex_t usermutex;

void CSockExit(int signo);
int Sockets(char *ip, int port);
void *SendMsg(void *client);
void *RecvMsg(void *client);
void StartTalk(int dsc);

void StartTalk(int dsc)
{
    Client client;
    pthread_t ptr[2];
    char name[MAX_NAME]="";
    char rbuf[MAX_MSG]="";

    recv(dsc, rbuf, 100, 0);
    fputs(rbuf,stdout);
    fgets(name, sizeof(name), stdin);
    send(dsc, name, strlen(name), 0);
    client.disc = dsc;
    strcmp(client.client_name,name);
    
    pthread_create(&ptr[0], NULL, RecvMsg, &client);
    pthread_detach(ptr[0]);
    pthread_create(&ptr[1], NULL, SendMsg, &client);
    pthread_detach(ptr[1]);

    while(1)
        pause();
}

void *RecvMsg(void *client)
{
    Client cs = *(Client *)client;
    char rbuf[MAX_MSG];
    int cnt;

    memset(rbuf, 0, sizeof(rbuf));

    while(1)
    {
        recv(cs.disc, rbuf, sizeof(rbuf), 0);
        memset(rbuf, 0, sizeof(rbuf));

        while(recv(cs.disc, rbuf, sizeof(rbuf), 0) > 0)
        {
            fputs(rbuf, stdout);
            memset(rbuf, 0, sizeof(rbuf));
        }
    }
}

void *SendMsg(void *client)
{
    Client cs = *(Client *)client;
    char sbuf[MAX_MSG];

    while(1)
    {
        fgets(sbuf, sizeof(sbuf), stdin);
        send(cs.disc, sbuf, sizeof(sbuf), 0);
        memset(sbuf, 0, sizeof(sbuf));
    }
}

int Sockets(char *ip, int port)
{
    int dsc;
    
    if((dsc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock error");
        return -1;
    }
    
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    if(connect(dsc, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("connect");
        return -1;
    }

    return dsc;
}

void CSockExit(int signo)
{
    printf("클라이언트를 종료합니다.\n");
    close(service);
    exit(0);
}

int main(int argc,char *argv[])
{
    int port;
    char pt[MAX_MSG];

    printf("포트를 입력하세요 :");
    scanf("%d", &port);
    getchar();
    signal(SIGINT, CSockExit);
    
    pthread_mutex_init(&usermutex, NULL);
    
    if((service = Sockets(IP, port)) == -1)
    {
        perror("socket");
        return 0;
    }

    StartTalk(service);

    return 0;
}
