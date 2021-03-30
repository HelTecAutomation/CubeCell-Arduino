#ifndef __DEBUG_H__
#define __DEBUG_H__

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "utilities.h"

#define ERR_PRINTF(format, ...)    do {}while(0)
#define WARN_PRINTF(format, ...)    do {}while(0)
#define VDBG_PRINTF(format, ...)    do {}while(0)
#define PRINTF_RAW(format, ...)           printf(format, ##__VA_ARGS__)
#define PRINTF_AT(...)
#define DBG_PRINTF_CRITICAL(p)
#define DBG_PRINTF(format, ...)		do {}while(0)

#if LoRaWAN_DEBUG_LEVEL >= 2
	#define FREQ_PRINTF(format, ...)    printf(format, ##__VA_ARGS__)
	#define DIO_PRINTF(format, ...)     printf(format, ##__VA_ARGS__)
#elif LoRaWAN_DEBUG_LEVEL == 1
	#define FREQ_PRINTF(format, ...)    printf(format, ##__VA_ARGS__)
	#define DIO_PRINTF(format, ...)
#else
	#define FREQ_PRINTF(format, ...)	do {}while(0)
	#define DIO_PRINTF(format, ...)		do {}while(0)
#endif

#endif /* __DEBUG_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
