CC = g++
CFLAGS = -g -Wall -ansi


all: devmanc devmand devmand-cli

devmanc: devmanc.c
	$(CC) $(CFLAGS) devmanc.c -o devmanc
devmand: devmand.c
	$(CC) $(CFLAGS) devmand.c -o devmand
devmand-cli: devmand-cli.c
	$(CC) $(CFLAGS) devmand-cli.c -o devmand-cli
clean:
	rm -f devmanc devmand

