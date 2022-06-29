//
// Created by dx on 2022/2/22.
//
#include "http_conn.h"
#include <errno.h>

// 定义HTTP响应的一些状态信息
const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the requested file.\n";
const char* doc_root = "/home/dx/CLionProjects/web_server/resources";
int http_conn::m_epoll_fd = -1;
int http_conn::m_user_count = 0;
map<string,string> users;
locker m_lock;
const char* passward="vLDl54JLgivkIdw4";
const char* user="debian-sys-maint";
const char* host="localhost";
const char* database="student_info";
const int port=3306;

connection_pool* http_conn::m_coonpool=NULL;

void cb_func(http_conn* user_data)
{
    user_data->close_conn();
}

void http_conn::connpool_init()
{
    m_coonpool=connection_pool::get_instance();    //获取一个初始化的连接池
    m_coonpool->init(host,user,passward,database,port,100,0);   //初始化并且创造一个连接描述字队列

}

void http_conn::initmysql_result(connection_pool *connpool)  //一开始就初始化，把数据都取出来；
{
    //MYSQL *mysql=NULL;
    connection_raii mysql_conn(&mysql,connpool);   //取出一个连接字
    if(mysql_query(mysql,"select username,passwd from user"))
    {
        LOG_ERROR("slect error:%s\n", mysql_errno(mysql));
    }

    MYSQL_RES *result = mysql_store_result(mysql);  //从表中检索完整的结果集
    int num_fields = mysql_num_fields(result);  //返回结果集中的列数
    MYSQL_FIELD *fields= mysql_fetch_field(result); //返回所有字段结构的数组

    while (MYSQL_ROW row= mysql_fetch_row(result))
    {
        string temp1=(row[0]);
        string temp2=(row[1]);
        users[temp1] = temp2;  //数据放入map类型
    }
}

int setnonblocking(int fd)
{
    int old_flag=fcntl(fd,F_GETFL);
    int new_flag=old_flag|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_flag);
    return old_flag;
}
void add_fd(int epoll_fd,int fd,bool one_shot)
{
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN|EPOLLRDHUP;  //异常直接断开
    if(one_shot)
    {
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

void remove_fd(int epoll_fd,int fd)
{
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

void mod_fd(int epoll_fd,int fd,int ev)
{
    epoll_event event;
    event.data.fd=fd;
    event.events=ev | EPOLLONESHOT | EPOLLRDHUP | EPOLLET;
    epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event);      //添加到触发标志位
}

void http_conn::init(int socked, const sockaddr_in& addr)
{
    m_sock_fd=socked;
    m_address=addr;
    int reuse = 1;
    setsockopt(m_sock_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof (reuse));
    add_fd(m_epoll_fd,socked, true);
    m_user_count++;
    init();
}

void http_conn::init()
{
    bytes_to_send = 0;
    bytes_have_send = 0;

    m_check_state = CHECK_STATE_REQUESTLINE;   //初始状态为检查请求行
    m_linger = false;       //默认不保持链接  Connection : keep-alive保持连接

    m_method = GET;        // 默认请求方式为GET
    m_url=0;
    m_version=0;
    m_content_length=0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    cgi = 0;
    bzero(m_read_buf,READ_BUFFER_SIZE);
    bzero(m_write_buf,WRITE_BUFFER_SIZE);
    bzero(m_real_file,FILENAME_LEN);
}
void http_conn::close_conn()
{    if(m_sock_fd!=-1)
    {
        remove_fd(m_epoll_fd,m_sock_fd);
        std::cout<<"close fd "<<m_sock_fd<<std::endl;
        char ip[16] = {0};
        inet_ntop(AF_INET, &m_address.sin_addr ,ip, sizeof(ip));
        LOG_INFO("client(%s:%d) is disconnected", ip,m_address.sin_port);  //记录ip位置
        m_sock_fd=-1;
        m_user_count--;
    }
}
bool http_conn::read()
{
    if(m_read_idx>=READ_BUFFER_SIZE)
    {
        return false;
    }
    int bytes_read = 0;
    while(true)
    {
        // 从m_read_buf + m_read_idx索引出开始保存数据，大小是READ_BUFFER_SIZE - m_read_idx
        bytes_read = recv(m_sock_fd,m_read_buf+m_read_idx,READ_BUFFER_SIZE-m_read_idx,0);
        if(bytes_read==-1)
        {
            if(errno==EAGAIN||errno==EWOULDBLOCK)
            {
                std::cerr<<"no accept data "<<std::endl;
                break;
            }
            return false;
        }
        else if(bytes_read == 0)
        {
            std::cerr<<"client is disconnected "<<std::endl;
            return false;
        }
        m_read_idx+=bytes_read;
    }
    std::cout<<m_read_buf<<std::endl;
    return true;
}

http_conn::LINE_STATUS http_conn::parse_line() {
    char temp;
    for (; m_checked_idx < m_read_idx; ++m_checked_idx) {
        temp = m_read_buf[m_checked_idx];
        if (temp == '\r') {
            if ((m_checked_idx + 1) == m_read_idx) {
                return LINE_OPEN;
            } else if (m_read_buf[m_checked_idx + 1] == '\n') {
                m_read_buf[m_checked_idx++] = '\0';     //将数据变成/0字符串好读取
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        } else if (temp == '\n') {
            if ((m_checked_idx > 1) && (m_read_buf[m_checked_idx - 1] == '\r')) {
                m_read_buf[m_checked_idx - 1] = '\0';   //将数据变成/0字符串好读取
                m_read_buf[m_checked_idx++] = '\0';     //和上面原理相同，不过会倒退
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

// 解析HTTP请求行，获得请求方法，目标URL,以及HTTP版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text)
{
    // GET /index.html HTTP/1.1
    m_url = strpbrk(text," \t");// 判断第二个参数中的字符哪个在text中最先出现
    if(!m_url)
    {
        return BAD_REQUEST;
    }
    // GET\0/index.html HTTP/1.1
    *m_url++='\0';
    char* method = text;
    if(strcasecmp(method,"GET")==0)
    {
        m_method = GET;
    }
    else if(strcasecmp(method,"POST")==0)
    {
        m_method = POST;
        cgi = 1;
    }
    else
    {
        return BAD_REQUEST;
    }
    // /index.html HTTP/1.1
    // 检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。
    m_version = strpbrk(m_url," \t");
    if(!m_version)
    {
        return BAD_REQUEST;
    }
    *m_version++='\0';
    if(strcasecmp(m_version,"HTTP/1.1")!=0)
    {
        return BAD_REQUEST;
    }

    if(strncasecmp(m_url,"http://",7)==0)
    {
        m_url+=7;
        // 在参数 str 所指向的字符串中搜索第一次出现字符 c（一个无符号字符）的位置。
        m_url = strchr(m_url,'/');
    }
    if(!m_url || m_url[0]!='/')
    {
        return BAD_REQUEST;
    }
    cout<<"url "<<m_url<<endl;
    if(strlen(m_url)==1)   //默认ip会有url为/，显示判断界面
        strcat(m_url,"index.html");
    m_check_state = CHECK_STATE_HEADER;  // 查状态变成检查头
    return NO_REQUEST;
}

// 解析HTTP请求的一个头部信息
http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{
    // 遇到空行，表示头部字段解析完毕
    if(text[0]=='\0')
    {// 如果HTTP请求有消息体，则还需要读取m_content_length字节的消息体，
    // 状态机转移到CHECK_STATE_CONTENT状态
    if(m_content_length!=0)
    {
        m_check_state = CHECK_STATE_CONTENT;
        cout<<"content_length "<<m_content_length<<endl;
        return NO_REQUEST;
    }
        // 否则说明我们已经得到了一个完整的HTTP请求
         return GET_REQUEST;
    }
    else if(strncasecmp(text,"Connection:",11)==0)
    {
        // 处理Connection 头部字段  Connection: keep-alive
        text+=11;
        text+= strspn(text," \t");
        if(strcasecmp(text,"keep-alive")==0)
        {
            m_linger= true;
        }
    }
    else if(strncasecmp(text,"Content-Length:",15)==0)
    {
        // 处理Content-Length头部字段
        text+=15;
        text+= strspn(text," \t");
        m_content_length= atol(text);   //检测content长度
    }
    else if(strncasecmp(text,"Host:",5)==0)
    {
        // 处理Host头部字段
        text+=5;
        text+= strspn(text," \t");
        m_host = text;
        std::cout<<m_host<<std::endl;
    }
    else
    {
        std::cout<<"oop! unknow header "<<std::endl;
    }
    return NO_REQUEST;
}
// 我们没有真正解析HTTP请求的消息体，只是判断它是否被完整的读入了
http_conn::HTTP_CODE http_conn::parse_content( char* text ) {
    if ( m_read_idx >= ( m_content_length + m_checked_idx ) )
    {
        text[ m_content_length ] = '\0';
        //POST请求中最后为输入的用户名和密码
        m_string = text;
        cout<<m_string<<endl;
        return GET_REQUEST;
    }
    return NO_REQUEST;
}
// 当得到一个完整、正确的HTTP请求时，我们就分析目标文件的属性，
// 如果目标文件存在、对所有用户可读，且不是目录，则使用mmap将其
// 映射到内存地址m_file_address处，并告诉调用者获取文件成功
http_conn::HTTP_CODE http_conn::do_request()
{
    strcpy(m_real_file,doc_root);
    int len = strlen(doc_root);
    const char *p= strrchr(m_url,'/');
    // 获取m_real_file文件的相关的状态信息，-1失败，0成功
    if(cgi==1&&(*(p+1)=='3'||*(p+1)=='5'))
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 100);
        strcpy(m_url_real, "/"); //加入/号
        strcat(m_url_real,m_url+2);
        strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
        free(m_url_real);

        //将用户名和密码提取出来
        //user=123&passwd=123
        char name[100],password[100];
        int i=0;
        for(i=5;m_string[i]!='&';++i)
            name[i-5]=m_string[i];
        name[i-5]='\0';
        int j=0;
        for(i=i+10;m_string[i]!='\0';++i,++j)
            password[j]=m_string[i];
        password[j]='\0';

        if(*(p+1)=='5')
        {
            char *sql_insert = (char *) malloc(sizeof (char) * 200);

            strcpy(sql_insert,"insert into user values(");
            strcat(sql_insert,"'");
            strcat(sql_insert,name);
            strcat(sql_insert,"','");
            strcat(sql_insert, password);
            strcat(sql_insert, "')");
            cout<<"id: "<<name<<endl;
            cout<<"pw: "<<password<<endl;
            cout<<sql_insert<<endl;
            m_lock.lock();
            this->initmysql_result(m_coonpool);
            m_lock.unlock();
            if(users.find(name)==users.end())
            {
                m_lock.lock();
                int res= mysql_query(mysql,sql_insert);
                users.insert(pair<string,string>(name,password));
                m_lock.unlock();

                if(!res)
                    strcpy(m_url,"/register_success.html");
                else
                    strcpy(m_url,"/register_error.html");
            }
        }
        else if(*(p+1)=='3')
        {
            this->initmysql_result(m_coonpool);
            if(users.find(name)!=users.end()&&users[name] == password)
            {
                strcpy(m_url,"/welcome.html");
            }
            else
                strcpy(m_url,"/login_error.html");
        }

    }
    if((*(p+1)=='0'))
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 100);
        strcpy(m_url_real, "/py.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        cout<<"m_real_file:"<<m_real_file<<endl;
        free(m_url_real);
    }
    else if((*(p+1)=='1'))
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 100);
        strcpy(m_url_real, "/ghs.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else if((*(p+1)=='2'))
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 100);
        strcpy(m_url_real, "/login.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else if((*(p+1)=='4'))
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 100);
        strcpy(m_url_real, "/register.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else
        strncpy(m_real_file+len,m_url,FILENAME_LEN-len-1);

    if(stat(m_real_file,&m_file_stat)<0)
    {
        return NO_RESOURCE;
    }
    if(!(m_file_stat.st_mode& S_IROTH))
    {
        return FORBIDDEN_REQUEST;
    }
    if(S_ISDIR(m_file_stat.st_mode))
    {
        return BAD_REQUEST;
    }
    // 以只读方式打开文件
    int fd = open(m_real_file,O_RDONLY);
    // 创建内存映射
    m_file_address = (char*) mmap(0,m_file_stat.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    close(fd);
    return FILE_REQUEST;
}
// 对内存映射区执行munmap操作
void http_conn::unmap()
{
    if(m_file_address)
    {
        munmap(m_file_address,m_file_stat.st_size);
        m_file_address = 0;
    }
}

bool http_conn::write()
{
    int temp = 0;
    if(bytes_to_send == 0)
    {
        mod_fd(m_epoll_fd,m_sock_fd,EPOLLIN);
        init();
        return true;
    }
    while (1)
    {
        
        temp = writev(m_sock_fd,m_iv,m_iv_count);
        //分散写  内存可以不连续
        if(temp<=-1)
        {
            // 如果TCP写缓冲没有空间，则等待下一轮EPOLLOUT事件，虽然在此期间，
            // 服务器无法立即接收到同一客户的下一个请求，但可以保证连接的完整性。
            if(errno == EAGAIN )
            {
                mod_fd(m_epoll_fd,m_sock_fd,EPOLLOUT);
                return true;
            }
            unmap();
            return false;
        }
        bytes_have_send +=temp;
        bytes_to_send -=temp;

        if(bytes_have_send>=m_iv[0].iov_len)
        {
            m_iv[0].iov_len=0;
            m_iv[1].iov_base = m_file_address + (bytes_have_send-m_write_idx);
            m_iv[1].iov_len = bytes_to_send;
        }
        else
        {
            m_iv[0].iov_base = m_write_buf+bytes_have_send;
            m_iv[0].iov_len = m_iv[0].iov_len - temp;
        }

        if(bytes_to_send<=0)
        {
            unmap();
            mod_fd(m_epoll_fd,m_sock_fd,EPOLLIN);

            if(m_linger)
            {
                init();
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}
// 往写缓冲中写入待发送的数据
bool http_conn::add_response(const char *format, ...)
{
    if(m_write_idx>=WRITE_BUFFER_SIZE)
    {
        return false;
    }
    va_list arg_list;
    va_start(arg_list,format);
    int len = vsnprintf(m_write_buf+m_write_idx,WRITE_BUFFER_SIZE - 1 - m_write_idx,format,arg_list);
    if(len>=(WRITE_BUFFER_SIZE-1-m_write_idx))
    {
        return false;
    }
    m_write_idx +=len;
    va_end(arg_list );
    return true;
}

bool http_conn::add_status_line(int status, const char *title) {
    return add_response("%s %d %s\r\n", "HTTP/1.1",status,title );
}

bool http_conn::add_headers(int content_length) {
    add_content_length(content_length);
    add_content_type();
    add_linger();
    add_blank_line();
    return true;
}

bool http_conn::add_content_length(int content_length) {
    return add_response("Content-Length: %d\r\n",content_length);
}

bool http_conn::add_linger() {
    return add_response("Connection: %s\r\n", ( m_linger == true ) ? "keep-alive" : "close");
}

bool http_conn::add_blank_line() {
    return add_response("%s", "\r\n" );
}

bool http_conn::add_content(const char *content) {
    return add_response("%s", content);
}

bool http_conn::add_content_type() {
    return add_response("Content-Type:%s\r\n", "text/html");
}

// 主状态机，解析请求
http_conn::HTTP_CODE http_conn::process_read() {
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;
    
    //解析到了一行完整的数据或则解析到了请求体，也是完成数据
    while(((m_check_state==CHECK_STATE_CONTENT)&&(line_status == LINE_OK))
    ||((line_status = parse_line())==LINE_OK))
    {
        // 获取一行数据
            text = get_line();
            m_start_line = m_checked_idx;
        switch (m_check_state) {
            case CHECK_STATE_REQUESTLINE:{
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:{
                //std::cout<<text<<std::endl;
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                } else if(ret == GET_REQUEST){
                    std::cout<<"get request "<<std::endl;
                    return do_request();
                }
                break;
            }
            case CHECK_STATE_CONTENT:{
                ret = parse_content(text);// 由线程池中的工作线程调用，这是处理HTTP请求的入口函数
                if(ret == GET_REQUEST)
                {
                    std::cout<<"get request content"<<std::endl;
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default:{
                return INTERNAL_ERROR;
            }

        }
    }
    return NO_REQUEST;
}


// 根据服务器处理HTTP请求的结果，决定返回给客户端的内容
bool http_conn::process_write(HTTP_CODE ret)
{
    switch (ret) {
        case INTERNAL_ERROR: {
            add_status_line(500, error_500_title);
            add_headers(strlen(error_500_form));
            if (!add_content(error_500_form)) {
                return false;
            }
            break;
        }
        case BAD_REQUEST: {
            add_status_line(400, error_400_title);
            add_headers(strlen(error_400_form));
            if (!add_content(error_400_form)) {
                return false;
            }
            break;
        }
        case NO_RESOURCE: {
            add_status_line(404, error_404_title);
            add_headers(strlen(error_404_form));
            if (!add_content(error_404_form)) {
                return false;
            }
            break;
        }
        case FORBIDDEN_REQUEST: {
            add_status_line(403, error_403_title);
            add_headers(strlen(error_403_form));
            if (!add_content(error_403_form)) {
                return false;
            }
            break;
        }
        case FILE_REQUEST: {
            add_status_line(200, ok_200_title);
            add_headers(m_file_stat.st_size);
            m_iv[0].iov_base = m_write_buf;
            m_iv[0].iov_len = m_write_idx;
            m_iv[1].iov_base = m_file_address;
            m_iv[1].iov_len = m_file_stat.st_size;
            m_iv_count = 2;

            bytes_to_send = m_write_idx + m_file_stat.st_size;
            return true;
        }

        default: {
            return false;
        }
    }
    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    bytes_to_send = m_write_idx;
    return true;
}

// 由线程池中的工作线程调用，这是处理HTTP请求的入口函数
void http_conn::process()
{
// 解析HTTP请求
HTTP_CODE read_ret = process_read();
if(read_ret == NO_REQUEST)
{
    mod_fd(m_epoll_fd,m_sock_fd,EPOLLIN);
    return;
}

bool write_ret = process_write(read_ret);
if(!write_ret)
{
    close_conn();
}
    mod_fd(m_epoll_fd,m_sock_fd,EPOLLOUT);   //
}

