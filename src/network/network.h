#ifndef NETWORK_H
#define NETWORK_H

#if (defined _WIN32 || defined _WIN64)
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#ifndef _WIN32
typedef int SOCKET;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

class UdpClient{
  public:
    UdpClient(const char *addr, int port);

    ~UdpClient();

    int send(void *data, int len);

    int recv(void *data,int len);

    bool listen();

  private:
    SOCKET create_socket();

    void close_socket(SOCKET);

    SOCKET fd_;
    struct sockaddr_in sock_addr_;
};

#endif /* NETWORK_H */
