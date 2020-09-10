#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define MAX 256
#define CLIENT 5
#define SA struct sockaddr

int fd_max(const int arr[CLIENT], int sock_fd, fd_set *readfds)
{
    //Finding the largest file descriptor, required for select()

    int max = -1;
    for (int i = 0; i < CLIENT; ++i)
    {
        max = (arr[i] > max) ? arr[i] : max;

        // If it is valid, add it to set of descriptors
        if (arr[i] > 0)
            FD_SET(arr[i], readfds);
    }

    max = (sock_fd > max) ? sock_fd : max;

    return max + 1;
}

int main(int argc, char **argv)
{
    if(argc < 2){
        fprintf(stderr, "Please pass port number as second argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    int sockfd, new_fd[CLIENT] = {0}, len;
    struct sockaddr_in servaddr, cli;
    char buff[MAX];

    // Setting timeouts for select()
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    // Tracking activity on descriptors
    fd_set readfds;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, "Socket creation failed!\n");
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Socket bind failed!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully bound\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, CLIENT)) != 0)
    {
        fprintf(stderr, "Listen failed!\n");
        exit(1);
    }
    else
        printf("Server listening for %d clients!\n", CLIENT);

    len = sizeof(cli);

    bzero(new_fd, sizeof(int) * CLIENT);
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        // Check for activity on server socket descriptor (for new connections)
        int activity = select(sockfd+1, &readfds, NULL, NULL, &tv);

        if (activity < 0){
            printf("Error in select()!\n");
            exit(EXIT_FAILURE);
        }

        // New connection detected
        if (FD_ISSET(sockfd, &readfds))
        {
            int client = accept(sockfd, (struct sockaddr *)&cli, &len);
            if (client < 0)
            {
                fprintf(stderr, "Accept error!\n");
                exit(1);
            }

            for (int i = 0; i < CLIENT; i++)
                if (new_fd[i] == 0)
                {
                    new_fd[i] = client;
                    break;
                }
            FD_CLR(sockfd, &readfds);
        }


        int limit = fd_max(new_fd, sockfd, &readfds);

        // Query for activity on existing clients (new message obtained)
        activity = select(limit, &readfds, NULL, NULL, &tv);

        if (activity < 0){
            printf("Error in select()!\n");
            exit(EXIT_FAILURE);
        }


        for (int i = 0; i < CLIENT; i++)
        {
            if (new_fd[i] < 0)
                continue;

            // Message from client 
            if (FD_ISSET(new_fd[i], &readfds))
            {
                int count = read(new_fd[i], buff, MAX);
                
                // Client has terminated
                if (strcmp(buff, "END") == 0)
                {
                    close(new_fd[i]);
                    new_fd[i] = 0;
                    printf("Client %d disconnected!\n", i);
                }

                else
                {
                    printf("Client %d: %s \n", (i+1), buff);
                    bzero(buff, MAX);
                    printf("Server: ");
                    scanf("%[^\n]", buff);
                    getchar();

                    if(strcmp(buff,"KILL") == 0) exit(EXIT_SUCCESS);
                    
                    // Write response to client
                    write(new_fd[i], buff, MAX);
                }

            }
        }
    }

    // Close the socket
    close(sockfd);
}
