#include "socket.h"

#define IP "127.0.0.1"
#define PORT 1234
int main() {
    SOCKET* sok = new_socket(IP, PORT, CLIENT);

    char* str = "56";
    send_to(sok, str, 3);

    printf("Connected\n");
    char s = ' ';
    while(scanf("%c", &s)) {
        if(s == 'y'){
            break;
        }
    }
    close_connection(sok);
    return 0;
}