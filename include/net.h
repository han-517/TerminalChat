#ifndef TC_NET_H
#define TC_NET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

constexpr int MAX_CONNECT = 6;

class Net
{
private:
    int socket;
    struct sockaddr_in address;

public:
    Net();
    ~Net();

    void ClientInit(const char * ip, uint16_t port);
    void ServerInit(uint16_t port);

    int AcceptClient();
    int ReadSocket(char * buffer, ssize_t length);
    int ReadSocket(int socket_num, char * buffer, ssize_t length);
    int WriteSocket(char * buffer, ssize_t length);
    int WriteSocket(int socket_num, char * buffer, ssize_t length);

    int close(int socket_num);
};


#endif