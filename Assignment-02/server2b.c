#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 150

#define SA struct sockaddr
int main(int argc, char ** argv)
{

	int PORT = atoi(argv[1]);
	int sockfd, new_fd, len;
	struct sockaddr_in servaddr, cli;
	char buff[MAX];
	int n;
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fprintf(stderr, "Socket creation failed!\n");
		exit(0);
	}
	else
		printf("Socket creation successfull!\n");

	bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
		fprintf(stderr,"Socket bind failed!\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		fprintf(stderr,"Listen failed!\n");
		exit(0);
	}
	else
		printf("Server listening..\n");

	len = sizeof(cli);
	// Accept the data packet from client and verification
	new_fd = accept(sockfd, (SA *)&cli, &len);
	if (new_fd < 0)
	{
		fprintf(stderr,"Server acccept failed!\n");
		exit(0);
	}
	else
		printf("Accept Successfull!\n");

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	read(new_fd, buff, sizeof(buff));
	// print buffer which contains the client contents
	printf("Message from client: %s\n", buff);
	// After chatting close the socket

	printf("Message echoed to client: %s\n", buff);
	write(new_fd, buff, sizeof(buff));
	close(sockfd);
}
