cc -pedantic -Wall -g -O2   -c -o demonize.o demonize.c
cc -pedantic -Wall -g -O2   -c -o server.o server.c
cc -o server server.o demonize.o
