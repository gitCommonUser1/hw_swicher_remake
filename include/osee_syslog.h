#ifndef OSEE_SYSLOG_H
#define OSEE_SYSLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <syslog.h>

#include <time.h>
#include <sys/time.h>

//#include "zlog.h"
//extern zlog_category_t *out_error;

static uint8_t g_syslog_priority = 0xFF;

#define PRINT_TIMENow() do { \
                        struct timespec ts; \
                        clock_gettime(CLOCK_BOOTTIME, &ts); \
                        printf("(%ld.%.9ld) ", ts.tv_sec, ts.tv_nsec); \
                    } while(0)

#define SYS_DEBUG(args, ...)    do {syslog(LOG_DEBUG, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x80) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_INFO(args, ...)     do {syslog(LOG_INFO, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x40) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__); } }while(0)
#define SYS_NOTICE(args, ...)   do {syslog(LOG_NOTICE, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x20) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_WARNING(args, ...)  do {syslog(LOG_WARNING, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x10) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_ERR(args, ...)      do {syslog(LOG_ERR, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x08) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_CRIT(args, ...)     do {syslog(LOG_CRIT, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x04) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_ALERT(args, ...)    do {syslog(LOG_ALERT, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x02) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)
#define SYS_EMERG(args, ...)    do {syslog(LOG_EMERG, "[%s:%d]: " args, __func__, __LINE__, ## __VA_ARGS__); if (g_syslog_priority & 0x01) { PRINT_TIMENow(); printf(args, ## __VA_ARGS__);} }while(0)

#ifdef __cplusplus
}
#endif

#endif // OSEE_SYSLOG_H
