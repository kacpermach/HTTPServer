#include "HTTPServer.h"

void parse_message(char *message_buffer){
    printf("Function\n");
    char method[16];
    char path[1024];
    char protocol[16];
    sscanf(message_buffer, "%s %c %s\n", method,path,protocol);
    printf("%s\n",method);
    printf("%s\n",path);
    printf("%s\n",protocol);
}