
#ifndef __SYSTEM_CM4_H
#define __SYSTEM_CM4_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tremo_cm4.h"

#ifdef USE_FULL_ASSERT
#define assert_param(x) ((x) ? ((void)0) : assert_failed(__FILE__, __LINE__))
void assert_failed(void* file, uint32_t line);
#else
#define assert_param(x) ((void)0)
#endif

void system_init(void);

#ifdef __cplusplus
}
#endif

#endif
