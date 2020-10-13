#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdbool.h>
#include<math.h>
#define MAXLINE 1024

#include "Hamming.h"

int main(int argc, char ** argv)
{
    srand(time(NULL));

    if (argc < 2){
        fprintf(stderr, "Enter port number as argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    int sockfd, newfd, n, arr[30], count = 0, bin;
    char buff[MAXLINE], buffer[MAXLINE], data_t[40];
    int i, j, r, total, nob, rem, dig, pos;
    long data;
    struct sockaddr_in servaddr,cliaddr;
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed!");
        exit(1);
    }
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    if(bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("Bind failed!");
        exit(1);
    }
    
    int len, m;
    listen(sockfd, 2);
    printf("Enter the data to send: ");
    scanf("%lu", &data);
    
    n = countbits(data);
    r = log2(n);
    r = floor(r);
    
    // Finding number of redundant bits
    while(pow(2, r) < n + r + 1)
        r += 1;
    
    
    printf("\nNo.of redundant bits required: %d\n", r);
    total = n + r;
    nob = floor(log2(total));
    for(i = 1; i <= total; i++)
    {
        dig = data % 10;
        if(isapower2(i) == 0)
        {
            arr[total - i] = dig;
            data /= 10;
        }
        else
            arr[total-i]=0;
    }
    for(i = 0; i < r; i++)
    {
        for(j = 1; j <= total; j++)
        {
            if((int)(pow(2, i)) != j)
            {
                bin = binary(j);
                if(ispresent(bin, i + 1))
                    count += arr[total - j];
            }
        }
        if(count % 2 == 0)
            arr[total - (int)(pow(2, i))] = 0;
        else
            arr[total - (int)(pow(2, i))] = 1;
        count = 0;
    }
    printf("\nData with redundant bits: ");
    for(i = 0; i < total; i++)
        printf("%d", arr[i]);
    // printf("\nEnter error position: ");
    // scanf("%d", &pos);

    pos = rand() % total + 1;
    printf("\nIntroducing error automatically at bit: %d\n", pos);

    if(arr[total - pos] == 0)
        arr[total - pos] = 1;
    else
        arr[total - pos] = 0;

    int k = 0;
    long num = 0;
    for(i = total - 1; i >= 0; i--)
    {
        num += pow(10, k) * arr[i];
        k++;
    }
    sprintf(data_t, "%lu", num);
    printf("Data transmitted is %s\n", data_t);
    
    len = sizeof(cliaddr);
    newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    m = write(newfd, data_t, sizeof(data_t));
}