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