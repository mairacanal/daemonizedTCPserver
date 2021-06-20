#ifndef DAEMONIZE_H
#define DAEMONIZE_H

#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>

void handle_signal(int sig, void (* sigint)(), void (* sighup)(), void (* sigchld)());
int read_conf_file(int reload, char* confFilename, int* port);
void open_log_file(FILE *logStream, char *logFilename);

#endif
