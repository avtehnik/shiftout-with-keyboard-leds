/*+--------------------------------------------------------------+
  |            server.c  -  description                          |
  |                -------------------                           |
  | begin      : 08/01/2010 13.00                                |
  | copyleft   : (C) 2010 xAppSoftware                           |
  | author     : Luigi D'Andrea                                  |
  | email      : gg1 ( at ) xappsoftware dot com                 |
  | compiling  : gcc -o server server.c                          |
  |                                                              |
  | Latest version on http://www.xappsoftware.com                |
  +--------------------------------------------------------------+
  | udp client-server may be redistributed and modified under    |
  | certain conditions. This software is distributed on an       |
  | "AS IS" basis WITHOUT WARRANTY OF ANY KIND, either express or|
  ! implied.  See the file License.txt for details.              |
  +--------------------------------------------------------------+*/

/*+--------------------------------------------------------------+
  | SYSTEM INCLUDES                                              |
  +--------------------------------------------------------------+*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

/*+--------------------------------------------------------------+
  | SPECIFIC INCLUDES                                            |
  +--------------------------------------------------------------+*/
#include "clientserver.h"
#include "demonize.h"

/*+--------------------------------------------------------------+
  | Function name :  main                                        |
  | Parameters    :  The port number                             |
  | Description   :  The simple udp server main                  |
  +--------------------------------------------------------------+*/

int state;
int fd ;
void setSH(int i) {
    //click
    if (i == 1) {
        state = state ^ 1;
    } else {
        state = state & (state ^1);
    }
    ioctl(fd, KDSETLED, state);
}

void setST(int i) {
    //out
    if (i == 1) {
        state = state ^ 2;
    } else {
        state = state & (state ^2);
    }
    ioctl(fd, KDSETLED, state);
}

void setDS(int i) {
    //data
    if (i == 1) {
        state = state ^ 4;
    } else {
        state = state & (state ^4);
    }
    ioctl(fd, KDSETLED, state);
}

int main(int argc, char **argv) {

    state = 0;
    struct sockaddr_in si_local, si_remote;
    int s, i, j, interval;
    int port;
    size_t slen;
    char buf[BUFLEN];

    slen = sizeof (si_remote);
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port number>  <interval> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    interval = atoi(argv[2]);
    port = atoi(argv[1]);
    if (port < 1024) {
        fprintf(stderr, "Usage: %s <port number>  <time delay>  \n", argv[0]);
        fprintf(stderr, "\twhere <port number> shall be > 1023\n");
        exit(EXIT_FAILURE);
    }

    demonize(argv[0]);
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset((char *) &si_local, 0, sizeof (si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(port);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (const struct sockaddr *) &si_local, sizeof (si_local)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    fd = open("/dev/console", O_NOCTTY);
    while (1) {
        memset(buf, 0, sizeof (char) *BUFLEN);
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_remote, &slen) == -1) {
            perror("recvfrom()");
            exit(EXIT_FAILURE);
        }
        if (strstr(buf, ".quit.") != NULL) {
            printf("Exiting\n");
            printf("\".quit.\" Received \n");
            break;
        } else {
            printf("Data received %i ", buf[0] );
            for (j = 1; j < buf[0]; j++) {
               for (i = 0; i < 8; i++) {


                if((buf[j] & (0x80 >> i))>0){
			printf("1");
		}else{
			printf("0");
		}


                    setDS((buf[j] & (0x80 >> i))>0);
                    usleep(interval);
                    setSH(1);
                    usleep(interval);
                    setSH(0);
                    usleep(interval);
                    setDS(0);
                    usleep(interval);
                }
		printf(" ");
            }
		printf("\n");

            usleep(interval);
            setST(1);
            usleep(interval);
            setST(0);
        }
    }
    close(fd);
    close(s);
    exit(EXIT_SUCCESS);
}
