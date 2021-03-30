/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
typedef enum __log_level_t { 
    LL_NONE   = 0x00, 
    LL_ERR    = 0x01, 
    LL_WARN   = 0x02, 
    LL_DEBUG  = 0x04, 
    LL_VDEBUG = 0x08, 
    LL_ALL    = 0x0F 
} log_level_t;


#ifdef CONFIG_LOG

extern log_level_t g_log_level;

#define LOG_PRINTF(level, ...)   \
    do {                         \
        if (g_log_level & level) \
            printf(__VA_ARGS__); \
    } while (0)

static inline int log_get_level()
{
    return g_log_level;
}

static inline void log_set_level(int level)
{
    g_log_level = level>LL_ALL?LL_ALL:level;
}

#else

#define LOG_PRINTF(level, ...)

static inline int log_get_level(void)
{
    return 0;
}

static inline void log_set_level(int level)
{
    (void)level;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H__*/
