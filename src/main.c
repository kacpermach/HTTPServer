#include "HTTPServer.h"

SOCKET server_socket_fd, client_socket_fd;
struct sockaddr_in server_address, client_address;
socklen_t client_address_len;
int socket_opt = 1;
char message_buffer[2048];
ssize_t message_size;

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup error\n");
        return 1;
    }
#endif

    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == INVALID_SOCKET){
        perror("Socket initialization error");
        return 0;
    }

    if(setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&socket_opt, sizeof(socket_opt)) < 0){
        perror("Socket option initialization error");
        return 0;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        perror("Binding error");
        return 0;
    }

    if(listen(server_socket_fd, LISTEN_BACKLOG) < 0){
        perror("Listen error");
        return 0;
    }
    while (1)
    {
        client_address_len = sizeof(client_address);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_address_len);

        if(client_socket_fd == INVALID_SOCKET){
            perror("Client socket error");
            break;
        } else {
            printf("Client connected FD: %d\n", (int)client_socket_fd);
        }
        message_size = recv(client_socket_fd, message_buffer, sizeof(message_buffer) - 1, 0);
        
        if(message_size < 0){
            perror("Recv error");
            return 0;
        } else if(message_size == 0){
            printf("Client disconnected\n");
        } else{
            message_buffer[message_size]='\0';
            printf("%s", message_buffer);   
            parse_message(message_buffer);
        }
        CLOSE_SOCKET(client_socket_fd);
    }
    CLOSE_SOCKET(server_socket_fd);
    
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}