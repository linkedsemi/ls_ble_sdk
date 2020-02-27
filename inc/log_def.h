#ifndef LOG_DEF_H_
#define LOG_DEF_H_

#define JLINK_RTT           1
#define UART1               2
#define RAM_LOG             4
#define LOG_BACKEND (JLINK_RTT)

#define LVL_ERROR 1
#define LVL_WARN 2
#define LVL_INFO 3
#define LVL_DBG  4

#define GLOBAL_OUTPUT_LVL     LVL_DBG

#endif
