//
// Created by dx on 2022/3/6.
//

#ifndef WEB_SERVER_LOG_H
#define WEB_SERVER_LOG_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <pthread.h>
#include "../queue/block_queue.h"
using namespace std;

class log {
public:
    static log *get_instance()     //静态函数等于只有一个
    {
        static log instance;
        return &instance;
    }

    static void *flush_log_thread(void *args)
    {
        log::get_instance()->async_write_log();
    }
    bool init(const char *file_name,int close_log,int log_buf_size = 8192,
              int split_lines = 5000000,int max_queue_size = 0);
    void write_log(int level,const char *format,...);

    void flush(void );

    static int m_close_log;

private:
    log();
    virtual ~log();
    void *async_write_log()
    {
        string single_log;
        while(m_log_queue->pop(single_log))    //阻塞状态，不会退出；
        {
            m_mutex.lock();
            fputs(single_log.c_str(),m_fp);
            m_mutex.unlock();
        }
    }

    char dir_name[128];
    char log_name[128];
    int m_split_lines;
    int m_log_buf_size;
    long long m_count;
    int m_today;
    FILE *m_fp;
    char *m_buf;
    block_queue<string> *m_log_queue;
    bool m_is_async;
    locker m_mutex;
};
#define LOG_DEBUG(format, ...) if(!log::m_close_log) {log::get_instance()->write_log(0, format, ##__VA_ARGS__); log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(!log::m_close_log) {log::get_instance()->write_log(1, format, ##__VA_ARGS__); log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(!log::m_close_log) {log::get_instance()->write_log(2, format, ##__VA_ARGS__); log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(!log::m_close_log) {log::get_instance()->write_log(3, format, ##__VA_ARGS__); log::get_instance()->flush();}

#endif //WEB_SERVER_LOG_H
