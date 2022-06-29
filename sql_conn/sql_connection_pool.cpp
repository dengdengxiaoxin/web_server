//
// Created by dx on 2022/3/8.
//

#include "sql_connection_pool.h"
#include <iostream>
#include "../log/log.h"
using namespace std;

connection_pool::connection_pool()
{
    m_cur_conn = 0;
    m_free_conn = 0;
}

connection_pool *connection_pool::get_instance()
{
    static connection_pool connpool;
    return &connpool;
}

void connection_pool::init(string url, string user, string password, string databasename, int port, int max_conn,
                           int close_log)
                           {
    m_url=url;
    m_port=port;
    m_user=user;
    m_password=password;
    m_database_name=databasename;
    m_close_log=close_log;

    for(int i=0;i<max_conn;++i)
    {
        MYSQL *conn=NULL;
        conn= mysql_init(conn);
        if(conn==NULL)
        {
            LOG_ERROR("mysql error");
            exit(1);
        }
        conn= mysql_real_connect(conn,url.c_str(),user.c_str(),password.c_str(),
                                 databasename.c_str(),port,NULL,0);
        if(conn == NULL)
        {
            cout<<"mysql error"<<endl;
            LOG_ERROR("mysql error");
            exit(1);
        }

        conn_list.push_back(conn);
        ++m_free_conn;
    }
    reserve = sem(m_free_conn);
    m_max_conn = m_free_conn;
}

MYSQL *connection_pool::get_connection()
{
    MYSQL *conn = NULL;
    if(0==conn_list.size())
        return NULL;
    reserve.wait();
    lock.lock();

    conn = conn_list.front();
    conn_list.pop_front();

    --m_free_conn;
    ++m_cur_conn;

    lock.unlock();
    return conn;
}
bool connection_pool::release_connection(MYSQL *conn)
{
    if(NULL==conn)
        return false;
    lock.lock();

    conn_list.push_back(conn);
    ++m_free_conn;
    --m_cur_conn;

    lock.unlock();

    reserve.post();
    return true;
}

void connection_pool::destory_pool()
{
    lock.lock();
    if(conn_list.size()>0)
    {
        list<MYSQL *>::iterator it;
        for(it=conn_list.begin();it!=conn_list.end();++it)
        {
            MYSQL *conn = *it;
            mysql_close(conn);
        }
        m_cur_conn = 0;
        m_free_conn = 0;
        conn_list.clear();
    }
    lock.unlock();
}

int connection_pool::get_freeconn()
{
    return this->m_free_conn;
}

connection_pool::~connection_pool()
{
    destory_pool();
}
connection_raii::connection_raii(MYSQL **sql, connection_pool *connpool) {
    *sql = connpool->get_connection();
    conn_raii = *sql;
    pool_raii = connpool;
}

connection_raii::~connection_raii() {
    pool_raii->release_connection(conn_raii);
}