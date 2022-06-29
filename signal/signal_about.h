//
// Created by dx on 2022/3/10.
//
#ifndef WEB_SERVER_SIGNAL_ABOUT_H
#define WEB_SERVER_SIGNAL_ABOUT_H
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include "../timer/lst_timer.h"

void sig_handler(int sig);
void add_sig(int sig,void(handler)(int));
void timer_handler(sort_timer_lst *timer_lst);
extern int pipe_fd[2];

#endif //WEB_SERVER_SIGNAL_ABOUT_H
