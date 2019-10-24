//! \file
//-----------------------------------------------------------------------------------------------------------------

#include "socket.h"

//-----------------------------------------------------------------------------------------------------------------

SOCKET* new_socket(const char *host, int port, enum type_socket type) {
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &sock_addr.sin_addr) <= 0) {
        printf("Can't parse host address %s\n", host);
        return NULL;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Can't create a socket: %s\n", strerror(errno));
        return NULL;
    }

    int reuse = 1;

    switch (type) {
    case SERVER:
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        if (bind(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
            printf("Can't bind to %s:%d: %s\n", host, port, strerror(errno));
            return NULL;
        }

        if (listen(sock, 10) < 0) {
            printf("Can't start listening on %s:%d: %s\n", host, port, strerror(errno));
            return NULL;
        }
        break;
    case CLIENT:
        if (connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
            fprintf(stderr, "can't connect to %s:%d: %s\n", host, port, strerror(errno));
            return NULL;
        }
        break;
    default:
        break;
    }

    SOCKET* new_sock = calloc(1, sizeof(SOCKET));
    new_sock->sock_addr = sock_addr;
    new_sock->type = type;

    if (new_sock->type == CLIENT) {
        new_sock->sock_to = sock;
    }
    else {
        new_sock->sock = sock;
    }

    return new_sock;
}

//-----------------------------------------------------------------------------------------------------------------

int found_connect(SOCKET* server) {
    assert(server != NULL);

    if (server->type != SERVER) {
        printf("This socket isn't server\n");
        return -1;
    }

    struct sockaddr_in addr_client;
    memset(&addr_client, 0, sizeof(addr_client));

    socklen_t sz = sizeof(addr_client);
    int sock = accept(server->sock, (struct sockaddr *)&addr_client, &sz);
    if (sock < 0) {
        printf("Can't accept: %s\n", strerror(errno));
        return -1;
    }

    server->sock_to = sock;

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------

void close_connection(SOCKET* sock) {
    if (sock == NULL) {
        return;
    }

    close(sock->sock);
    close(sock->sock_to);
    free(sock);
}

//-----------------------------------------------------------------------------------------------------------------

int send_to(SOCKET* to, char* buff, size_t buff_size) {
    assert(to   != NULL);
    assert(buff != NULL);

    if (write(to->sock_to, buff, buff_size) == -1) {
        printf("Can't send message: %s", strerror(errno));
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------

int get_from(SOCKET* from, char* buff, size_t buff_size) {
    assert(from != NULL);
    assert(buff != NULL);

    int    wroten       = 0;
    size_t total_wroten = 0;
    char   ch           = '\0';

    while (1) {
        wroten = read(from->sock_to, &ch, 1);

        if (wroten == -1) {
            return -1;
        }
        else if (wroten == 0) {
            if (total_wroten == 0) {
                return 0;
            }
            else {
                break;
            }
        }
        else {
            assert(wroten == 1);
            if (total_wroten < buff_size - 1) {
                total_wroten++;
                *(buff++) = ch;
            }

            if (ch == '\n' || total_wroten >= buff_size - 1) {
                break;
            }
        }
    }

    buff[buff_size - 1] = '\0';

    return 0;
}