#include <iostream>
#include <cstdlib>
#include "thread_pool/threadpool.h"
#include "lock/locker.h"
#include "http_conn/http_conn.h"
#include "signal/signal_about.h"
#include "socket/socket_server.h"
#include "web/web.h"
using namespace std;
int port=10000,close_log=0;

int main(int argc,char * argv[])
{
    for(int i=1;i<argc;++i)
    {
        if(i==1)
            port=atoi(argv[1]);
        else if(i==2)
            close_log=atoi(argv[2]);
        else
            break;
    }

     web web_server;
     web_server.init(close_log);
     web_server.set(port,1,5);
     while (true)
    {
         int num;
         if((num=web_server.epoll())<0&&(errno!=EINTR))
         {
             cout<<"epoll failed"<<endl;
             break;
         }
         web_server.work(num);
    }
     web_server.close_web();
     return 0;
}
