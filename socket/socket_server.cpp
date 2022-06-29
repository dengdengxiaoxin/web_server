//
// Created by dx on 2022/3/10.
//
#include "socket_server.h"
using namespace std;
int socket_server::init(const int &port)
{
    server_port=port;
    listen_fd= socket(PF_INET,SOCK_STREAM,0);
    if(listen_fd==-1)
    {
        cerr<<"create socket failed "<<endl;
        exit(0);
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port= htons(server_port);
    return listen_fd;
}

void socket_server::build_bind(const int &reuse)
{
    if(reuse)
    {
        setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof (reuse));
    }
    int ret= bind(listen_fd,(sockaddr*)&server_addr,sizeof (server_addr));
    if(ret==-1)
    {
        cout<<"bind fialed "<<endl;
        exit(-1);
    }
    ret= listen(listen_fd,5);  //建立监听状态
    if(ret==-1)
    {
        cout<<"listen fialed "<<endl;
        exit(-1);
    }
}

int socket_server::accept_conn()
{

    socklen_t addr_len=sizeof(client_addr);
    conn_fd= accept(listen_fd, (sockaddr*)&client_addr,&addr_len);
    return conn_fd;
}

void socket_server::close_fd()
{
    close(listen_fd);
    //close(conn_fd);
}