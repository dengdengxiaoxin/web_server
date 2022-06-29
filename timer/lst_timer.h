//
// Created by dx on 2022/3/5.
//

#ifndef WEB_SERVER_LST_TIMER_H
#define WEB_SERVER_LST_TIMER_H
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../http_conn/http_conn.h"
#define BUFFER_SIZE 64
class util_timer;
//用户数据结构
class http_conn;
class client_data
{
public:
    sockaddr_in address;
    int sock_fd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};
//定时器类
class util_timer{
public:
    util_timer():prev(NULL),next(NULL){}

    time_t expire;  //任务的超时时间，使用绝对时间
    void (*cb_func)(http_conn*);   //任务回调函数 回调函数处理的客户端，由定时器的执行者传递给回调函数
    //client_data* user_data;
    http_conn* user_data;
    util_timer* prev;
    util_timer* next;
};

//定时器链表，它是一个升序、双向链表，且带有头节点和尾节点*/
class sort_timer_lst{
public:
    sort_timer_lst():head(NULL),tail(NULL){}
    ~sort_timer_lst()
    {
        util_timer* tmp=head;
        while (tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }

    void add_timer(util_timer* timer)
    {
        if(!timer)
        {
            return;
        }
        if(!head)
        {
            head=tail=timer;
            return;
        }
        if(timer->expire<head->expire) //如果时间高于头部进行升序
        {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        add_timer(timer,head);
    }
//定时器任务发生变化时，调整位置，一般是定时器时间延长
    void adjust_timer(util_timer* timer)
    {
        if(!timer)
        {
            return;
        }
        util_timer* tmp = timer->next;

        if(!tmp || (timer->expire<tmp->expire))    //如果在尾部第二个，或则最后一个
        {
            return;
        }
        if(timer == head)
        {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer(timer,head);
        }
        else
        {
            timer->prev->next=timer->next;   //先把定时器取出来
            timer->next->prev = timer->prev;
            add_timer(timer,timer->next);
        }
    }

    void del_timer(util_timer* timer)
    {
        if(!timer)
        {
            return;
        }
        //只有一个定时器时
        if((timer == head)&&(timer == tail))
        {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }
        if(timer == head)
        {
            head =head->next;
            head->prev=NULL;
            delete timer;
            return;
        }
        if(timer == tail)
        {
            tail=timer->prev;
            tail->next=NULL;
            delete timer;
            return;
        }
        timer->next->prev=timer->prev;
        timer->prev->next=timer->next;
        delete timer;
    }
    void tick()       //sigalrm信号每次被触发就在其信号处理函数 中执行一次tick函数，以处理链表上到期的任务
    {
        if (!head)
        {
            return;
        }
        std::cout<<"timer tick"<<std::endl;
        time_t cur = time(NULL);
        util_timer* tmp=head;
        while (tmp)
        {
            if(cur<tmp->expire)
            {
                break;
            }
            tmp->cb_func(tmp->user_data);
            head = tmp->next;
            if(head)
            {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }
private:
    void add_timer(util_timer* timer,util_timer* lst_head)
    {
        util_timer* prev=lst_head;
        util_timer* tmp=prev->next;
        while (tmp)
        {
            if(timer->expire<tmp->expire)
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev=timer;
                timer->prev=prev;
                break;
            }
            prev = tmp;
            tmp =tmp->next;
        }
        //没找到则直接插到尾部，没找到则直接到了尾部
        if(!tmp)
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }
    util_timer* head;
    util_timer* tail;
};

#endif //WEB_SERVER_LST_TIMER_H
