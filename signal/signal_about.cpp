//
// Created by dx on 2022/3/10.
//
#include "signal_about.h"
int pipe_fd[2];

void timer_handler(sort_timer_lst *timer_lst)
{
    timer_lst->tick();
    alarm(5);
}
void sig_handler(int sig)     //信号响应函数
{
    int save_errno = errno;
    int msg = sig;
    send(pipe_fd[1],(char* )&msg,1,0);
    errno = save_errno;
}

//添加信号捕捉
void add_sig(int sig,void(handler)(int))
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof (sa));
    sa.sa_handler=handler;
    sigfillset(&sa.sa_mask); //注册信号
    //sigaction(sig,&sa,NULL);
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

