#ifndef SERVER_H
#define SERVER_H

#include <net.h>

#include <string>
#include <vector>
#include <unordered_map>

#define MESSAGE_BODY    1024
#define MESSAGE_HEADER  37
#define MESSAGE_MAX     (MESSAGE_BODY + MESSAGE_HEADER)

class Server
{
    using UUID_SOCKET_MAP   = std::unordered_map<std::string, int>;
    using TOPIC_VECTOR_MAP  = std::unordered_map<std::string, std::vector<int>>;

    struct Message
    {
        uint8_t         flag;
        std::string     des;
        std::string     body;
    };
    

private:
    /* data */
    Net*                 server;
    UUID_SOCKET_MAP*     clientMap;
    TOPIC_VECTOR_MAP*    topicMap;
    void buffer2Message(char *, Message *);

public:
    Server();
    ~Server();

    void AccessRequests();

};

#endif // SERVER_H