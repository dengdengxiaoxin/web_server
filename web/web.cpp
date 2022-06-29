//
// Created by dx on 2022/3/10.
//

#include "web.h"

//void cb_func(http_conn* user_data)
//{
//    user_data->close_conn();
//}
int sock_fd=0;
bool time_out;
const int max_event_number=10000;
void web::init(int close_log)
{
    add_sig(SIGPIPE,SIG_IGN);
    try
    {
        pool = new threadpool<http_conn>;
    }catch (...)
    {
        exit(-1);
    }
    if(close_log==0)
    {
        m_close_log=0;
        log::get_instance()->init("server_log",0,2000,80000,0);
    }
    else
    {
        m_close_log=1;
    }
    users=new http_conn[max_fd];
    http_conn::connpool_init();
    server=new socket_server;
    timer_lst=new sort_timer_lst;
}
void web::set(const int &port,const int &reuse,const int &time_slot)
{
    m_listen_fd=server->init(port);
    m_port=port;
    m_time_slot=time_slot;
    server->build_bind(reuse);
    m_epoll_fd= epoll_create(5);
    add_fd(m_epoll_fd,m_listen_fd, false);      // 添加到epoll对象中
    http_conn::m_epoll_fd=m_epoll_fd;
    int ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipe_fd);     //一对套接字用来通信
    assert(ret!=-1);
    setnonblocking( pipe_fd[1]);
    add_fd( m_epoll_fd, pipe_fd[0], false);
    add_sig(SIGALRM, sig_handler);   //定时器结束则会发出此信号；

    time_out = false;
    alarm(time_slot);
}
int web::epoll()
{
    int num=epoll_wait(m_epoll_fd,events,max_event_number,-1);
    return num;
}
int web::conn()
{
    int conn_fd=server->accept_conn();
    if(conn_fd<0)
    {
        cout<<"errno is: "<<errno<<endl;
        return 0;
    }
    if(http_conn::m_user_count>=max_fd)
    {
        //连接满了
        close(conn_fd);
        return 0;
    }
    char ip[16] = {0};
    inet_ntop(AF_INET, &server->client_addr.sin_addr ,ip, sizeof(ip));
    cout<<ip<<":"<<server->client_addr.sin_port<<endl<<endl;
    LOG_INFO("client(%s:%d) is connected", ip,server->client_addr.sin_port);  //记录ip位置
    users[conn_fd].init(conn_fd,server->client_addr);
    util_timer* timer=new util_timer;

    timer->user_data=&users[conn_fd];
    timer->cb_func = cb_func;
    time_t cur = time( NULL );
    timer->expire = cur+3*m_time_slot;

    users[conn_fd].timer=timer;
    timer_lst->add_timer(timer);
    return 1;
}
int web::pipe_signal()
{
    int sig;
    char signals[1024];
    int ret= recv(pipe_fd[0],signals,sizeof(signals),0);
    if( ret == -1 )
    {
        return 0;
    } else if( ret == 0 )
    {
        return 0;
    }
    else
    {
        for(int i = 0;i<ret;++i)
        {
            if(signals[i]==SIGALRM)
            {
                time_out=true;
                break;
            }
        }
    }
    return 1;
}
void web::read()
{
    int len=users[sock_fd].read();
    util_timer* timer=users[sock_fd].timer;
    if(len)
    {
        pool->append(users+sock_fd);
        if(timer)
        {
            time_t cur = time(NULL);
            timer->expire = cur+3*m_time_slot;
            cout<<"adjust timer once"<<endl;
            timer_lst->adjust_timer(timer);
        }
    }
    else
    {
        cb_func(&users[sock_fd]);
        if(time)
        {
            timer_lst->del_timer(timer);
        }
        //users[sock_fd].close_conn();
    }
}
void web::write()
{
    util_timer* timer=users[sock_fd].timer;
    if(users[sock_fd].write())
    {

        //cb_func(&time_users[sock_fd]);
        if(timer)
        {
            time_t cur = time(NULL);
            timer->expire = cur+3*m_time_slot;
            cout<<"adjust timer once"<<endl;
            timer_lst->adjust_timer(timer);
        }
    }
    else
    {
        if(timer)
        {
            timer_lst->del_timer(timer);
        }
        users[sock_fd].close_conn();
    }
}

void web::work(const int &num)
{
    for(int i=0;i<num;++i)
    {
        sock_fd=events[i].data.fd;
        if(sock_fd==m_listen_fd)
        {
            if(!this->conn())
                continue;
        }
        else if((sock_fd == pipe_fd[0])&&( events[i].events & EPOLLIN ))
        {
            if(!this->pipe_signal())
                continue;
        }
        else if(events[i].events&EPOLLIN)
        {
            this->read();
        }
        else if(events[i].events&EPOLLOUT)
        {
            this->write();
        }
        else if(events[i].events&(EPOLLRDHUP|EPOLLHUP|EPOLLERR))
        {
            users[sock_fd].close_conn();
        }
    }
    if(time_out)
    {
        timer_handler(timer_lst);
        time_out = false;
    }

}

void web::close_web()
{
    close(m_epoll_fd);
    close(m_listen_fd);
    close( pipe_fd[1] );
    close( pipe_fd[0] );

}






