#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#define MAXLINE 1024

#include "Hamming.h"

int main(int argc, char **argv)
{
    if (argc < 2){
        fprintf(stderr, "Please enter port number as second argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    long num;
    int sockfd, total, i, rem, arr[20], count = 0, r = 0, result = 0, bin, j, newarr[20], finalarr[20];
    char buffer1[40];
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed!");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int n, len;
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    n = read(sockfd, buffer1, sizeof(buffer1));
    num = atol(buffer1);
    total = countbits(num);
    printf("Received data: %lu\n", num);
    i = 1;
    while(num > 0)
    {
        rem = num % 10;
        arr[total - i] = rem;
        num /= 10;
        i++;
    }
    for(i = 1; i <= total; i++)
        if(ceil(log2(i)) == floor(log2(i)))
            r++;
    int k = 0;
    for(i = 0; i < 4; i++)
    {
        for(j = 1; j <= total; j++)
        {
            bin = binary(j);
            if(ispresent(bin, i + 1))
                count += arr[total - j];
        }
        if(count % 2 == 0)
            result += pow(10, k) * 0;
        else
            result += pow(10, k) * 1;
        k++;
        count=0;
    }
    int error = decimal(result);
    printf("\nError bit in binary: %d\n", result);
    printf("\nError in bit %d\n", error);
    if(arr[total - error] == 0)
        arr[total - error] = 1;
    else
        arr[total - error] = 0;
    k = 0;
    printf("\nData after error correction: ");
    for(i = total - 1; i >= 0; i--)
    {
        newarr[k] = arr[i];
        k++;
    }
    int x = 0;
    for(i = 0;i < k; i++)
    {
        if(ceil(log2(i + 1)) != floor(log2(i + 1)))
        {
            finalarr[x] = newarr[i];
            x++;
        }
    }
    for(i = x - 1; i >= 0; i--)
        printf("%d", finalarr[i]);
    printf("\n");
    return 0;
}