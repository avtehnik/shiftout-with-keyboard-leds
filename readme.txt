client.c

clientserver.h

demonize.c

demonize.h

Makefile

server.c

    -   Run the Makefile
 
# make
 

cc -pedantic -Wall -g -O2   -c -o client.o client.c

cc -o udp_client client.o

cc -pedantic -Wall -g -O2   -c -o demonize.o demonize.c

cc -pedantic -Wall -g -O2   -c -o server.o server.c

cc -o udp_server server.o demonize.o



sudo ./udp_server  1099

./udp_client  10.0.1.12 1099 7



// значение в val, выводятся начиная с младшего бита
for (size_t i=0; i<8; i++)
  printf("%c", '0' + ((val & (1<<i))?1:0));

// значение в val, выводятся начиная со старшего бита
for (size_t i=0; i<8; i++)
  printf("%c", '0' + ((val & (0x80>>i))?1:0));
