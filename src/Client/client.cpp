#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "util.h"
#include "Buffer.h"
#include "Socket.h"
#include "InetAddress.h"

int main()
{
    Socket *clnt_sock = new Socket();
    InetAddress *clnt_addr = new InetAddress("127.0.0.1", 1234);
    clnt_sock->Connect(clnt_addr);

    int sockfd = clnt_sock->GetFd();

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    while (true)
    {
        sendBuffer->Getline();
        ssize_t write_bytes = write(sockfd, sendBuffer->C_str(), sendBuffer->Size());
        if (write_bytes == -1)
        {
            printf("Socket already disconnect, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];
        while (true)
        {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0)
            {
                readBuffer->Append(buf, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0)
            {
                printf("Server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->Size())
            {
                printf("Message from server: %s\n", readBuffer->C_str());
                break;
            }
        }
        readBuffer->Clear();
    }
    delete clnt_sock;
    delete clnt_addr;
    return 0;
}