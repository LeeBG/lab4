#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>                     
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

 
#define EPOLL_SIZE 60
#define EPOLL_EVENT_SIZE 100
#define OPENPORT 9000                       // open port no

// main
int main (int argc, char **argv)
{

        struct epoll_event *events;          // epoll 
        struct epoll_event ev;        
        struct sockaddr_in addr, clientaddr;
        int clilen;
        int sfd, efd, cfd;                   // server socket, epoll socket, client 
        int i,j;                             // for
        int max_got_events;
        int result;
        int readn;
        int sendflags = 0;
        char buf_in[256] = { '\0' };         // buffer
        int count = 0;
 
        // epoll socket
        if ((efd = epoll_create (EPOLL_EVENT_SIZE)) < 0)
        {
                perror ("epoll_create (1) error");
                return -1;
        }
        // init pool
        events = (struct epoll_event *) malloc (sizeof (*events) * EPOLL_SIZE);
        if (NULL == events)
        {
                perror ("epoll_create (0) error");
                return -1;
        }

        // 서버 소켓 생성
        clilen = sizeof (clientaddr);
        sfd = socket (AF_INET, SOCK_STREAM, 0);
        if (sfd == -1)
        {
                perror ("socket error :");
                close (sfd);
                return -1;
        }
        // 소켓 정보 넣고 bind, listen 처리 
        addr.sin_family = AF_INET;
        addr.sin_port = htons (OPENPORT);
        addr.sin_addr.s_addr = htonl (INADDR_ANY);
        if (bind (sfd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
        {
                close (sfd);
                return -2;
        }
        listen (sfd, 5);
        if (sfd < 0)
        {
                perror ("init_acceptsock error");
                return 1;
        }
        // 서버시작, epoll 이벤트 설정 및 컨트롤 함수로 활성화 
        printf("Running Server port %d\n",OPENPORT);
        ev.events = EPOLLIN;
        ev.data.fd = sfd;
        result = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &ev);
        if (result < 0)
        {
                perror ("epoll_ctl error");
                return 1;
        }

        // 서버 실제 루트
        while (1)
        {
                // epoll 이벤트 대기 (클라이언트의 반응(접속 or send) 감지)
                max_got_events = epoll_wait (efd, events, EPOLL_SIZE, -1);
                // epoll 이벤트 받은만큼의 숫자만큼 for
                for (i = 0; i < max_got_events; i++)
                {
                        // 이벤트 들어온 소켓이 연결 요청이면
                        if (events[i].data.fd == sfd)
                        {
                                cfd = accept (sfd, (struct sockaddr *)
                                                &clientaddr, &clilen);
                                if (cfd < 0)
                                {
                                        perror ("Accept error");
                                        return -1;
                                }

                                ev.events = EPOLLIN;
                                ev.data.fd = cfd;
                                epoll_ctl (efd, EPOLL_CTL_ADD, cfd, &ev);
                                printf("Accepted socket : %d\n",cfd);
                                count++;
                        }

                        // 그게 아니면,
                        else
                        {
                                cfd = events[i].data.fd;
                                memset (buf_in, 0x00, 256);
                                readn = read (cfd, buf_in, 255);
                                // if it occured ploblems with reading,
                                // delete from epoll event pool and close socket                              // readn 값이 0 이하이면 recv 처리

                                if (readn <= 0)
                                {
                                        epoll_ctl(efd, EPOLL_CTL_DEL, cfd, &ev);
                                        close (cfd);
                                        printf ("Close fd %d\n", cfd);
                                        count--;
                                }
                                else
                                {
                                // 아니면 send 처리
                                //      printf ("%s", buf_in);
                                        for(j=5; j<count+5; j++)
                                        {
                                          // 단 이벤트로온 client엔 send 하지말고
                                          // 다른 모든 클라이언트에 send 해서 채팅
                                                if(cfd != j)
                                                        send (j, buf_in,
                                                         strlen (buf_in),
                                                         sendflags);
                                        }
                                }
                        }

                }

        }
        return 1;
}

