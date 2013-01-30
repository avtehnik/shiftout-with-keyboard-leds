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
int main(int argc, char **argv)
{
	struct sockaddr_in   si_local, si_remote;
	int                  s,i,j,interval;
	int                  port;
	size_t               slen;
	char                 buf[BUFLEN];
	interval = 70000;

	slen     =   sizeof(si_remote);
	if(argc!=2){
		fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port=atoi(argv[1]);
	if(port<1024){
		fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
		fprintf(stderr, "\twhere <port number> shall be > 1023\n");
		exit(EXIT_FAILURE);
	}

   // demonize(argv[0]);
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset((char *) &si_local, 0, sizeof(si_local));
	si_local.sin_family       =  AF_INET;
	si_local.sin_port         =  htons(port);
	si_local.sin_addr.s_addr  =  htonl(INADDR_ANY);
	if (bind(s, (const struct sockaddr *)&si_local, sizeof(si_local))==-1){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	int fd=open("/dev/console",O_NOCTTY);
			ioctl(fd, KDSETLED,0);
			ioctl(fd, KDSETLED,1);
		 	usleep(interval);
			ioctl(fd, KDSETLED,0);
			ioctl(fd, KDSETLED,2);
		 	usleep(interval);
			ioctl(fd, KDSETLED,0);
			ioctl(fd, KDSETLED,4);
		 	usleep(interval);
			ioctl(fd, KDSETLED,0);

	while(1){
		memset(buf, 0, sizeof(char)*BUFLEN);
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_remote, &slen)==-1){
			perror("recvfrom()");
			exit(EXIT_FAILURE);
		}
		if(strstr(buf, ".quit.")!=NULL){
			printf("\".quit.\" Received \n");
			printf("Exiting\n");
			break;
		}
		else {


			printf("all off\n");
			ioctl(fd, KDSETLED,0);


			for(j=1; j<buf[0]; j++){
				printf("BYTE %d\n",j);
				for(i=0; i<8; i++){
					ioctl(fd, KDSETLED, 0);
					if(buf[j] & (1<<i)){
						printf("ScrollLock ON\n");
						ioctl(fd, KDSETLED, 1);
					}else{
						printf("NumLock ScrollLock on\n");
						ioctl(fd, KDSETLED,3);
					}

				 	usleep(interval);
					printf("all off\n");
					ioctl(fd, KDSETLED, 0);// выключить все
				 	usleep(interval);
				}
 			}
			printf("CapsLock on\n");
			ioctl(fd, KDSETLED,4);
		 	usleep(interval);
			printf("all off\n");
			ioctl(fd, KDSETLED,0);

			printf("end of packet\n");
			printf("\n");
			//NumLock==2,
			//ScrollLock==1,
			//CapsLock==4. Комбинируя сумму этих чисел можем вкючить те или инные диоды. В данном случае будут гореть только Num и Caps
			
			//printf("Received packet from %s:%d\n", inet_ntoa(si_remote.sin_addr), ntohs(si_remote.sin_port));
			//printf("Data: %s\n", buf);
		}
	}


	close(fd);
	close(s);
	exit(EXIT_SUCCESS);
}
