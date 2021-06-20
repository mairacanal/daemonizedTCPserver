#include "daemonize/daemonize.h"
#include "server/server.h"

static int running = 0;
static int port = 0;
static char *confFilename = "/etc/TCPserver/TCPserver.conf";
static char *logFilename = "/var/log/TCPserver.log";
static char *daemonName  = "TCPserver";
static FILE *logStream;

void server_signal_handler(int sig);

int main (void) {

    int s;

    openlog(daemonName, LOG_PID|LOG_CONS, LOG_DAEMON);

    syslog(LOG_INFO, "Started %s", daemonName);

    open_log_file(logStream, logFilename);

    signal(SIGINT , server_signal_handler);
    signal(SIGTERM, server_signal_handler);
    signal(SIGCHLD, server_signal_handler);

    read_conf_file(0, confFilename, &port);
    running = 1;

    s = initialize_socket(port);

    while (running == 1)
        establish_connection(s);

    if (logStream != stdout)
        fclose(logStream);

    syslog(LOG_INFO, "Stopped %s", daemonName);
    closelog();

    return 0;
    
}

void sigint() {
    running = 0;
    return;
}

void sighup() {
    read_conf_file(1, confFilename, &port);
}

void sigchld() {
    return;
}

void server_signal_handler(int sig) {
    handle_signal(sig, sigint, sighup, sigchld);
    return;
}
