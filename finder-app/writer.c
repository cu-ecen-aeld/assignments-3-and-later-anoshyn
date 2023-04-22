#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);

    if (argc < 3) {
        syslog(LOG_ERR, "Not enough arguments");
        exit(EXIT_FAILURE);
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    FILE *fp = fopen(writefile, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "Failed to open file %s", writefile);
        exit(EXIT_FAILURE);
    }

    if (fprintf(fp, "%s", writestr) < 0) {
        syslog(LOG_ERR, "Failed to write string to file");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    if (fclose(fp) != 0) {
        syslog(LOG_ERR, "Failed to close file %s", writefile);
        exit(EXIT_FAILURE);
    }

    closelog();

    return EXIT_SUCCESS;
}

