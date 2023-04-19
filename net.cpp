#include <net.h>
#include <constant.h>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


Net::Net() {
    // memset(this->buffer, 0, sizeof this->buffer);
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket == 0) {
        perror("socket failed");
        exit(CREAT_FAILURE);
    }
}

Net::~Net() {
    close(this->socket);
}

void Net::ServerInit(uint16_t port) {
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);
    
    if(bind(this->socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0) {
        perror("bind failed");
        exit(BIND_FAILURE);
    }
    else printf("bind success! Begin to listen port %d\n", port);

    if(listen(this->socket, MAX_CONNECT) < 0) {
        perror("listen failed");
        exit(LISTEN_FAILURE);
    }
}

void Net::ClientInit(const char * ip, uint16_t port) {
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = inet_addr(ip);
    this->address.sin_port = htons(port);

    if(connect(this->socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0) {
        perror("connect failed");
        exit(CONNECT_FAILURE);
    }
}

int Net::AcceptClient() {
    int addressLength = sizeof(this->address);
    int newSocket = accept(this->socket, (sockaddr *)&this->address, (socklen_t *)&addressLength);

    std::cout << "Accept client." << std::endl;

    if(newSocket < 0) {
        perror("accept failed");
        exit(ACCEPT_FAILURE);
    }

    return newSocket;
}

int Net::ReadSocket(int socket_num, char * buffer, ssize_t length) {
    int readnum = read(socket_num, buffer, length);
    return readnum;
}

int Net::ReadSocket(char * buffer, ssize_t length) {
    int readnum = read(this->socket, buffer, length);
    return readnum;
}

int Net::WriteSocket(int socket_num, char * buffer, ssize_t length) {
    int writenum = write(socket_num, buffer, length);
    return writenum;
}

int Net::WriteSocket(char * buffer, ssize_t length) {
    int writenum = write(this->socket, buffer, length);
    return writenum;
}

int Net::close(int socket_num) {
    std::cout << "Client closed." << std::endl;
    return close(socket_num);
}