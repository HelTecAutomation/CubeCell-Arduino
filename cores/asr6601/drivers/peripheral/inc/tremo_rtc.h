/**
 ******************************************************************************
 * @file    tremo_rtc.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of RTC module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup RTC
 * @{
 */

#ifndef __TREMO_RTC_H
#define __TREMO_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "tremo_regs.h"
#include "tremo_rcc.h"

#define RTC_MICROSECOND 1000000 /*!< microsecond*/

#define RTC_ENABLE_TIME (0x10000000);  /*!< enable time flag*/
#define RTC_ENABLE_ALARM (0x80000000); /*!< enable alarm flag*/

/**
 * @brief RTC alarm wakeup flag
 */
typedef enum {
    RTC_ALARM0_WKEN = (uint32_t)0x8000000, /*!< alarm0 wakeup flag*/
    RTC_ALARM1_WKEN = (uint32_t)0x4000000  /*!< alarm1 wakeup flag*/
} rtc_alarm_wakeup_t;

/**
 * @brief RTC cyc control information
 */
typedef enum {
    RTC_CYC_WKEN    = (uint32_t)0x2000000, /*!< cyc wakeup flag*/
    RTC_CYC_COUNTER = (uint32_t)0x1000000  /*!< cyc counter flag*/
} rtc_cyc_ctrl_t;

/**
 * @brief RTC tamper control information
 */
typedef enum {
    RTC_TAMPER       = (uint32_t)0x800000, /*!< tamper flag*/
    RTC_TAMPER_LEVEL = (uint32_t)0x400000, /*!< tamper high level flag*/
    RTC_TAMPER_WKEN0 = (uint32_t)0x200000, /*!< tamper level wakeup flag*/
    RTC_TAMPER_WKEN1 = (uint32_t)0x100000  /*!< tamper wakeup flag*/
} rtc_tamper_ctrl_t;

/**
 * @brief RTC wakeup control information
 */
typedef enum {
    RTC_WAKEUP0       = (uint32_t)0x20000, /*!< wakeup0 flag*/
    RTC_WAKEUP0_LEVEL = (uint32_t)0x10000, /*!< wakeup0 high level flag*/
    RTC_WAKEUP0_WKEN0 = (uint32_t)0x8000,  /*!< wakeup0 level wakeup flag*/
    RTC_WAKEUP0_WKEN1 = (uint32_t)0x4000,  /*!< wakeup0 wakeup flag*/
    RTC_WAKEUP1       = (uint32_t)0x800,   /*!< wakeup1 flag*/
    RTC_WAKEUP1_LEVEL = (uint32_t)0x400,   /*!< wakeup1 high level flag*/
    RTC_WAKEUP1_WKEN0 = (uint32_t)0x200,   /*!< wakeup1 level wakeup flag*/
    RTC_WAKEUP1_WKEN1 = (uint32_t)0x100,   /*!< wakeup1 wakeup flag*/
    RTC_WAKEUP2       = (uint32_t)0x20,    /*!< wakeup2 flag*/
    RTC_WAKEUP2_LEVEL = (uint32_t)0x10,    /*!< wakeup2 high level flag*/
    RTC_WAKEUP2_WKEN0 = (uint32_t)0x8,     /*!< wakeup2 level wakeup flag*/
    RTC_WAKEUP2_WKEN1 = (uint32_t)0x4      /*!< wakeup2 wakeup flag*/
} rtc_wakeup_ctrl_t;

/**
 * @brief RTC filter
 */
typedef enum {
    RTC_NO_FILTER = 0, /*!< rtc no filter*/
    RTC_FILTER_1,      /*!< rtc filter 1*/
    RTC_FILTER_3,      /*!< rtc filter 3*/
    RTC_FILTER_7       /*!< rtc filter 7*/
} rtc_filter_t;

/**
 * @brief RTC status
 */
typedef enum {
    RTC_ALARM0_SR  = (uint32_t)0x40, /*!< rtc alarm0 status*/
    RTC_ALARM1_SR  = (uint32_t)0x20, /*!< rtc alarm1 status*/
    RTC_CYC_SR     = (uint32_t)0x10, /*!< rtc cyc status*/
    RTC_TAMPER_SR  = (uint32_t)0x8,  /*!< rtc tamper status*/
    RTC_WAKEUP0_SR = (uint32_t)0x4,  /*!< rtc wakeup0 status*/
    RTC_WAKEUP1_SR = (uint32_t)0x2,  /*!< rtc wakeup1 status*/
    RTC_WAKEUP2_SR = (uint32_t)0x1,  /*!< rtc wakeup2 status*/
} rtc_status_t;

/**
 * @brief RTC interrupt enable flag
 */
typedef enum {
    RTC_SEC_IT     = (uint32_t)0x80, /*!< rtc second interrupt*/
    RTC_ALARM0_IT  = (uint32_t)0x40, /*!< rtc alarm0 interrupt*/
    RTC_ALARM1_IT  = (uint32_t)0x20, /*!< rtc alarm1 interrupt*/
    RTC_CYC_IT     = (uint32_t)0x10, /*!< rtc cyc interrupt*/
    RTC_TAMPER_IT  = (uint32_t)0x8,  /*!< rtc tamper interrupt*/
    RTC_WAKEUP0_IT = (uint32_t)0x4,  /*!< rtc wakeup0 interrupt*/
    RTC_WAKEUP1_IT = (uint32_t)0x2,  /*!< rtc wakeup1 interrupt*/
    RTC_WAKEUP2_IT = (uint32_t)0x1,  /*!< rtc wakeup2 interrupt*/
} rtc_it_t;

/**
 * @brief RTC calendar
 */
typedef struct {
    uint16_t year;      /*!< year*/
    uint8_t week;       /*!< week*/
    uint8_t month;      /*!< month*/
    uint8_t day;        /*!< day*/
    uint8_t hour;       /*!< hour*/
    uint8_t minute;     /*!< minute*/
    uint8_t second;     /*!< second*/
    uint32_t subsecond; /*!< subsecond,us*/
} rtc_calendar_t;

/**
 * @brief RTC alarm
 */
typedef struct {
    bool dayMask;       /*!< day mask*/
    bool weekMask;      /*!< week mask*/
    bool hrMask;        /*!< hour mask*/
    bool minMask;       /*!< minute mask*/
    bool secMask;       /*!< second mask*/
    uint8_t subsecMask; /*!< subsecond mask*/
} rtc_alarm_mask_t;

/**
 * @brief RTC io output level
 */
typedef enum {
    RTC_IO_LEVEL_NO_INVERT = (uint32_t)0x0, /*!< io output level don't inverted*/
    RTC_IO_LEVEL_INVERT    = (uint32_t)0x80 /*!< io output level inverted*/
} rtc_io_level_t;

/**
 * @brief RTC io output selection
 */
typedef enum {
    RTC_IO_LOW_LEVEL = (uint32_t)0x30, /*!< low level output*/
    RTC_IO_ALARM0    = (uint32_t)0x40, /*!< alarm0 output*/
    RTC_IO_ALARM1    = (uint32_t)0x50, /*!< alarm1 output*/
    RTC_IO_CYC       = (uint32_t)0x60, /*!< cyc output*/
    RTC_IO_SEC       = (uint32_t)0x70  /*!< second output*/
} rtc_io_t;

/**
 * @brief RTC erase retention sram source
 */
typedef enum {
    RTC_ERASE_WAKEUP0_ENABLE = (uint32_t)0x1, /*!< enable wakeup0 erase*/
    RTC_ERASE_WAKEUP1_ENABLE = (uint32_t)0x2, /*!< enable wakeup1 erase*/
    RTC_ERASE_WAKEUP2_ENABLE = (uint32_t)0x4, /*!< enable wakeup2 erase*/
    RTC_ERASE_TAMPER_ENABLE  = (uint32_t)0x8  /*!< enable tamper erase*/
} rtc_ret_sram_erase_t;

#define RTC_ALARM_MAX_NUM  2 /*!< ALARM number*/
#define RTC_WAKEUP_MAX_NUM 3 /*!< WAKEUP number*/

#define CHECK_RTC_FILTER(FILTER)                                                             \
    (((FILTER) == RTC_NO_FILTER) || ((FILTER) == RTC_FILTER_1) || ((FILTER) == RTC_FILTER_3) \
        || ((FILTER) == RTC_FILTER_7)) /*!< Check rtc filter value*/

#define CHECK_SR_STATE(STATE)                                                                                          \
    (((STATE) == RTC_ALARM0_SR) || ((STATE) == RTC_ALARM1_SR) || ((STATE) == RTC_CYC_SR) || ((STATE) == RTC_TAMPER_SR) \
        || ((STATE) == RTC_WAKEUP0_SR) || ((STATE) == RTC_WAKEUP1_SR) || ((STATE) == RTC_WAKEUP2_SR)) /*!< Check rtc state*/

#define CHECK_IT_ENABLE(IT)                                                                             \
    (((IT) == RTC_SEC_IT) || ((IT) == RTC_ALARM0_IT) || ((IT) == RTC_ALARM1_IT) || ((IT) == RTC_CYC_IT) \
        || ((IT) == RTC_TAMPER_IT) || ((IT) == RTC_WAKEUP0_IT) || ((IT) == RTC_WAKEUP1_IT)              \
        || ((IT) == RTC_WAKEUP2_IT)) /*!< Check rtc interrupt*/

void rtc_deinit(void);

void rtc_calendar_cmd(bool state);
void rtc_set_calendar(rtc_calendar_t* rtc_calendar);
void rtc_get_calendar(rtc_calendar_t* rtc_calendar);
uint16_t rtc_get_subsecond_cnt(void);

void rtc_alarm_cmd(uint8_t alarm_index, bool state);
void rtc_set_alarm(uint8_t alarm_index, rtc_alarm_mask_t* alarm_mask, rtc_calendar_t* time);
void rtc_config_alarm_wakeup(uint8_t alarm_index, uint8_t state);

void rtc_cyc_cmd(bool state);
void rtc_config_cyc_wakeup(uint8_t state);
void rtc_config_cyc_max(uint32_t max_value);
uint32_t rtc_get_cyc_cnt(void);

void rtc_tamper_cmd(bool state);
void rtc_config_tamper_high_level(uint8_t state);
void rtc_config_tamper_level_wakeup(uint8_t state);
void rtc_config_tamper_wakeup(uint8_t state);
void rtc_config_tamper_filter(rtc_filter_t filter_type);

void rtc_wakeup_io_cmd(uint8_t wakeup_index, bool state);
void rtc_config_wakeup_io_high_level(uint8_t wakeup_index, uint8_t state);
void rtc_config_wakeup_io_level_wakeup(uint8_t wakeup_index, uint8_t state);
void rtc_config_wakeup_io_wakeup(uint8_t wakeup_index, uint8_t state);
void rtc_config_wakeup_io_filter(uint8_t wakeup_index, rtc_filter_t filter_type);

void rtc_config_ppm(int16_t adjust_value);

void rtc_invert_io_level(uint8_t state);
void rtc_config_io_output(rtc_io_t rtc_io);
uint8_t rtc_get_io_output();

void rtc_config_erase_ret_sram(rtc_ret_sram_erase_t ret_sram_erase, uint8_t state);

void rtc_config_interrupt(rtc_it_t it, uint8_t state);

void rtc_set_status(rtc_status_t status, bool set);
bool rtc_get_status(rtc_status_t status);
void rtc_clear_sec_it_status();
bool rtc_get_sec_it_status();

void rtc_check_syn(void);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_RTC_H */

/**
 * @} 
 * @}
 */
