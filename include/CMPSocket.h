//================================DOXYGEN================================================
//! \file       CMPSocket.h
//!
//! \mainpage
//! \brief      Библиотека глупого программиста для работы с сокетами
//!
//! \par        Описание
//!             CMPSocket - простой вариант сокета для работы и проб.
//!             Мало вреятно что тут будет много возможностей, здесь основа.
//!             Дальше думайте сами
//!
//! \version    0.0.1a
//!
//! \author     Copyright (C) TheCompiler (Artem Vetoshkin) <vetan22@mail.ru>, 1 курс
//! \date       10.2019
//=======================================================================================

#ifndef _CMPSOCKET_H_
#define _CMPSOCKET_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define LOCAL_HOST "127.0.0.1"

//=======================================================================================
//! @enum type_socket
//! @brief тип сокета
//---------------------------------------------------------------------------------------

enum type_socket {
    CMP_CLIENT,         //!< тип клиента
    CMP_SERVER,         //!< тип сервера
    CMP_EMPTY,          //!< пустой сокет
    CMP_BLOCKING,       //!< блокирующий сокет
    CMP_NONBLOCKING     //!< неблокирующий сокет
};

//=======================================================================================

typedef struct SOCKET      SOCKET;
typedef enum   type_socket type_socket;

//=======================================================================================
//! \brief структура сокета
//---------------------------------------------------------------------------------------

struct SOCKET {
    type_socket         type;      ///< тип сокета
    type_socket         work_type; ///< тип работы сокета (blocking, nonbloking)
    int                 sock;      ///< сам сокет
    int                 sock_to;   ///< сокет с которым общается наш
    struct sockaddr_in  sock_addr; ///< адрес сокета
};

//=======================================================================================
//! \brief создаёт новый сокет
//!
//! \param[in] host         ip по которому подключается или создаётся сокет
//! \param     port         порт по которому подключается или создаётся сокет
//! \param     type         тип сокета
//! \param     work_type    тип работы сокета (blocking, nonbloking)
//!
//! \return созданный сокет или NULL в случае ошибки
//!
//---------------------------------------------------------------------------------------

SOCKET* new_socket(const char *host, int port, type_socket type, type_socket work_type);

//---------------------------------------------------------------------------------------
//! \brief Функция сервера поиска клиентов
//!
//! \param[in] server   сокет сервера
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//---------------------------------------------------------------------------------------

int found_connect(SOCKET* server);

//---------------------------------------------------------------------------------------
//! \brief закрывает сокет
//!
//! \param[in] sock   сокет
//!
//---------------------------------------------------------------------------------------

void close_connection(SOCKET* sock);

//---------------------------------------------------------------------------------------
//! \brief Отправлает строку в сокет
//!
//! \param[in] to           сокет, которому отправляем
//! \param[in] buff         сообщение
//! \param     buff_size    размер сообщения
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//---------------------------------------------------------------------------------------

int send_to(SOCKET* to, void* buff, size_t buff_size);

//---------------------------------------------------------------------------------------
//! \brief Принимает сообщение от сокета
//!
//! \param[in]  from         сокет, от которого ожидается сообщение
//! \param[out] buff         сообщение
//! \param      buff_size    размер сообщения
//!
//! \return 0 в случае успеха, -1 в случае ошибки
//!
//---------------------------------------------------------------------------------------

int get_from(SOCKET* from, char* buff, size_t buff_size);

#endif // _CMPSOCKET_H_