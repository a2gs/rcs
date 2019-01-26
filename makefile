all: clean
	cc -o rcs rcs.c -Wall -g

clean:
	-rm rcs
