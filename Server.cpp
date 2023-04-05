#include <Server.h>
#include <uuid.hpp>
#include <constant.h>

#include <iostream>
#include <thread>

Server::Server()
{
    this->server = new Net();
    this->server->ServerInit(PORT);
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
            this->clientMap[uuid] = client;

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

                uint8_t flag = (uint8_t)buffer[0];
                std::string str(strndup(buffer+1, 36));

                if(flag & TC_TOPIC) {
                    std::vector<int> aim;
                    for(auto tpc : this->topicMap) {
                        if(str == tpc.first) {
                            aim = tpc.second;
                            break;
                        }
                    }
                    if(!aim.size()) {
                        this->topicMap[str].push_back(client);
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
                    for(auto u : this->clientMap) {
                        if(u.first == str) {
                            exist = true;
                            toClient = u.second;
                        }
                    }
                    if(exist) {
                        this->server->WriteSocket(toClient, buffer, valread);
                    } else {
                        this->server->WriteSocket(client, "NotExist", 8);
                    }
                }
            }
        });
        t.detach();
    }

}