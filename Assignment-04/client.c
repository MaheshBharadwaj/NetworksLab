#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 1024
#define SA struct sockaddr
int main(int argc, char ** argv)
{


	 if(argc < 2){
        fprintf(stderr, "Please pass port number of server as second argument!\n");
        exit(EXIT_FAILURE);
    }
	int PORT = atoi(argv[1]);

	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	char buff[MAX] = {0},
	     filename[30] = {0};

	int n; // socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fprintf(stderr,"Socket creation failed!\n");
		exit(0);
	}
	else
		printf("Socket creation successfull!\n");

	bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		fprintf(stderr,"Connection failed!\n");
		exit(0);
	}
	else
		printf("Connection to server successfull!\n");

	printf("Enter the path of the file: ");
	scanf("%[^\n]", buff);
	getchar();
	write(sockfd, buff, MAX);
	read(sockfd, buff, MAX);

	if(strcmp(buff, "File Not Found!")==0){
		fprintf(stderr, "%s\n", buff);
		exit(EXIT_FAILURE);
	}

	else{
		printf("File Transferred\nEnter the path to save: ");
		scanf("%[^\n]", filename);
		int fd = creat(filename,S_IRWXU);

		if (fd < 0){
			fprintf(stderr, "Unable to create file!\n");
			exit(EXIT_FAILURE);
		}

		write(fd, buff, strlen(buff));
		close(fd);
	}

	close(sockfd);
}