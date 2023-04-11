#ifndef LOGGER_H
#define LOGGER_H

#define LOG_INFO "INFO"
#define LOG_WARN "WARN"
#define LOG_ERR  "ERR "

#define logger(level, format, ...) printf("[%s] %s:%d: " format, level, __FILE__, __LINE__,  __VA_ARGS__)

#endif /* LOGGER_H */
