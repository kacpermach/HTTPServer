#ifdef _WIN32
    #include <stdio.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef int socklen_t;
    #define CLOSE_SOCKET closesocket
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>

    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET close
#endif

#define PORT 8080
#define LISTEN_BACKLOG 50
#define GET 1
#define POST 2

extern SOCKET server_socket_fd, client_socket_fd;
extern struct sockaddr_in server_address, client_address;
extern socklen_t client_address_len;
extern int socket_opt;
extern char message_buffer[2048];
extern ssize_t message_size;
extern int HTTP_method;

void parse_message(char *message_buffer);
void open_file_path(char *file_path);