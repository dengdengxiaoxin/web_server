//
// Created by dx on 2022/3/10.
//

#ifndef WEB_SERVER_WEB_H
#define WEB_SERVER_WEB_H
#include "../timer/lst_timer.h"
#include "../http_conn/http_conn.h"
#include "../thread_pool/threadpool.h"
#include "../socket/socket_server.h"
#include "../signal/signal_about.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <cassert>
extern const int max_event_number;

class web
{
public:
    web()=default;
    ~web()
    {
        delete pool;
        delete server;
        delete timer_lst;
        delete [] users;
    }
    void init( int close_log);
    void set(const int &port,const int &reuse,const int &time_slot);
    int epoll();
    void work(const int &num);
    int conn();
    int pipe_signal();
    void  read();
    void  write();
    void close_web();
private:
    threadpool<http_conn> * pool;
    http_conn *users;
    socket_server *server;
    sort_timer_lst *timer_lst;

    const int max_fd=65535;
    epoll_event events[10000];
    int m_epoll_fd;
    int m_listen_fd;
    int m_close_log;
    int m_port;
    int m_time_slot;

};
extern bool time_out;
extern int sock_fd;
#endif //WEB_SERVER_WEB_H
