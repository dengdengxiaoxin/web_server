cmake_minimum_required(VERSION 3.21)
project(web_server)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(INC_DIR /usr/include/mysql/)
set(LINK_DIR /usr/lib/mysql/)
include_directories(${INC_DIR})

link_directories(LINK_DIR)
link_libraries(mysqlclient)

add_executable(web_server main.cpp queue/block_queue.h thread_pool/threadpool.h lock/locker.h http_conn/http_conn.cpp http_conn/http_conn.h timer/lst_timer.h log/log.cpp log/log.h sql_conn/sql_connection_pool.cpp sql_conn/sql_connection_pool.h signal/signal_about.cpp signal/signal_about.h socket/socket_server.cpp socket/socket_server.h web/web.cpp web/web.h)
target_link_libraries(web_server mysqlclient)