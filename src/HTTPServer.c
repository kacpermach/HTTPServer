#include "HTTPServer.h"

int main(){
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket_fd < 0){
        perror("Socket initialization error\n");
    }

    if(setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_opt, sizeof(socket_opt)) < 0){
        perror("Socket option initialization error\n");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address))<0){
        perror("Binding error\n");
    }

    if(listen(server_socket_fd, LISTEN_BACKLOG) < 0){
        perror("Listen error\n");
    }
    client_address_len = sizeof(client_address);
    client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_address_len);

    if(client_socket_fd < 0){
        perror("Client socket error\n");
    }
    while(client_socket_fd){
        printf("%d\n", client_socket_fd);
    }
}