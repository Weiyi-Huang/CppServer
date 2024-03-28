#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"

#define BUFFER_SIZE 1024

int main()
{
    Socket *clnt_sock = new Socket();
    InetAddress *clnt_addr = new InetAddress("127.0.0.1", 8888);
    clnt_sock->Connect(clnt_addr);

    while (true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(clnt_sock->GetFd(), buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("Socket already disconnect, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(clnt_sock->GetFd(), buf, sizeof(buf));
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
            close(clnt_sock->GetFd());
            ErrorIf(true, "Socket read error!");
        }
    }
    delete clnt_sock;
    delete clnt_addr;
    return 0;
}