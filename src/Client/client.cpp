#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(sockfd == -1, "Socket create error!");

    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    ErrorIf(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "Socket connect error!");

    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("Socket already disconnect, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("Message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("Server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
            ErrorIf(true, "Socket read error!");
        }
    }
    close(sockfd);
    return 0;
}