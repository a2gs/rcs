all: clean
	cc -o rcs rcs.c -Wall -g -Wextra

clean:
	-rm rcs
