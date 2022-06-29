//
// Created by dx on 2022/3/8.
//

#ifndef WEB_SERVER_SQL_CONNECTION_POOL_H
#define WEB_SERVER_SQL_CONNECTION_POOL_H
#include <iostream>
#include <list>
#include <mysql/mysql.h>
#include "../lock/locker.h"
using namespace std;

class connection_pool
{
public:
    MYSQL *get_connection();
    bool release_connection(MYSQL *conn);
    int get_freeconn();
    void destory_pool();

    static connection_pool *get_instance();
    void init(string url,string user,string password,string databasename,int port,int maxconn,int close_log);

    string m_url;
    string m_port;
    string m_user;
    string m_password;
    string m_database_name;
    int m_close_log;

private:
    connection_pool();
    ~connection_pool();

    int m_max_conn;
    int m_cur_conn;
    int m_free_conn;
    locker lock;
    list<MYSQL *> conn_list;
    sem reserve;
};

class connection_raii{
public:
    connection_raii(MYSQL **sql,connection_pool *connpool);
    ~connection_raii();

private:
    MYSQL *conn_raii;
    connection_pool *pool_raii;
};

#endif //WEB_SERVER_SQL_CONNECTION_POOL_H
