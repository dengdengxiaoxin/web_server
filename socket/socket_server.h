//
// Created by dx on 2022/3/10.
//

#ifndef WEB_SERVER_SOCKET_SERVER_H
#define WEB_SERVER_SOCKET_SERVER_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
class socket_server
{
public:
    socket_server()=default;
    int init(const int &port);
    void build_bind(const int &reuse);
    int accept_conn();
    void close_fd();

    sockaddr_in client_addr;

private:
    sockaddr_in server_addr;
    int server_port;
    int listen_fd;
    int conn_fd;

};
#endif //WEB_SERVER_SOCKET_SERVER_H
