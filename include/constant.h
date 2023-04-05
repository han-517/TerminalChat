#include <errno.h>
#include <stdint.h>

#define PORT                12345
#define IP                  "43.142.162.30"

#define SUCCESS             (u_int32_t)0
#define FAILED              (u_int32_t)1

#define CREAT_FAILURE       (u_int32_t)15
#define BIND_FAILURE        (u_int32_t)16
#define LISTEN_FAILURE      (u_int32_t)17
#define CONNECT_FAILURE     (u_int32_t)18
#define ACCEPT_FAILURE      (u_int32_t)19
#define SEND_FAILURE        (u_int32_t)20
#define READ_FAILURE        (u_int32_t)21

enum TC_FLAG {
    TC_MESSAGE            = (uint8_t)1 >> 1,
    TC_FILE               = (uint8_t)1 >> 2,
    TC_TOPIC              = (uint8_t)1 >> 3,
    TC_END                = (uint8_t)1 >> 4
};