#include "network.h"
#include <fcntl.h> 


UdpClient::UdpClient(const char *addr, int port)
{
    fd_ = create_socket();

    sock_addr_.sin_family = AF_INET;
    if (NULL == addr)
    {
    sock_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
    sock_addr_.sin_addr.s_addr = inet_addr(addr);
    }
    sock_addr_.sin_port = htons(port);
}

UdpClient::~UdpClient()
{
    close_socket(fd_);
}

int UdpClient::send(void *data, int len)
{
    return sendto(fd_, (const char*)data, len, 0, (struct sockaddr *)&sock_addr_, sizeof(sock_addr_));
}

int UdpClient::recv(void *data, int len)
{
#ifdef _WIN32
    int sin_len = sizeof(sock_addr_);
#else
    socklen_t sin_len = sizeof(sock_addr_);
#endif
    return recvfrom(fd_, (char*)data, len, 0, (struct sockaddr *)&sock_addr_, &sin_len);
}

bool UdpClient::listen()
{
#if (defined _WIN32 || defined _WIN64)
    /*
    unsigned long arg = 1;
    if (ioctlsocket(fd_, FIONBIO, &arg) == -1)
    {
        return false;
    }
    */
#else
    int flags = fcntl(fd_, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(fd_, F_SETFL, flags) == -1)
    {
        return false;
    }
#endif

    if (0 == bind(fd_, (struct sockaddr *)&sock_addr_, sizeof(sock_addr_)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

SOCKET UdpClient::create_socket()
{
    SOCKET fd;
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == INVALID_SOCKET)
    {
        return fd;
    }
    return fd;
}

void UdpClient::close_socket(SOCKET fd)
{
#if (defined _WIN32 || defined _WIN64)
    if (closesocket(fd) == SOCKET_ERROR)
    {
        return;
    }
#else
    if (close(fd) == SOCKET_ERROR)
    {
        return;
    }
#endif
}
