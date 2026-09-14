#include "HTTPServer.h" // Pamiętaj, aby plik nagłówkowy też był platform-agnostic

int main() {
    // 1. Inicjalizacja Winsock (tylko na Windows)
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup error\n");
        return 1;
    }
#endif

    // Używamy typu SOCKET (na Linuxie to zwykły int)
    SOCKET server_socket_fd, client_socket_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len;
    int socket_opt = 1;

    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == INVALID_SOCKET){
        perror("Socket initialization error");
    }

    // Rzutowanie (const char*) jest wymagane przez Windows API, na Linuxie zadziała bez problemu
    if(setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&socket_opt, sizeof(socket_opt)) < 0){
        perror("Socket option initialization error");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        perror("Binding error");
    }

    if(listen(server_socket_fd, LISTEN_BACKLOG) < 0){
        perror("Listen error");
    }
    
    client_address_len = sizeof(client_address);
    client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_address_len);

    if(client_socket_fd == INVALID_SOCKET){
        perror("Client socket error");
    }
    
    // Przykładowa pętla, uważaj, żeby nie zablokowała konsoli
    if(client_socket_fd != INVALID_SOCKET){
        printf("Klient podłączony! FD: %d\n", (int)client_socket_fd);
    }

    // Zamykanie gniazd (makro dopasowane do systemu)
    CLOSE_SOCKET(client_socket_fd);
    CLOSE_SOCKET(server_socket_fd);

    // Czyszczenie Winsock (tylko na Windows)
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}