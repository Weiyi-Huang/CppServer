#include <iostream>
#include <unistd.h>
#include <cstring>
#include <functional>
#include "source/util.h"
#include "source/Buffer.h"
#include "source/Socket.h"
#include "source/ThreadPool.h"
using namespace std;

void OneClient(int msgs, int wait);

int main(int argc, char *argv[])
{
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char *optstring = "t:m:w:";

    while ((o = getopt(argc, argv, optstring)) != -1)
    {
        switch (o)
        {
        case 't':
            printf("opt is t, oprarg is: %s\n", optarg);
            threads = stoi(optarg);
            break;
        case 'm':
            printf("opt is m, oprarg is: %s\n", optarg);
            msgs = stoi(optarg);
            break;
        case 'w':
            printf("opt is w, oprarg is: %s\n", optarg);
            wait = stoi(optarg);
            break;
        case '?':
            printf("Error optopt: %c\n", optopt);
            printf("Error opterr: %d\n", opterr);
            break;
        }
    }

    ThreadPool *pool = new ThreadPool(threads);
    std::function<void()> func = std::bind(OneClient, msgs, wait);
    for (int i = 0; i < threads; ++i)
    {
        pool->Add(func);
    }
    delete pool;
    return 0;
}

void OneClient(int msgs, int wait)
{
    Socket *clnt_sock = new Socket();
    InetAddress *clnt_addr = new InetAddress("127.0.0.1", 1234);
    clnt_sock->Connect(clnt_addr);

    int sockfd = clnt_sock->GetFd();

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    sleep(wait);
    int count = 0;
    while (count < msgs)
    {
        sendBuffer->SetBuf("I'm client!");
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
                printf("Count: %d, message from server: %s\n", count++, readBuffer->C_str());
                break;
            }
        }
        readBuffer->Clear();
    }   
    delete clnt_sock;
    delete clnt_addr;
}
