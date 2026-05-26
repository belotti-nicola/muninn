#ifndef LOG_TYPES_H
#define LOG_TYPES_H

#define LOG_TYPES_H

typedef enum log_severity_t
{
    LOG_NONE = 0,

    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL

} log_severity_t;

#endif