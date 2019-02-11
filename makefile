CC = gcc
CFLAGS = -g -Wall -std=c11 -D_XOPEN_SOURCE=700

RM = rm -rf

all: clean
	$(CC) -o rcs rcs.c $(CFLAGS)

clean:
	-$(RM) rcs
