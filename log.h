#ifndef LOG_H
#define LOG_H

extern int debug_level;

#define DEBUG_NONE   (0)
#define DEBUG_FATAL  (1)
#define DEBUG_ERR    (2)
#define DEBUG_WARN   (3)
#define DEBUG_INFO   (4)
#define DEBUG_DBG    (5)

#define DEFAULT_DEBUG_LEVEL DEBUG_FATAL

#define LOG(level, stream, message) \
{                                   \
    if (debug_level>=level) {       \
        fprintf(stream, message);   \
    }                               \
}

#define LOG_PARAMS(level, stream, format, ...)  \
{                                               \
    if (debug_level>=level) {                   \
        fprintf(stream, format, ##__VA_ARGS__); \
    }                                           \
}

#endif /* LOG_H */