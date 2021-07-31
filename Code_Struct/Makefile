CC = g++
CFLAGS = -g -Wall -ansi


all: echo_client echo_epollserv

echo_client: echo_client.c
	$(CC) $(CFLAGS) echo_client.c -o echo_client
echo_epollserv: echo_epollserv.c
	$(CC) $(CFLAGS) echo_epollserv.c -o echo_epollserv

clean:
	rm -f echo_client echo_epollserv

