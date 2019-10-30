//================================DOXYGEN================================================
//! \file       CMPSocket.c
//! \brief      Описание всех функции билиотеки
//=======================================================================================

#include "CMPSocket.h"

//---------------------------------------------------------------------------------------

SOCKET* new_socket(const char *host, int port, type_socket type, type_socket work_type) {
    SOCKET* new_sock = (SOCKET*) calloc(1, sizeof(SOCKET));
    memset(&new_sock->sock_addr, 0, sizeof(struct sockaddr_in));

    new_sock->type                  = type;
    new_sock->work_type             = work_type;
    new_sock->sock_addr.sin_family  = AF_INET;
    new_sock->sock_addr.sin_port    = htons(port);

    if (inet_pton(AF_INET, host, &new_sock->sock_addr.sin_addr) <= 0) {
        fprintf(stderr, "Can't parse host address %s\n", host);
        close_connection(new_sock);
        return NULL;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Can't create a socket: %s\n", strerror(errno));
        close_connection(new_sock);
        return NULL;
    }

    int reuse = 1;
    switch (type) {
    case CMP_SERVER:
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        if (bind(sock, (struct sockaddr*)&new_sock->sock_addr,
                 sizeof(new_sock->sock_addr)) < 0) {
            fprintf(stderr, "Can't bind to %s:%d: %s\n", host, port, strerror(errno));
            close_connection(new_sock);
            return NULL;
        }

        if (listen(sock, 10) < 0) {
            fprintf(stderr, "Can't start listening on %s:%d: %s\n",
                    host, port, strerror(errno));
            close_connection(new_sock);
            return NULL;
        }
        new_sock->sock = sock;
        break;
    case CMP_CLIENT:
        if (connect(sock, (struct sockaddr *)&new_sock->sock_addr,
                    sizeof(new_sock->sock_addr)) < 0) {
            fprintf(stderr, "can't connect to %s:%d: %s\n", host, port, strerror(errno));
            close_connection(new_sock);
            return NULL;
        }
        new_sock->sock_to = sock;
        break;
    case CMP_EMPTY:
        new_sock->sock = sock;
        break;
    default:
        fprintf(stderr, "Error type socket: %d", type);
        close_connection(new_sock);
        return NULL;
    }

    if (work_type == CMP_NONBLOCKING) {
        fcntl(sock, F_SETFL, O_NONBLOCK);
    }

    return new_sock;
}

//---------------------------------------------------------------------------------------

int found_connect(SOCKET* server) {
    assert(server != NULL);

    if (server->type != CMP_SERVER) {
        printf("This socket isn't server\n");
        return -1;
    }

    struct sockaddr_in addr_client;
    memset(&addr_client, 0, sizeof(addr_client));

    socklen_t sz = sizeof(addr_client);
    int sock = accept(server->sock, (struct sockaddr *)&addr_client, &sz);
    if (sock < 0) {
        if (server->work_type == CMP_BLOCKING) {
            printf("Can't accept: %s\n", strerror(errno));
        }
        return -1;
    }

    server->sock_to = sock;

    return 0;
}

//---------------------------------------------------------------------------------------

void close_connection(SOCKET* sock) {
    if (sock == NULL) {
        return;
    }

    close(sock->sock);
    close(sock->sock_to);
    free(sock);
}

//---------------------------------------------------------------------------------------

int send_to(SOCKET* to, void* buff, size_t buff_size) {
    assert(to   != NULL);
    assert(buff != NULL);

    switch (to->type) {
    case CMP_CLIENT:
    case CMP_SERVER:
        if (send(to->sock_to, buff, buff_size, 0) == -1) {
            printf("Can't send message: %s", strerror(errno));
            return -1;
        }
        break;
    case CMP_EMPTY:
        if (sendto(to->sock_to, buff, buff_size, 0,
                   (struct sockaddr *) &to->sock_addr, sizeof (to->sock_addr)) == -1) {
            printf("Can't send message: %s", strerror(errno));
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------------------

int get_from(SOCKET* from, char* buff, size_t buff_size) {
    assert(from != NULL);
    assert(buff != NULL);

    socklen_t size = sizeof(from->sock_addr);

    switch (from->type) {
    case CMP_CLIENT:
    case CMP_SERVER:
        if (recv(from->sock_to, buff, buff_size, 0) == -1) {
            printf("Can't send message: %s", strerror(errno));
            return -1;
        }
        break;
    case CMP_EMPTY:
        if (recvfrom(from->sock_to, buff, buff_size, 0,
                     (struct sockaddr *) &from->sock_addr, &size) == -1) {
            printf("Can't send message: %s", strerror(errno));
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}