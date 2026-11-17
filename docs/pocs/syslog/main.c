#include <syslog.h>

int main()
{

    int flags = LOG_CONS || // logs in console if it is not possible to use syslog
                LOG_PID;    // processID is logged each message

    int facility = LOG_LOCAL0; // this is a local log

    openlog("poc_syslog", LOG_PID | LOG_CONS, LOG_LOCAL0);

    syslog(LOG_ERR,  "This is an error");
    syslog(LOG_INFO, "This is an info");

    closelog();

    return 0;
}