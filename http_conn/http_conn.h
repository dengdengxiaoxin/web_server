//
// Created by dx on 2022/2/22.
//

#ifndef WEB_SERVER_HTTP_CONN_H
#define WEB_SERVER_HTTP_CONN_H
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "../lock/locker.h"
#include <sys/uio.h>
#include <iostream>
#include <map>
#include <mysql/mysql.h>
#include "../timer/lst_timer.h"
#include "../log/log.h"
#include "../sql_conn/sql_connection_pool.h"
class util_timer;
class http_conn{
public:
    http_conn(){};
    ~http_conn(){};

    util_timer* timer;
    static int m_epoll_fd;
    static int m_user_count;
    static void connpool_init();
    MYSQL *mysql;
    static connection_pool* m_coonpool;
    void process();   //处理客户端请求
    void init(int socked,const sockaddr_in & addr);
    void close_conn();
    void initmysql_result(connection_pool *connpool);

    static const int FILENAME_LEN = 200;        // 文件名的最大长度
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;

    // HTTP请求方法，这里只支持GET
    enum METHOD {GET = 0,POST,HEAD,PUT,DELETE,TRACE,OPTIONS,CONNECT};

    /*
    解析客户端请求时，主状态机的状态
    CHECK_STATE_REQUESTLINE:当前正在分析请求行
    CHECK_STATE_HEADER:当前正在分析头部字段
    CHECK_STATE_CONTENT:当前正在解析请求体
*/
    enum CHECK_STATE {CHECK_STATE_REQUESTLINE = 0,CHECK_STATE_HEADER,CHECK_STATE_CONTENT};

    /*
    服务器处理HTTP请求的可能结果，报文解析的结果
    NO_REQUEST          :   请求不完整，需要继续读取客户数据
    GET_REQUEST         :   表示获得了一个完成的客户请求
    BAD_REQUEST         :   表示客户请求语法错误
    NO_RESOURCE         :   表示服务器没有资源
    FORBIDDEN_REQUEST   :   表示客户对资源没有足够的访问权限
    FILE_REQUEST        :   文件请求,获取文件成功
    INTERNAL_ERROR      :   表示服务器内部错误
    CLOSED_CONNECTION   :   表示客户端已经关闭连接了
  */

    enum HTTP_CODE {NO_REQUEST,GET_REQUEST,BAD_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST,FILE_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION};
    // 从状态机的三种可能状态，即行的读取状态，分别表示
    // 1.读取到一个完整的行 2.行出错 3.行数据尚且不完整

    enum LINE_STATUS {LINE_OK,LINE_BAD,LINE_OPEN };
    bool read();
    bool write();
    int get_sock_fd()
    {
        return m_sock_fd;
    }
private:
    int m_sock_fd;
    sockaddr_in m_address;
    char m_read_buf[READ_BUFFER_SIZE];
    int m_read_idx;        //标识读缓冲区中已经读入的客户端数据的最后一个字节的下一个位置
    int m_checked_idx;    //当前正在分析的字符在读缓冲区中的位置
    int m_start_line;       // 当前正在解析的行的起始位置

    char m_write_buf[WRITE_BUFFER_SIZE];
    int m_write_idx;
    char* m_file_address;
    struct stat m_file_stat;
    struct iovec m_iv[2];
    int m_iv_count;
    char* m_string;


    void init();

    CHECK_STATE m_check_state;              // 主状态机当前所处的状态
    METHOD m_method;                        // 请求方法

    char m_real_file[ FILENAME_LEN ];       // 客户请求的目标文件的完整路径，其内容等于 doc_root + m_url, doc_root是网站根目录
    char* m_url;                            // 客户请求的目标文件的文件名
    char* m_version;                        // HTTP协议版本号，我们仅支持HTTP1.1
    char* m_host;                           // 主机名
    int m_content_length;                   // HTTP请求的消息总长度
    bool m_linger;                          // HTTP请求是否要求保持连接
    int cgi;   //是否启用的POST
    int bytes_to_send;              // 将要发送的数据的字节数
    int bytes_have_send;
    // 已经发送的字节数
    HTTP_CODE process_read();      // 解析HTTP请求
    bool process_write(HTTP_CODE ret);    // 填充HTTP应答

    // 下面这一组函数被process_read调用以分析HTTP请求
    HTTP_CODE parse_content(char* text);         //解析请求体
    HTTP_CODE parse_request_line(char* text);   //解析请求首行
    HTTP_CODE parse_headers(char* text);        //解析请求头
    HTTP_CODE do_request();
    
    char* get_line() {return m_read_buf+m_start_line;}
    LINE_STATUS parse_line();            //解析行

    // 这一组函数被process_write调用以填充HTTP应答。
    void unmap();
    bool add_response(const char* format,...);
    bool add_content(const char* content );
    bool add_content_type();
    bool add_status_line(int status,const char* title);
    bool add_headers(int content_length );
    bool add_content_length( int content_length );
    bool add_linger();
    bool add_blank_line();
    };

int setnonblocking(int fd);
void add_fd(int epoll_fd,int fd,bool one_shot);
void remove_fd(int epoll_fd,int fd);
void mod_fd(int epoll_fd,int fd,int ev);
void cb_func(http_conn* user_data);

#endif //WEB_SERVER_HTTP_CONN_H
