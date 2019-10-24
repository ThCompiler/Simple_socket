#include "socket.h"

#define IP "127.0.0.1"
#define PORT 1234

int main() {
    SOCKET* sok = new_socket(IP, PORT, SERVER);

    found_connect(sok);
    printf("Connect\n");

    char* str = calloc(3, 1);
    get_from(sok, str, 3);

    printf("%s\n", str);
    char s = ' ';
    while (scanf("%c", &s)) {
        if (s == 'y') {
            break;
        }
    }
    close_connection(sok);
    return 0;
}