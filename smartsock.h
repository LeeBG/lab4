
 #ifndef __SMARTSOCK_H__
#include <sys/select.h>
#define __SMARTSOCK_H__

#include <stdio.h>
#include <string.h>
#include <strings.h>
// socket & bind & listen & accept & connect
#include <sys/types.h>
#include <sys/socket.h>

// sockaddr_in
#include <netinet/in.h>

// read & write
#include <unistd.h>

// htonl
#include <arpa/inet.h>

// errno, perror
#include <errno.h>

// open
#include <fcntl.h>
#include <sys/stat.h>

//select
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT      7777
#define IP        "222.234.96.162"

#define MAX_USER     3
#define NIC_NAME_SIZE   9
#define NIC_NAME_MSG   (9+2)

#define BUF_SIZE  255              //only message
#define MSG_SIZE  (BUF_SIZE+1+NIC_NAME_MSG)  //message + Nick Name
#define MSG_END    "\x01\x02\x03"        



#endif /* __SMARTSOCK_H__ */
   
  
