#ifdef __APPLE__
    #include <stdio.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif

#define PORT 8080
#define LISTEN_BACKLOG 50

int server_socket_fd, client_socket_fd;
struct sockaddr_in server_address, client_address;
socklen_t client_address_len;
int socket_opt = 1;