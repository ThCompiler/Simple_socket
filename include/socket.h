//! @file
//! \brief Простая реализация сокета
//! @mainpage
//-----------------------------------------------------------------------------------------------------------------
//! \author     TheCompiler
//! \version    alpha 1
//! \date       25.10.19
//-----------------------------------------------------------------------------------------------------------------


#ifndef _SOCKET_LIB_H_
#define _SOCKET_LIB_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

typedef struct SOCKET SOCKET;

//-----------------------------------------------------------------------------------------------------------------
//! \brief тип сокета
//-----------------------------------------------------------------------------------------------------------------

enum type_socket {
    CLIENT,         ///< тип клиента
    SERVER,         ///< тип сервера
    EMPTY           ///< пустой сокет
};

//-----------------------------------------------------------------------------------------------------------------
//! \brief структура сокета
//-----------------------------------------------------------------------------------------------------------------

struct SOCKET {
    enum type_socket    type;      ///< тип сокета
    struct sockaddr_in  sock_addr; ///< адрес сокета
    int                 sock;      ///< сам сокет
    int                 sock_to;   ///< сокет с которым общается наш
};

//-----------------------------------------------------------------------------------------------------------------
//! \brief создаёт новый сокет
//!
//! \param[in] host   ip по которому подключается или создаётся сокет
//! \param     port   порт по которому подключается или создаётся сокет
//! \param     type   тип сокета
//!
//! \return созданный сокет или NULL в случае ошибки
//!
//-----------------------------------------------------------------------------------------------------------------

SOCKET* new_socket(const char *host, int port, enum type_socket type);

//-----------------------------------------------------------------------------------------------------------------
//! \brief Функция сервера поиска клиентов
//!
//! \param[in] server   сокет сервера
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//-----------------------------------------------------------------------------------------------------------------

int found_connect(SOCKET* server);

//-----------------------------------------------------------------------------------------------------------------
//! \brief закрывает сокет
//!
//! \param[in] sock   сокет
//!
//-----------------------------------------------------------------------------------------------------------------

void close_connection(SOCKET* sock);

//-----------------------------------------------------------------------------------------------------------------
//! \brief Отправлает строку в сокет
//!
//! \param[in] to           сокет, которому отправляем
//! \param[in] buff         сообщение
//! \param     buff_size    размер сообщения
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//-----------------------------------------------------------------------------------------------------------------

int send_to(SOCKET* to, void* buff, size_t buff_size);

//-----------------------------------------------------------------------------------------------------------------
//! \brief Принимает сообщение от сокета
//!
//! \param[in]  from         сокет, от которого ожидается сообщение
//! \param[out] buff         сообщение
//! \param      buff_size    размер сообщения
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//-----------------------------------------------------------------------------------------------------------------

int get_from(SOCKET* from, char* buff, size_t buff_size);

#endif // _SOCKET_LIB_H_