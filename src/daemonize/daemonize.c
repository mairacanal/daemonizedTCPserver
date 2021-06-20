#include "daemonize.h"

void handle_signal(int sig, void (* sigint)(), void (* sighup)(), void (* sigchld)()) {

    switch (sig) {
        case SIGINT:
            syslog(LOG_INFO, "DEBUG: stopping daemon");
            sigint();
            signal(SIGINT, SIG_DFL);
            break;
        case SIGHUP:
            syslog(LOG_INFO, "DEBUG: reloading daemon config file");
            sighup();
            break;
        case SIGCHLD:
            syslog(LOG_INFO, "DEBUG: received SIGCHLD signal");
            sigchld();
            break;
    }

}

int read_conf_file(int reload, char* confFilename, int* port) {

    FILE* confFile = NULL;
    int ret = -1;

    if (confFilename == NULL) return 0;

    confFile = fopen(confFilename, "r");

    if (confFile == NULL) {
        syslog(LOG_ERR, "Cannot open config file: %s", strerror(errno));
        return -1;
    }

    ret = fscanf(confFile, "%d", port);
    
    if (ret > 0) {
        if (reload == 1)
            syslog(LOG_INFO, "Reloaded configuration file");
        else
            syslog(LOG_INFO, "Configuration file read");
    }

    fclose(confFile); 

    return ret;

}

void open_log_file(FILE *logStream, char *logFilename) {

    if (logFilename != NULL) {
        logStream = fopen(logFilename, "w+");
        if (logStream == NULL) {
            syslog(LOG_ERR, "Cannot open log file: %s", strerror(errno));
            logStream = stdout;
        } else logStream = stdout;
    }

}
