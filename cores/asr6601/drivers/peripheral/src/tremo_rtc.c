#include <stdio.h>
#include "tremo_rtc.h"

/**
 * @brief  RTC deinitialize
 * @return
 */
void rtc_deinit(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, false);
    rcc_rst_peripheral(RCC_PERIPHERAL_RTC, true);
    rcc_rst_peripheral(RCC_PERIPHERAL_RTC, false);
}

/**
 * @brief  Check syncroniztion status
 * @return
 */
void rtc_check_syn(void)
{
    uint32_t sr1;
    int n = 0;
    while (1) {
        sr1 = RTC->SR1;
        if ((sr1 & 0xdff) == 0xdff) {
            break;
        }
        delayMicroseconds(1);
        n++;
        if(n>=10000)
        {
            printf("rtc_check_syn timeout\r\n");
        }
    }
}

/**
 * @brief  Enable or disable rtc calendar
 * @param  state true or false
 * @return
 */
void rtc_calendar_cmd(bool state)
{
    rtc_check_syn();
    if (state == true) {
        RTC->CTRL |= (uint32_t)RTC_ENABLE_TIME;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_ENABLE_TIME;
    }
}

/**
 * @brief  Enable or disable rtc alarm
 * @param  alarm_index alarm index
 * @param  state true or false
 * @return
 */
void rtc_alarm_cmd(uint8_t alarm_index, bool state)
{
    rtc_check_syn();
    if (alarm_index == 0) {
        if (state == true) {
            RTC->ALARM0 |= (uint32_t)RTC_ENABLE_ALARM;
        } else {
            RTC->ALARM0 &= (uint32_t) ~(uint32_t)RTC_ENABLE_ALARM;
        }
    } else if (alarm_index == 1) {
        if (state == true) {
            RTC->ALARM1 |= (uint32_t)RTC_ENABLE_ALARM;
        } else {
            RTC->ALARM1 &= (uint32_t) ~(uint32_t)RTC_ENABLE_ALARM;
        }
    } else {
        return;
    }
}

/**
 * @brief  Config alarm wakeup
 * @param  alarm_index alarm index
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_alarm_wakeup(uint8_t alarm_index, uint8_t state)
{
    uint32_t alarm_wakeup;

    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    if (alarm_index == 0) {
        alarm_wakeup = (uint32_t)RTC_ALARM0_WKEN;
    } else if (alarm_index == 1){
        alarm_wakeup = (uint32_t)RTC_ALARM1_WKEN;
    } else {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= alarm_wakeup;
    } else {
        RTC->CTRL &= ~alarm_wakeup;
    }
}

/**
 * @brief  Enable or disable cyc
 * @param  state true or false
 * @return
 */
void rtc_cyc_cmd(bool state)
{
    rtc_check_syn();
    if (state == true) {
        RTC->CTRL |= (uint32_t)RTC_CYC_COUNTER;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_CYC_COUNTER;
    }
}

/**
 * @brief  Enable or disable cyc
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_cyc_wakeup(uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= (uint32_t)RTC_CYC_WKEN;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_CYC_WKEN;
    }
}

/**
 * @brief  Enable or disable tamper
 * @param  state true or false
 * @return
 */
void rtc_tamper_cmd(bool state)
{
    rtc_check_syn();
    if (state == true) {
        RTC->CTRL |= (uint32_t)RTC_TAMPER;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_TAMPER;
    }
}

/**
 * @brief  Config tamper high level
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_tamper_high_level(uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= (uint32_t)RTC_TAMPER_LEVEL;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_TAMPER_LEVEL;
    }
}

/**
 * @brief  Config tamper level wakeup
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_tamper_level_wakeup(uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= (uint32_t)RTC_TAMPER_WKEN0;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_TAMPER_WKEN0;
    }
}

/**
 * @brief  Config tamper wakeup
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_tamper_wakeup(uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= (uint32_t)RTC_TAMPER_WKEN1;
    } else {
        RTC->CTRL &= (uint32_t) ~(uint32_t)RTC_TAMPER_WKEN1;
    }
}

/**
 * @brief  Config tamper filter
 * @param  filter_type filter configuration
 * @return
 */
void rtc_config_tamper_filter(rtc_filter_t filter_type)
{
    if (CHECK_RTC_FILTER(filter_type) == 0) {
        return;
    }
    rtc_check_syn();
    RTC->CTRL &= ~((uint32_t)RTC_FILTER_7 << 18);
    rtc_check_syn();
    RTC->CTRL |= (uint32_t)filter_type << 18;
}

/**
 * @brief  Enable or disable wakeup
 * @param  wakeup_io_index wakeup index
 * @param  state true or false
 * @return
 */
void rtc_wakeup_io_cmd(uint8_t wakeup_io_index, bool state)
{
    uint32_t wakeup;

    if (wakeup_io_index == 0) {
        wakeup = (uint32_t)RTC_WAKEUP0;
    } else if (wakeup_io_index == 1){
        wakeup = (uint32_t)RTC_WAKEUP1;
    } else if (wakeup_io_index == 2){
        wakeup = (uint32_t)RTC_WAKEUP2;
    } else {
        return;
    }
    rtc_check_syn();
    if (state == true) {
        RTC->CTRL |= wakeup;
    } else {
        RTC->CTRL &= ~wakeup;
    }
}

/**
 * @brief  Config wakeup high level
 * @param  wakeup_io_index wakeup index
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_wakeup_io_high_level(uint8_t wakeup_io_index, uint8_t state)
{
    uint32_t wakeup;

    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    if (wakeup_io_index == 0) {
        wakeup = (uint32_t)RTC_WAKEUP0_LEVEL;
    } else if (wakeup_io_index == 1){
        wakeup = (uint32_t)RTC_WAKEUP1_LEVEL;
    } else if (wakeup_io_index == 2){
        wakeup = (uint32_t)RTC_WAKEUP2_LEVEL;
    } else {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= wakeup;
    } else {
        RTC->CTRL &= ~wakeup;
    }
}

/**
 * @brief  Config wakeup level of wakeup
 * @param  wakeup_io_index wakeup index
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_wakeup_io_level_wakeup(uint8_t wakeup_io_index, uint8_t state)
{
    uint32_t wakeup;

    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    if (wakeup_io_index == 0) {
        wakeup = (uint32_t)RTC_WAKEUP0_WKEN0;
    } else if (wakeup_io_index == 1){
        wakeup = (uint32_t)RTC_WAKEUP1_WKEN0;
    } else if (wakeup_io_index == 2){
        wakeup = (uint32_t)RTC_WAKEUP2_WKEN0;
    } else {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= wakeup;
    } else {
        RTC->CTRL &= ~wakeup;
    }
}

/**
 * @brief  Config wakeup of wakeup io
 * @param  wakeup_io_index wakeup index
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_wakeup_io_wakeup(uint8_t wakeup_io_index, uint8_t state)
{
    uint32_t wakeup;

    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    if (wakeup_io_index == 0) {
        wakeup = (uint32_t)RTC_WAKEUP0_WKEN1;
    } else if (wakeup_io_index == 1){
        wakeup = (uint32_t)RTC_WAKEUP1_WKEN1;
    } else if (wakeup_io_index == 2){
        wakeup = (uint32_t)RTC_WAKEUP2_WKEN1;
    } else {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CTRL |= wakeup;
    } else {
        RTC->CTRL &= ~wakeup;
    }
}

/**
 * @brief  Config wakeup filter
 * @param  wakeup_io_index wakeup index
 * @param  filter_type filter configuration
 * @return
 */
void rtc_config_wakeup_io_filter(uint8_t wakeup_index, rtc_filter_t filter_type)
{
    if (CHECK_RTC_FILTER(filter_type) == 0) {
        return;
    }
    rtc_check_syn();
    if (wakeup_index == 0) {
        RTC->CTRL &= ~((uint32_t)RTC_FILTER_7 << 12);
        rtc_check_syn();
        RTC->CTRL |= (uint32_t)filter_type << 12;
    } else if (wakeup_index == 1) {
        RTC->CTRL &= ~((uint32_t)RTC_FILTER_7 << 6);
        rtc_check_syn();
        RTC->CTRL |= (uint32_t)filter_type << 6;
    } else if (wakeup_index == 1) {
        RTC->CTRL &= ~((uint32_t)RTC_FILTER_7 << 6);
        rtc_check_syn();
        RTC->CTRL |= (uint32_t)filter_type << 6;
    } else {
        RTC->CTRL &= ~((uint32_t)RTC_FILTER_7);
        rtc_check_syn();
        RTC->CTRL |= (uint32_t)filter_type;
    }
}

/**
 * @brief  Set calendar
 * @param  rtc_calendar calendar settings
 * @return
 */
void rtc_set_calendar(rtc_calendar_t* rtc_calendar)
{
    uint8_t year;

    if (rtc_calendar == NULL) {
        return;
    }
    if ((rtc_calendar->year) > 2099) {
        return;
    }
    if ((rtc_calendar->week) > 7) {
        return;
    }
    if ((rtc_calendar->month) > 12) {
        return;
    }
    if ((rtc_calendar->day) > 31) {
        return;
    }
    if ((rtc_calendar->hour) > 39) {
        return;
    }
    if ((rtc_calendar->minute) > 60) {
        return;
    }
    if ((rtc_calendar->second) > 60) {
        return;
    }
    year = rtc_calendar->year - 2000;
    rtc_check_syn();
    RTC->CALENDAR_H = ((year / 10) << 18) | ((year % 10) << 14) | ((rtc_calendar->week) << 11)
        | ((rtc_calendar->month / 10) << 10) | ((rtc_calendar->month % 10) << 6) | ((rtc_calendar->day / 10) << 4)
        | (rtc_calendar->day % 10);

    rtc_check_syn();
    RTC->CALENDAR = ((rtc_calendar->hour / 10) << 18) | ((rtc_calendar->hour % 10) << 14) | ((rtc_calendar->minute / 10) << 11)
        | ((rtc_calendar->minute % 10) << 7) | ((rtc_calendar->second / 10) << 4) | (rtc_calendar->second % 10);
}

/**
 * @brief  Get subsecond count
 * @return subsecond count
 */
uint16_t rtc_get_subsecond_cnt(void)
{
    uint16_t subsecond_cnt = 0;

    subsecond_cnt = RTC->SUB_SECOND_CNT;

    return subsecond_cnt;
}

/**
 * @brief  Get cyc count
 * @return cyc count
 */
uint32_t rtc_get_cyc_cnt(void)
{
    uint32_t cyc_cnt = 0;

    do {
        cyc_cnt = RTC->CYC_CNT;
    } while (cyc_cnt != RTC->CYC_CNT);

    return cyc_cnt;
}

/**
 * @brief  Get calendar
 * @param  rtc_calendar save calendar buffer
 * @return
 */
void rtc_get_calendar(rtc_calendar_t* rtc_calendar)
{
    uint32_t syn_data;
    uint32_t syn_data_h;
    uint8_t temp;
    uint16_t subsecond_cnt;
    float subsecond;

    if (rtc_calendar == NULL) {
        return;
    }
    do {
        subsecond_cnt = rtc_get_subsecond_cnt();
        do {
            syn_data = RTC->CALENDAR_R;
        } while (syn_data != RTC->CALENDAR_R);
        do {
            syn_data_h = RTC->CALENDAR_R_H;
        } while (syn_data_h != RTC->CALENDAR_R_H);
    } while ((subsecond_cnt != rtc_get_subsecond_cnt()) || subsecond_cnt<1);

    if (RCC_RTC_CLK_SOURCE_XO32K == rcc_get_rtc_clk_source()) {
        subsecond = ((float)(((float)RTC_MICROSECOND) / 32768) * subsecond_cnt) + 0.5;
    } else {
        subsecond = ((float)(((float)RTC_MICROSECOND) / 32000) * subsecond_cnt) + 0.5;
    }
    rtc_calendar->subsecond = (uint32_t)subsecond;

    temp             = syn_data & 0x0F;
    rtc_calendar->second = temp + ((syn_data >> 4) & 0x07) * 10;
    temp             = (syn_data >> 7) & 0x0F;
    rtc_calendar->minute = temp + ((syn_data >> 11) & 0x07) * 10;
    temp             = (syn_data >> 14) & 0x0F;
    rtc_calendar->hour   = temp + ((syn_data >> 18) & 0x03) * 10;

    temp            = syn_data_h & 0x0F;
    rtc_calendar->day   = temp + ((syn_data_h >> 4) & 0x03) * 10;
    temp            = (syn_data_h >> 6) & 0x0F;
    rtc_calendar->month = temp + ((syn_data_h >> 10) & 0x01) * 10;
    temp            = (syn_data_h >> 11) & 0x07;
    rtc_calendar->week  = temp;
    temp            = (syn_data_h >> 14) & 0x0F;
    rtc_calendar->year  = 2000 + temp + ((syn_data_h >> 18) & 0x0F) * 10;
}

/**
 * @brief  Set status
 * @param  status sr status
 * @param  set true or false
 * @return
 */
void rtc_set_status(rtc_status_t status, bool set)
{
    if (CHECK_SR_STATE(status) == 0) {
        return;
    }
    rtc_check_syn();
    if (set == true) {
        return;
    } else {
        RTC->SR = (uint32_t)status;
    }
}

/**
 * @brief  Get status
 * @param  status sr status
 * @retval true set status
 * @retval false reset status
 */
bool rtc_get_status(rtc_status_t status)
{
    uint32_t srValue;

    if (CHECK_SR_STATE(status) == 0) {
        return false;
    }
    rtc_check_syn();
    srValue = RTC->SR;
    if ((srValue & (uint32_t)status) == (uint32_t)status) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Clear second interrupt status
 * @return
 */
void rtc_clear_sec_it_status()
{
    RTC->SR1 |= (uint32_t)0x200;
}

/**
 * @brief  Get second interrupt status
 * @retval true set interrupt status
 * @retval false reset interrupt status
 */
bool rtc_get_sec_it_status()
{
    uint32_t sr1Value;

    sr1Value = RTC->SR1;
    if ((sr1Value & (uint32_t)0x200) == (uint32_t)0x200) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Config interrupt
 * @param  it rtc interrupt
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_interrupt(rtc_it_t it, uint8_t state)
{
    if ((CHECK_IT_ENABLE(it) == 0) || (CHECK_FUNC_STATE(state) == 0)) {
        return;
    }
    if (state == ENABLE) {
        RTC->CR1 |= (uint32_t)it;
    } else {
        RTC->CR1 &= (uint32_t) ~(uint32_t)it;
    }
}

/**
 * @brief  Set alarm
 * @param  alarm_index alarm index
 * @param  alarm_mask alarm mask
 * @param  time time settings
 * @return
 */
void rtc_set_alarm(uint8_t alarm_index, rtc_alarm_mask_t* alarm_mask, rtc_calendar_t* time)
{
    uint8_t dayOrWeekMaskValue = 1;
    uint8_t hrMaskValue        = 1;
    uint8_t minMaskValue       = 1;
    uint8_t secMaskValue       = 1;
    uint8_t dayMatchFlag       = 0;
    uint32_t dayOrWeekValue    = 0;
    uint32_t hrValue           = 0;
    uint32_t minValue          = 0;
    uint32_t secValue          = 0;
    uint32_t alarmValue        = 0;
    float temp;
    uint32_t* alarm_subsec_reg;

    if ((alarm_index >= RTC_ALARM_MAX_NUM) || (alarm_mask == NULL) || (time == NULL)) {
        return;
    }
    rtc_alarm_cmd(alarm_index, false);
    if (alarm_index == 0) {
        alarm_subsec_reg = (uint32_t*)&(RTC->ALARM0_SUBSECOND);
    } else {
        alarm_subsec_reg = (uint32_t*)&(RTC->ALARM1_SUBSECOND);
    }

    if (((alarm_mask->subsecMask != 0) && (time->subsecond >= 1000000)) || (alarm_mask->subsecMask > 15)) {
        return;
    }
    if (alarm_mask->subsecMask != 0) {
        if (RCC_RTC_CLK_SOURCE_XO32K == rcc_get_rtc_clk_source()) {
            temp = ((float)(time->subsecond)) / ((float)(((float)RTC_MICROSECOND) / 32768)) + 0.5;
        } else {
            temp = ((float)(time->subsecond)) / ((float)(((float)RTC_MICROSECOND) / 32000)) + 0.5;
        }
        rtc_check_syn();
        *alarm_subsec_reg &= 0xFFFF0000;
        rtc_check_syn();
        *alarm_subsec_reg |= (uint16_t)temp;
    }
    rtc_check_syn();
    *alarm_subsec_reg &= 0x0000FFFF;
    rtc_check_syn();
    *alarm_subsec_reg |= ((alarm_mask->subsecMask) << 16);

    if ((alarm_mask->dayMask) && (alarm_mask->weekMask)) {
        return;
    } else {
        if (alarm_mask->dayMask) {
            if (time->day > 31) {
                return;
            }
            dayOrWeekMaskValue = 0;
        }
        if (alarm_mask->weekMask) {
            if (time->week > 7) {
                return;
            }
            dayOrWeekMaskValue = 0;
            dayMatchFlag       = 1;
        }
    }
    if (alarm_mask->hrMask) {
        if (time->hour > 39) {
            return;
        }
        hrMaskValue = 0;
    }
    if (alarm_mask->minMask) {
        if (time->minute > 59) {
            return;
        }
        minMaskValue = 0;
    }
    if (alarm_mask->secMask) {
        if (time->second > 59) {
            return;
        }
        secMaskValue = 0;
    }
    if (dayOrWeekMaskValue == 0) {
        if (dayMatchFlag == 0) {
            dayOrWeekValue = ((time->day) / 10 << 24) | ((time->day) % 10 << 20);
        } else {
            dayOrWeekValue = (time->week) % 10 << 20;
        }
    }
    if (hrMaskValue == 0) {
        hrValue = ((time->hour) / 10 << 18) | ((time->hour) % 10 << 14);
    }
    if (minMaskValue == 0) {
        minValue = ((time->minute) / 10 << 11) | ((time->minute) % 10 << 7);
    }
    if (secMaskValue == 0) {
        secValue = ((time->second) / 10 << 4) | (time->second) % 10;
    }
    alarmValue = (dayMatchFlag << 30) | ((dayOrWeekMaskValue << 3 | hrMaskValue << 2
        | minMaskValue << 1 | secMaskValue) << 26) | dayOrWeekValue
        | hrValue | minValue | secValue;
    rtc_check_syn();
    if (alarm_index == 0) {
        RTC->ALARM0 = alarmValue;
    } else {
        RTC->ALARM1 = alarmValue;
    }
}

/**
 * @brief  Config cyc max value
 * @param  max_value max value
 * @return
 */
void rtc_config_cyc_max(uint32_t max_value)
{
    if (max_value == 0) {
        return;
    }
    rtc_check_syn();
    RTC->CYC_MAX = (uint32_t)max_value;
}

/**
 * @brief  Config ppm
 * @param  adjust_value ppm value is adjustValue/2
 * @return
 */
void rtc_config_ppm(int16_t adjust_value)
{
    if ((adjust_value > 2048) || (adjust_value < -2048)) {
        return;
    }
    rtc_check_syn();
    if (adjust_value > 0) {
        RTC->PPM_ADJUST = (uint32_t)(0x7FFF - adjust_value);
    } else if (adjust_value < 0) {
        RTC->PPM_ADJUST = (uint32_t)(0x7FFF + (-adjust_value));
    } else {
        RTC->PPM_ADJUST = (uint32_t)0x7FFF;
    }
}

/**
 * @brief  Invert io output level
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_invert_io_level(uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CR2 |= (uint32_t)RTC_IO_LEVEL_INVERT;
    } else {
        RTC->CR2 &= (uint32_t) ~(uint32_t)RTC_IO_LEVEL_INVERT;
    }
}

/**
 * @brief  Config io output
 * @param  rtc_io io output selection
 * @return
 */
void rtc_config_io_output(rtc_io_t rtc_io)
{
    rtc_check_syn();
    RTC->CR2 &= (uint32_t) ~(uint32_t)RTC_IO_SEC;
    rtc_check_syn();
    RTC->CR2 |= (uint32_t)rtc_io;
}

/**
 * @brief  Get io output
 * @retval RTC_IO_LOW_LEVEL low level output
 * @retval RTC_IO_ALARM0 alarm0 output
 * @retval RTC_IO_ALARM1 alarm1 output
 * @retval RTC_IO_CYC cyc output
 * @retval RTC_IO_SEC second output
 */
uint8_t rtc_get_io_output()
{
    rtc_check_syn();
    return (RTC->CR2 & (uint32_t)RTC_IO_SEC) >> 4;
}

/**
 * @brief  Config erase retention sram
 * @param  ret_sram_erase erase retention sram source
 * @param  state ENABLE or DISABLE
 * @return
 */
void rtc_config_erase_ret_sram(rtc_ret_sram_erase_t ret_sram_erase, uint8_t state)
{
    if (CHECK_FUNC_STATE(state) == 0) {
        return;
    }
    rtc_check_syn();
    if (state == ENABLE) {
        RTC->CR2 |= (uint32_t)ret_sram_erase;
    } else {
        RTC->CR2 &= (uint32_t) ~(uint32_t)ret_sram_erase;
    }
}