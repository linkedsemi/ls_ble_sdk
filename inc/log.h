#ifndef LOG_H_
#define LOG_H_
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "log_def.h"


#if !defined(LOG_TAG)
    #define LOG_TAG                "NO_TAG"
#endif /* !defined(LOG_TAG) */

#if !defined(LOG_LVL)
    #define LOG_LVL                LVL_DBG
#endif /* !defined(LOG_LVL) */

void log_output(bool linefeed,const char *format,...);
void log_init(void);
void log_hex_output(const void * data_pointer , uint16_t data_length);


#if (LOG_LVL >= LVL_DBG) && (GLOBAL_OUTPUT_LVL >= LVL_DBG)
    #define log_d(TAG, ...)           log_output(true,"D/"TAG":"__VA_ARGS__)
#else
    #define log_d(TAG, ...)
#endif

#if (LOG_LVL >= LVL_INFO) && (GLOBAL_OUTPUT_LVL >= LVL_INFO)
    #define log_i(TAG, ...)           log_output(true,"I/"TAG":"__VA_ARGS__)
#else
    #define log_i(TAG, ...)
#endif

#if (LOG_LVL >= LVL_WARN) && (GLOBAL_OUTPUT_LVL >= LVL_WARN)
    #define log_w(TAG, ...)           log_output(true,"W/"TAG":"__VA_ARGS__)
#else
    #define log_w(TAG, ...)
#endif 

#if (LOG_LVL >= LVL_ERROR) && (GLOBAL_OUTPUT_LVL >= LVL_ERROR)
    #define log_e(TAG, ...)           log_output(true,"E/"TAG":"__VA_ARGS__)
#else
    #define log_e(TAG, ...)
#endif 

#define LOG_D(...)  log_d(LOG_TAG,__VA_ARGS__)
#define LOG_I(...)  log_i(LOG_TAG,__VA_ARGS__)
#define LOG_W(...)  log_w(LOG_TAG,__VA_ARGS__)
#define LOG_E(...)  log_e(LOG_TAG,__VA_ARGS__)
#if (GLOBAL_OUTPUT_LVL)
#define LOG_RAW(...)                        log_output(false, __VA_ARGS__)
#define LOG_HEX(data_pointer,data_length)   log_hex_output(data_pointer,data_length)
#define LOG_INIT()  log_init()
#else
#define LOG_RAW(...)             
#define LOG_HEX(data_pointer,data_length)  
#define LOG_INIT()  

#endif
#endif
