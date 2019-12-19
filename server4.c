#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_NAME    30
#define    MAX_MSG    1024
#define SOCKETPORT    9000

typedef struct _client{
    int disc;
    int clientnum;
    int num;
    char name[MAX_NAME];
}Client;

Client list[5];

int sd;
int num;
int clientnum = 0;
int rusernum = 0;
pthread_mutex_t usermutex;

void SSockExit(int signo);
int SSockets(char *ip, int port);
void *Notice(void *client);
void CSockExit(Client client);
void *ReceiveSend(void *client);
void *StartTalk(void *client);
void *thread_main();

void *thread_main()
{
    Client client;// = *(Client *)cs;

    int sockid;
    int port = SOCKETPORT;

    sockid = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in ssockaddress = {0};
    ssockaddress.sin_family = AF_INET;
    ssockaddress.sin_port = htons(port);
    ssockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(sockid, (struct sockaddr *)&ssockaddress, sizeof(ssockaddress)))
    {
        perror("bind error");
        close(sockid);
    }
    
    if(listen(sockid, 5) == -1)
    {
        perror("listen error");
        close(sockid);
    }

    printf("port %d\n",port);

    struct sockaddr_in csockaddress = {0};
    int clen = sizeof(csockaddress);
    pthread_t ptr;
    
    while((client.disc = accept(sockid, (struct sockaddr *)&csockaddress, &clen)))
    {
        if(client.disc == -1)
        {
            perror("accept");
            return 0;
        }

        pthread_mutex_lock(&usermutex);
        
        client.clientnum = clientnum;

        client.num = sockid;
        pthread_create(&ptr, NULL, StartTalk, &client);
        pthread_detach(ptr);

        clientnum++;
        
        pthread_mutex_unlock(&usermutex);
    }
    
    return 0;
}

void *StartTalk(void *client)
{
    Client cs = *(Client *)client;
    char nick[MAX_NAME]="";
    pthread_t ptr[2];

    send(cs.disc, "채팅방에 오신 것을 환영합니다.\n닉네임을 입력하세요.\n", 100, 0);
    recv(cs.disc, nick, sizeof(nick), 0);
    nick[strlen(nick)-1] = '\0';
    printf("%s 접속\n",nick);
    
    strcpy(cs.name, nick);

    list[cs.clientnum] = cs;

    pthread_create(&ptr[0], NULL, ReceiveSend, &cs);
    pthread_create(&ptr[1], NULL, Notice, &cs);
    pthread_join(ptr[0], NULL);
    pthread_join(ptr[1], NULL);
}

void *ReceiveSend(void *client)
{
    Client cs = *(Client *)client;
    int i;
    char rbuf[MAX_MSG];
    char sbuf[MAX_MSG];

    while(recv(cs.disc, rbuf, sizeof(rbuf), 0) >0)
    {
        sprintf(sbuf, "%s : %s", cs.name, rbuf);
        for(i=0;i<clientnum;i++)
        {
            if(list[i].disc == cs.disc)
                continue;
            if(list[i].num == cs.num)
                send(list[i].disc, sbuf, sizeof(sbuf), 0);
        }

        memset(rbuf, 0, sizeof(rbuf));
        memset(sbuf, 0, sizeof(sbuf));
    }    

    CSockExit(cs);
}

void CSockExit(Client client)
{
    int i, j;
    char sbuf[MAX_MSG];
    
    sprintf(sbuf, "%s님이 퇴장하였습니다.\n", client.name);

    for(i=0;i<clientnum;i++)
    {
        if(list[i].disc == client.disc)
        {
            for(j=0;j<clientnum;j++)
            {
                if(list[j].disc == client.disc)
                    continue;
                if(list[j].num == client.num)
                    send(list[j].disc, sbuf, sizeof(sbuf), 0);
            }
            break;
        }
    }

    pthread_mutex_lock(&usermutex);
    for(j=i;j<clientnum-1;j++)
    {
        list[j] = list[j+1];
    }
    clientnum --;
    pthread_mutex_unlock(&usermutex);
}

void *Notice(void *client)
{
    int i;
    Client cs = *(Client *)client;
    char sbuf[MAX_MSG];
    char inbuf[MAX_MSG];
    
    sprintf(sbuf, "%s님이 접속 하였습니다.\n", cs.name);
    for(i=0;i<clientnum;i++)
    {
        if(list[i].num == cs.num)
            send(list[i].disc, sbuf, strlen(sbuf), 0);
    }

    while(1)
    {
        memset(sbuf, 0, sizeof(sbuf));
        memset(inbuf, 0, sizeof(inbuf));
        fgets(inbuf, sizeof(inbuf), stdin);
        sprintf(sbuf, "[SERVER] : %s\n", inbuf);
        for(i=0;i<clientnum;i++)
            send(list[i].disc, sbuf, strlen(sbuf), 0);
    }
}

int SSockets(char *ip, int port)
{
    int sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in ssockaddress = {0};
    ssockaddress.sin_family = AF_INET;
    ssockaddress.sin_port = htons(port);
    ssockaddress.sin_addr.s_addr = inet_addr(ip);

    if(bind(sockid, (struct sockaddr *)&ssockaddress, sizeof(ssockaddress)))
    {
        perror("bind error");
        close(sockid);
        return -1;
    }
    
    if(listen(sockid, 5) == -1)
    {
        perror("listen error");
        close(sockid);
        return -1;
    }
    return sockid;
}

void SSockExit(int signo)
{
    printf("서버 종료\n");
    close(num);
    close(sd);
    exit(0);
}


int main()
{
    int room = 1, i;

    signal(SIGINT, SSockExit);

    pthread_mutex_init(&usermutex, NULL);
 
    pthread_t ptr;

    pthread_create(&ptr, NULL, thread_main, &room);
    pthread_detach(ptr);

    while(1)
        pause();

    return 0;
}
