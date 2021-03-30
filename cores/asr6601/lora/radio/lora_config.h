#ifndef __LORA_CONFIG_H
#define __LORA_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tremo_gpio.h"

#define PRINT_BY_DMA

#define CONFIG_LORA_USE_TCXO
#define CONFIG_LORA_RFSW_CTRL_PIN   59
#define CONFIG_LORA_RFSW_VDD_PIN    40

//#define CONFIG_LORA_RFSW_CTRL_GPIOX GPIOD
//#define CONFIG_LORA_RFSW_CTRL_PIN   GPIO_PIN_11
	
//#define CONFIG_LORA_RFSW_VDD_GPIOX GPIOA
//#define CONFIG_LORA_RFSW_VDD_PIN   GPIO_PIN_10

#ifdef __cplusplus
}
#endif

#endif /* __LORA_CONFIG_H */
