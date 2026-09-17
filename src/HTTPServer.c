#include "HTTPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int HTTP_method = 0;

bool recognise_file_path(const char *file_path) {
    if (strncmp(file_path, "./sites/", 8) != 0) {
        return false;
    }

    if (strstr(file_path, "..") != NULL) {
        return false;
    }
    
    return true;
}

void open_file_path(char *file_path){
    FILE *file = fopen(file_path, "rb");

    if (file == NULL) {
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

void parse_message(char *message_buffer){
    char method[16];
    char path[1024];
    char protocol[16];
    
    sscanf(message_buffer, "%15s %1023s %15s", method, path, protocol);

    if (strcmp(method, "GET") == 0){
        HTTP_method = GET;
    }
    else if (strcmp(method, "POST") == 0) {
        HTTP_method = POST;
    }
    
    switch (HTTP_method) {
        case GET: {
            char file_path[2048];
            
            if (strcmp(path, "/") == 0) {
                strcpy(file_path, "./sites/index.html");
            } else {
                snprintf(file_path, sizeof(file_path), ".%s", path);
            }

            if (recognise_file_path(file_path)) {
                open_file_path(file_path);
            } else {
                const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
                send(client_socket_fd, not_found, (int)strlen(not_found), 0);
            }
            break;
        }
        
        case POST:
            printf("POST request received\n");
            break;

        default:
            printf("Unknown HTTP method\n");
            break;
    }
}