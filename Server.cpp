#include <Server.h>
#include <uuid.hpp>
#include <constant.h>

#include <iostream>
#include <thread>

Server::Server()
{
    this->server = new Net();
    this->server->ServerInit(PORT);

    this->clientMap = new UUID_SOCKET_MAP;
    this->topicMap  = new TOPIC_VECTOR_MAP;
}

Server::~Server()
{
    delete this->server;
}

void Server::AccessRequests() {
    while(true) {
        int client = this->server->AcceptClient();
        std::thread t([&]() {
            std::string uuid = generate_uuid();
            this->clientMap->operator[](uuid) = client;

            while(true) {
                char buffer[MESSAGE_MAX];
                ssize_t valread = this->server->ReadSocket(client, buffer, sizeof(buffer));
                if(valread == 0) {
                    break;
                }
                if(valread < 0) {
                    perror("read error");
                    exit(READ_FAILURE);
                }

                Message * message = new Message();
                buffer2Message(buffer, message);
                std::cout << "Receive message, flag: " << (int)message->flag << ", des: " << message->des << ", message: " << message->body << std::endl;

                if(message->flag & TC_TOPIC) {
                    std::vector<int> aim;
                    for(auto tpc = this->topicMap->begin(); tpc != this->topicMap->end(); ++tpc) {
                        if(message->des == tpc->first) {
                            aim = tpc->second;
                            break;
                        }
                    }
                    if(!aim.size()) {
                        this->topicMap->operator[](message->des).push_back(client);
                    }
                    else {
                        bool exist = false;
                        for(auto toClient : aim) {
                            if(toClient == client) {
                                exist = true;
                                continue;
                            }
                            this->server->ReadSocket(toClient, buffer, valread);
                        }
                    }
                    
                }
                else {
                    bool exist = false;
                    int toClient;
                    for(auto u = this->clientMap->begin(); u != this->clientMap->end(); u++) {
                        if(u->first == message->des) {
                            exist = true;
                            toClient = u->second;
                        }
                    }
                    if(exist) {
                        this->server->WriteSocket(toClient, buffer, valread);
                    } else {
                        strcpy(buffer, "NotExist");
                        this->server->WriteSocket(client, buffer, 8);
                    }
                }
            }
        });
        t.detach();
    }

}

void Server::buffer2Message(char * buffer, Server::Message * message) {
    message->flag = (uint8_t)buffer[0];
    message->des = strndup(buffer+1, MESSAGE_HEADER-1);
    message->body = strndup(buffer+MESSAGE_HEADER, MESSAGE_BODY);
}