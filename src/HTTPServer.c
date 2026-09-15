#include "HTTPServer.h"

void parse_message(char *message_buffer){
    printf("Function\n");
    char method[16];
    char path[1024];
    char protocol[16];
    sscanf(message_buffer, "%s %c %s\n", method, path, protocol);

    char file_path[2048];
    if(strcmp(path, "/")==0){
        strcpy(file_path, "index.html");
    } else{
        snprintf(file_path, sizeof(file_path), ".%s", path);
    }
    open_file_path(file_path);
    
}


void open_file_path(char *file_path){
    FILE *file = fopen(file_path, "rb");

    if(file == NULL){
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
        send(client_socket_fd, not_found, (int)strlen(not_found), 0);
    } else {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *file_buffer = malloc(file_size);
        fread(file_buffer, 1, file_size, file);
        fclose(file);

        char header[512];
        int header_len = snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n", file_size);

        send(client_socket_fd, header, header_len, 0);

        send(client_socket_fd, file_buffer, file_size, 0);

        free(file_buffer);
    }
}