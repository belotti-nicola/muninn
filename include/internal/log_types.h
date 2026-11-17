#ifndef LOG_TYPES_H
#define LOG_TYPES_H

#define LOG_TYPES_H

typedef enum log_severity_t
{
    LOG_NONE = 0,

    LOG_DEBUG = 40,
    LOG_INFO = 80,
    LOG_WARN = 120,
    LOG_ERROR = 160,
    LOG_FATAL = 200

} log_severity_t;

#endif