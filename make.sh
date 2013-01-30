cc -pedantic -Wall -g -O2   -c -o client.o client.c

cc -o udp_client client.o

cc -pedantic -Wall -g -O2   -c -o demonize.o demonize.c

cc -pedantic -Wall -g -O2   -c -o server.o server.c

cc -o udp_server server.o demonize.o
