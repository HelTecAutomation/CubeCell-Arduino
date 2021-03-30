/*******************************************************************************
* File Name: PWM1.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the PWM1
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_PWM1_H)
#define CY_TCPWM_PWM1_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/* PWM1 */
#define PWM1_cy_m0s8_tcpwm_1__CC CYREG_TCPWM_CNT5_CC
#define PWM1_cy_m0s8_tcpwm_1__CC_BUFF CYREG_TCPWM_CNT5_CC_BUFF
#define PWM1_cy_m0s8_tcpwm_1__COUNTER CYREG_TCPWM_CNT5_COUNTER
#define PWM1_cy_m0s8_tcpwm_1__CTRL CYREG_TCPWM_CNT5_CTRL
#define PWM1_cy_m0s8_tcpwm_1__INTR CYREG_TCPWM_CNT5_INTR
#define PWM1_cy_m0s8_tcpwm_1__INTR_MASK CYREG_TCPWM_CNT5_INTR_MASK
#define PWM1_cy_m0s8_tcpwm_1__INTR_MASKED CYREG_TCPWM_CNT5_INTR_MASKED
#define PWM1_cy_m0s8_tcpwm_1__INTR_SET CYREG_TCPWM_CNT5_INTR_SET
#define PWM1_cy_m0s8_tcpwm_1__PERIOD CYREG_TCPWM_CNT5_PERIOD
#define PWM1_cy_m0s8_tcpwm_1__PERIOD_BUFF CYREG_TCPWM_CNT5_PERIOD_BUFF
#define PWM1_cy_m0s8_tcpwm_1__STATUS CYREG_TCPWM_CNT5_STATUS
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMD CYREG_TCPWM_CMD
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_MASK 0x20u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_SHIFT 5u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_MASK 0x2000u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_SHIFT 13u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_MASK 0x20000000u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_SHIFT 29u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_MASK 0x200000u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_SHIFT 21u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL CYREG_TCPWM_CTRL
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK 0x20u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL_SHIFT 5u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE CYREG_TCPWM_INTR_CAUSE
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_MASK 0x20u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_SHIFT 5u
#define PWM1_cy_m0s8_tcpwm_1__TCPWM_NUMBER 5u
#define PWM1_cy_m0s8_tcpwm_1__TR_CTRL0 CYREG_TCPWM_CNT5_TR_CTRL0
#define PWM1_cy_m0s8_tcpwm_1__TR_CTRL1 CYREG_TCPWM_CNT5_TR_CTRL1
#define PWM1_cy_m0s8_tcpwm_1__TR_CTRL2 CYREG_TCPWM_CNT5_TR_CTRL2    

/* ClockPWM1 */
#define ClockPWM1__CTRL_REGISTER CYREG_PERI_PCLK_CTL11
#define ClockPWM1__DIV_ID 0x00000041u
#define ClockPWM1__DIV_REGISTER CYREG_PERI_DIV_16_CTL1
#define ClockPWM1__PA_DIV_ID 0x000000FFu    
    
    
/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} PWM1_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  PWM1_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define PWM1_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define PWM1_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define PWM1_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define PWM1_QUAD_ENCODING_MODES            (0lu)
#define PWM1_QUAD_AUTO_START                (0lu)

/* Signal modes */
#define PWM1_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define PWM1_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define PWM1_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define PWM1_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define PWM1_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define PWM1_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define PWM1_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define PWM1_TC_RUN_MODE                    (0lu)
#define PWM1_TC_COUNTER_MODE                (0lu)
#define PWM1_TC_COMP_CAP_MODE               (2lu)
#define PWM1_TC_PRESCALER                   (0lu)

/* Signal modes */
#define PWM1_TC_RELOAD_SIGNAL_MODE          (0lu)
#define PWM1_TC_COUNT_SIGNAL_MODE           (3lu)
#define PWM1_TC_START_SIGNAL_MODE           (0lu)
#define PWM1_TC_STOP_SIGNAL_MODE            (0lu)
#define PWM1_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWM1_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define PWM1_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define PWM1_TC_START_SIGNAL_PRESENT        (0lu)
#define PWM1_TC_STOP_SIGNAL_PRESENT         (0lu)
#define PWM1_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWM1_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define PWM1_PWM_KILL_EVENT                 (0lu)
#define PWM1_PWM_STOP_EVENT                 (0lu)
#define PWM1_PWM_MODE                       (4lu)
#define PWM1_PWM_OUT_N_INVERT               (0lu)
#define PWM1_PWM_OUT_INVERT                 (0lu)
#define PWM1_PWM_ALIGN                      (0lu)
#define PWM1_PWM_RUN_MODE                   (0lu)
#define PWM1_PWM_DEAD_TIME_CYCLE            (0lu)
#define PWM1_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define PWM1_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define PWM1_PWM_COUNT_SIGNAL_MODE          (3lu)
#define PWM1_PWM_START_SIGNAL_MODE          (0lu)
#define PWM1_PWM_STOP_SIGNAL_MODE           (0lu)
#define PWM1_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWM1_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define PWM1_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define PWM1_PWM_START_SIGNAL_PRESENT       (0lu)
#define PWM1_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define PWM1_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWM1_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define PWM1_TC_PERIOD_VALUE                (65535lu)
#define PWM1_TC_COMPARE_VALUE               (65535lu)
#define PWM1_TC_COMPARE_BUF_VALUE           (65535lu)
#define PWM1_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define PWM1_PWM_PERIOD_VALUE               (63000lu)
#define PWM1_PWM_PERIOD_BUF_VALUE           (65535lu)
#define PWM1_PWM_PERIOD_SWAP                (0lu)
#define PWM1_PWM_COMPARE_VALUE              (0lu)
#define PWM1_PWM_COMPARE_BUF_VALUE          (65535lu)
#define PWM1_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define PWM1__LEFT 0
#define PWM1__RIGHT 1
#define PWM1__CENTER 2
#define PWM1__ASYMMETRIC 3

#define PWM1__X1 0
#define PWM1__X2 1
#define PWM1__X4 2

#define PWM1__PWM 4
#define PWM1__PWM_DT 5
#define PWM1__PWM_PR 6

#define PWM1__INVERSE 1
#define PWM1__DIRECT 0

#define PWM1__CAPTURE 2
#define PWM1__COMPARE 0

#define PWM1__TRIG_LEVEL 3
#define PWM1__TRIG_RISING 0
#define PWM1__TRIG_FALLING 1
#define PWM1__TRIG_BOTH 2

#define PWM1__INTR_MASK_TC 1
#define PWM1__INTR_MASK_CC_MATCH 2
#define PWM1__INTR_MASK_NONE 0
#define PWM1__INTR_MASK_TC_CC 3

#define PWM1__UNCONFIG 8
#define PWM1__TIMER 1
#define PWM1__QUAD 3
#define PWM1__PWM_SEL 7

#define PWM1__COUNT_UP 0
#define PWM1__COUNT_DOWN 1
#define PWM1__COUNT_UPDOWN0 2
#define PWM1__COUNT_UPDOWN1 3


/* Prescaler */
#define PWM1_PRESCALE_DIVBY1                ((uint32)(0u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY2                ((uint32)(1u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY4                ((uint32)(2u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY8                ((uint32)(3u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY16               ((uint32)(4u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY32               ((uint32)(5u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY64               ((uint32)(6u << PWM1_PRESCALER_SHIFT))
#define PWM1_PRESCALE_DIVBY128              ((uint32)(7u << PWM1_PRESCALER_SHIFT))

/* TCPWM set modes */
#define PWM1_MODE_TIMER_COMPARE             ((uint32)(PWM1__COMPARE         <<  \
                                                                  PWM1_MODE_SHIFT))
#define PWM1_MODE_TIMER_CAPTURE             ((uint32)(PWM1__CAPTURE         <<  \
                                                                  PWM1_MODE_SHIFT))
#define PWM1_MODE_QUAD                      ((uint32)(PWM1__QUAD            <<  \
                                                                  PWM1_MODE_SHIFT))
#define PWM1_MODE_PWM                       ((uint32)(PWM1__PWM             <<  \
                                                                  PWM1_MODE_SHIFT))
#define PWM1_MODE_PWM_DT                    ((uint32)(PWM1__PWM_DT          <<  \
                                                                  PWM1_MODE_SHIFT))
#define PWM1_MODE_PWM_PR                    ((uint32)(PWM1__PWM_PR          <<  \
                                                                  PWM1_MODE_SHIFT))

/* Quad Modes */
#define PWM1_MODE_X1                        ((uint32)(PWM1__X1              <<  \
                                                                  PWM1_QUAD_MODE_SHIFT))
#define PWM1_MODE_X2                        ((uint32)(PWM1__X2              <<  \
                                                                  PWM1_QUAD_MODE_SHIFT))
#define PWM1_MODE_X4                        ((uint32)(PWM1__X4              <<  \
                                                                  PWM1_QUAD_MODE_SHIFT))

/* Counter modes */
#define PWM1_COUNT_UP                       ((uint32)(PWM1__COUNT_UP        <<  \
                                                                  PWM1_UPDOWN_SHIFT))
#define PWM1_COUNT_DOWN                     ((uint32)(PWM1__COUNT_DOWN      <<  \
                                                                  PWM1_UPDOWN_SHIFT))
#define PWM1_COUNT_UPDOWN0                  ((uint32)(PWM1__COUNT_UPDOWN0   <<  \
                                                                  PWM1_UPDOWN_SHIFT))
#define PWM1_COUNT_UPDOWN1                  ((uint32)(PWM1__COUNT_UPDOWN1   <<  \
                                                                  PWM1_UPDOWN_SHIFT))

/* PWM output invert */
#define PWM1_INVERT_LINE                    ((uint32)(PWM1__INVERSE         <<  \
                                                                  PWM1_INV_OUT_SHIFT))
#define PWM1_INVERT_LINE_N                  ((uint32)(PWM1__INVERSE         <<  \
                                                                  PWM1_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define PWM1_TRIG_RISING                    ((uint32)PWM1__TRIG_RISING)
#define PWM1_TRIG_FALLING                   ((uint32)PWM1__TRIG_FALLING)
#define PWM1_TRIG_BOTH                      ((uint32)PWM1__TRIG_BOTH)
#define PWM1_TRIG_LEVEL                     ((uint32)PWM1__TRIG_LEVEL)

/* Interrupt mask */
#define PWM1_INTR_MASK_TC                   ((uint32)PWM1__INTR_MASK_TC)
#define PWM1_INTR_MASK_CC_MATCH             ((uint32)PWM1__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define PWM1_CC_MATCH_SET                   (0x00u)
#define PWM1_CC_MATCH_CLEAR                 (0x01u)
#define PWM1_CC_MATCH_INVERT                (0x02u)
#define PWM1_CC_MATCH_NO_CHANGE             (0x03u)
#define PWM1_OVERLOW_SET                    (0x00u)
#define PWM1_OVERLOW_CLEAR                  (0x04u)
#define PWM1_OVERLOW_INVERT                 (0x08u)
#define PWM1_OVERLOW_NO_CHANGE              (0x0Cu)
#define PWM1_UNDERFLOW_SET                  (0x00u)
#define PWM1_UNDERFLOW_CLEAR                (0x10u)
#define PWM1_UNDERFLOW_INVERT               (0x20u)
#define PWM1_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define PWM1_PWM_MODE_LEFT                  (PWM1_CC_MATCH_CLEAR        |   \
                                                         PWM1_OVERLOW_SET           |   \
                                                         PWM1_UNDERFLOW_NO_CHANGE)
#define PWM1_PWM_MODE_RIGHT                 (PWM1_CC_MATCH_SET          |   \
                                                         PWM1_OVERLOW_NO_CHANGE     |   \
                                                         PWM1_UNDERFLOW_CLEAR)
#define PWM1_PWM_MODE_ASYM                  (PWM1_CC_MATCH_INVERT       |   \
                                                         PWM1_OVERLOW_SET           |   \
                                                         PWM1_UNDERFLOW_CLEAR)

#if (PWM1_CY_TCPWM_V2)
    #if(PWM1_CY_TCPWM_4000)
        #define PWM1_PWM_MODE_CENTER                (PWM1_CC_MATCH_INVERT       |   \
                                                                 PWM1_OVERLOW_NO_CHANGE     |   \
                                                                 PWM1_UNDERFLOW_CLEAR)
    #else
        #define PWM1_PWM_MODE_CENTER                (PWM1_CC_MATCH_INVERT       |   \
                                                                 PWM1_OVERLOW_SET           |   \
                                                                 PWM1_UNDERFLOW_CLEAR)
    #endif /* (PWM1_CY_TCPWM_4000) */
#else
    #define PWM1_PWM_MODE_CENTER                (PWM1_CC_MATCH_INVERT       |   \
                                                             PWM1_OVERLOW_NO_CHANGE     |   \
                                                             PWM1_UNDERFLOW_CLEAR)
#endif /* (PWM1_CY_TCPWM_NEW) */

/* Command operations without condition */
#define PWM1_CMD_CAPTURE                    (0u)
#define PWM1_CMD_RELOAD                     (8u)
#define PWM1_CMD_STOP                       (16u)
#define PWM1_CMD_START                      (24u)

/* Status */
#define PWM1_STATUS_DOWN                    (1u)
#define PWM1_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   PWM1_Init(void);
void   PWM1_Enable(void);
void   PWM1_Start(void);
void   PWM1_Stop(void);

void   PWM1_SetMode(uint32 mode);
void   PWM1_SetCounterMode(uint32 counterMode);
void   PWM1_SetPWMMode(uint32 modeMask);
void   PWM1_SetQDMode(uint32 qdMode);

void   PWM1_SetPrescaler(uint32 prescaler);
void   PWM1_TriggerCommand(uint32 mask, uint32 command);
void   PWM1_SetOneShot(uint32 oneShotEnable);
uint32 PWM1_ReadStatus(void);

void   PWM1_SetPWMSyncKill(uint32 syncKillEnable);
void   PWM1_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   PWM1_SetPWMDeadTime(uint32 deadTime);
void   PWM1_SetPWMInvert(uint32 mask);

void   PWM1_SetInterruptMode(uint32 interruptMask);
uint32 PWM1_GetInterruptSourceMasked(void);
uint32 PWM1_GetInterruptSource(void);
void   PWM1_ClearInterrupt(uint32 interruptMask);
void   PWM1_SetInterrupt(uint32 interruptMask);

void   PWM1_WriteCounter(uint32 count);
uint32 PWM1_ReadCounter(void);

uint32 PWM1_ReadCapture(void);
uint32 PWM1_ReadCaptureBuf(void);

void   PWM1_WritePeriod(uint32 period);
uint32 PWM1_ReadPeriod(void);
void   PWM1_WritePeriodBuf(uint32 periodBuf);
uint32 PWM1_ReadPeriodBuf(void);

void   PWM1_WriteCompare(uint32 compare);
uint32 PWM1_ReadCompare(void);
void   PWM1_WriteCompareBuf(uint32 compareBuf);
uint32 PWM1_ReadCompareBuf(void);

void   PWM1_SetPeriodSwap(uint32 swapEnable);
void   PWM1_SetCompareSwap(uint32 swapEnable);

void   PWM1_SetCaptureMode(uint32 triggerMode);
void   PWM1_SetReloadMode(uint32 triggerMode);
void   PWM1_SetStartMode(uint32 triggerMode);
void   PWM1_SetStopMode(uint32 triggerMode);
void   PWM1_SetCountMode(uint32 triggerMode);

void   PWM1_SaveConfig(void);
void   PWM1_RestoreConfig(void);
void   PWM1_Sleep(void);
void   PWM1_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define PWM1_BLOCK_CONTROL_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWM1_BLOCK_CONTROL_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWM1_COMMAND_REG                    (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWM1_COMMAND_PTR                    ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWM1_INTRRUPT_CAUSE_REG             (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWM1_INTRRUPT_CAUSE_PTR             ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWM1_CONTROL_REG                    (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__CTRL )
#define PWM1_CONTROL_PTR                    ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__CTRL )
#define PWM1_STATUS_REG                     (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__STATUS )
#define PWM1_STATUS_PTR                     ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__STATUS )
#define PWM1_COUNTER_REG                    (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__COUNTER )
#define PWM1_COUNTER_PTR                    ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__COUNTER )
#define PWM1_COMP_CAP_REG                   (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__CC )
#define PWM1_COMP_CAP_PTR                   ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__CC )
#define PWM1_COMP_CAP_BUF_REG               (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWM1_COMP_CAP_BUF_PTR               ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWM1_PERIOD_REG                     (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__PERIOD )
#define PWM1_PERIOD_PTR                     ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__PERIOD )
#define PWM1_PERIOD_BUF_REG                 (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWM1_PERIOD_BUF_PTR                 ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWM1_TRIG_CONTROL0_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWM1_TRIG_CONTROL0_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWM1_TRIG_CONTROL1_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWM1_TRIG_CONTROL1_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWM1_TRIG_CONTROL2_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWM1_TRIG_CONTROL2_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWM1_INTERRUPT_REQ_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR )
#define PWM1_INTERRUPT_REQ_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR )
#define PWM1_INTERRUPT_SET_REG              (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_SET )
#define PWM1_INTERRUPT_SET_PTR              ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_SET )
#define PWM1_INTERRUPT_MASK_REG             (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWM1_INTERRUPT_MASK_PTR             ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWM1_INTERRUPT_MASKED_REG           (*(reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_MASKED )
#define PWM1_INTERRUPT_MASKED_PTR           ( (reg32 *) PWM1_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define PWM1_MASK                           ((uint32)PWM1_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define PWM1_RELOAD_CC_SHIFT                (0u)
#define PWM1_RELOAD_PERIOD_SHIFT            (1u)
#define PWM1_PWM_SYNC_KILL_SHIFT            (2u)
#define PWM1_PWM_STOP_KILL_SHIFT            (3u)
#define PWM1_PRESCALER_SHIFT                (8u)
#define PWM1_UPDOWN_SHIFT                   (16u)
#define PWM1_ONESHOT_SHIFT                  (18u)
#define PWM1_QUAD_MODE_SHIFT                (20u)
#define PWM1_INV_OUT_SHIFT                  (20u)
#define PWM1_INV_COMPL_OUT_SHIFT            (21u)
#define PWM1_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define PWM1_RELOAD_CC_MASK                 ((uint32)(PWM1_1BIT_MASK        <<  \
                                                                            PWM1_RELOAD_CC_SHIFT))
#define PWM1_RELOAD_PERIOD_MASK             ((uint32)(PWM1_1BIT_MASK        <<  \
                                                                            PWM1_RELOAD_PERIOD_SHIFT))
#define PWM1_PWM_SYNC_KILL_MASK             ((uint32)(PWM1_1BIT_MASK        <<  \
                                                                            PWM1_PWM_SYNC_KILL_SHIFT))
#define PWM1_PWM_STOP_KILL_MASK             ((uint32)(PWM1_1BIT_MASK        <<  \
                                                                            PWM1_PWM_STOP_KILL_SHIFT))
#define PWM1_PRESCALER_MASK                 ((uint32)(PWM1_8BIT_MASK        <<  \
                                                                            PWM1_PRESCALER_SHIFT))
#define PWM1_UPDOWN_MASK                    ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                            PWM1_UPDOWN_SHIFT))
#define PWM1_ONESHOT_MASK                   ((uint32)(PWM1_1BIT_MASK        <<  \
                                                                            PWM1_ONESHOT_SHIFT))
#define PWM1_QUAD_MODE_MASK                 ((uint32)(PWM1_3BIT_MASK        <<  \
                                                                            PWM1_QUAD_MODE_SHIFT))
#define PWM1_INV_OUT_MASK                   ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                            PWM1_INV_OUT_SHIFT))
#define PWM1_MODE_MASK                      ((uint32)(PWM1_3BIT_MASK        <<  \
                                                                            PWM1_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define PWM1_CAPTURE_SHIFT                  (0u)
#define PWM1_COUNT_SHIFT                    (2u)
#define PWM1_RELOAD_SHIFT                   (4u)
#define PWM1_STOP_SHIFT                     (6u)
#define PWM1_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define PWM1_CAPTURE_MASK                   ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                  PWM1_CAPTURE_SHIFT))
#define PWM1_COUNT_MASK                     ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                  PWM1_COUNT_SHIFT))
#define PWM1_RELOAD_MASK                    ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                  PWM1_RELOAD_SHIFT))
#define PWM1_STOP_MASK                      ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                  PWM1_STOP_SHIFT))
#define PWM1_START_MASK                     ((uint32)(PWM1_2BIT_MASK        <<  \
                                                                  PWM1_START_SHIFT))

/* MASK */
#define PWM1_1BIT_MASK                      ((uint32)0x01u)
#define PWM1_2BIT_MASK                      ((uint32)0x03u)
#define PWM1_3BIT_MASK                      ((uint32)0x07u)
#define PWM1_6BIT_MASK                      ((uint32)0x3Fu)
#define PWM1_8BIT_MASK                      ((uint32)0xFFu)
#define PWM1_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define PWM1_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define PWM1_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(PWM1_QUAD_ENCODING_MODES     << PWM1_QUAD_MODE_SHIFT))       |\
         ((uint32)(PWM1_CONFIG                  << PWM1_MODE_SHIFT)))

#define PWM1_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(PWM1_PWM_STOP_EVENT          << PWM1_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(PWM1_PWM_OUT_INVERT          << PWM1_INV_OUT_SHIFT))         |\
         ((uint32)(PWM1_PWM_OUT_N_INVERT        << PWM1_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(PWM1_PWM_MODE                << PWM1_MODE_SHIFT)))

#define PWM1_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(PWM1_PWM_RUN_MODE         << PWM1_ONESHOT_SHIFT))
            
#define PWM1_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(PWM1_PWM_ALIGN            << PWM1_UPDOWN_SHIFT))

#define PWM1_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(PWM1_PWM_KILL_EVENT      << PWM1_PWM_SYNC_KILL_SHIFT))

#define PWM1_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(PWM1_PWM_DEAD_TIME_CYCLE  << PWM1_PRESCALER_SHIFT))

#define PWM1_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(PWM1_PWM_PRESCALER        << PWM1_PRESCALER_SHIFT))

#define PWM1_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(PWM1_TC_PRESCALER            << PWM1_PRESCALER_SHIFT))       |\
         ((uint32)(PWM1_TC_COUNTER_MODE         << PWM1_UPDOWN_SHIFT))          |\
         ((uint32)(PWM1_TC_RUN_MODE             << PWM1_ONESHOT_SHIFT))         |\
         ((uint32)(PWM1_TC_COMP_CAP_MODE        << PWM1_MODE_SHIFT)))
        
#define PWM1_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(PWM1_QUAD_PHIA_SIGNAL_MODE   << PWM1_COUNT_SHIFT))           |\
         ((uint32)(PWM1_QUAD_INDEX_SIGNAL_MODE  << PWM1_RELOAD_SHIFT))          |\
         ((uint32)(PWM1_QUAD_STOP_SIGNAL_MODE   << PWM1_STOP_SHIFT))            |\
         ((uint32)(PWM1_QUAD_PHIB_SIGNAL_MODE   << PWM1_START_SHIFT)))

#define PWM1_PWM_SIGNALS_MODES                                                              \
        (((uint32)(PWM1_PWM_SWITCH_SIGNAL_MODE  << PWM1_CAPTURE_SHIFT))         |\
         ((uint32)(PWM1_PWM_COUNT_SIGNAL_MODE   << PWM1_COUNT_SHIFT))           |\
         ((uint32)(PWM1_PWM_RELOAD_SIGNAL_MODE  << PWM1_RELOAD_SHIFT))          |\
         ((uint32)(PWM1_PWM_STOP_SIGNAL_MODE    << PWM1_STOP_SHIFT))            |\
         ((uint32)(PWM1_PWM_START_SIGNAL_MODE   << PWM1_START_SHIFT)))

#define PWM1_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(PWM1_TC_CAPTURE_SIGNAL_MODE  << PWM1_CAPTURE_SHIFT))         |\
         ((uint32)(PWM1_TC_COUNT_SIGNAL_MODE    << PWM1_COUNT_SHIFT))           |\
         ((uint32)(PWM1_TC_RELOAD_SIGNAL_MODE   << PWM1_RELOAD_SHIFT))          |\
         ((uint32)(PWM1_TC_STOP_SIGNAL_MODE     << PWM1_STOP_SHIFT))            |\
         ((uint32)(PWM1_TC_START_SIGNAL_MODE    << PWM1_START_SHIFT)))
        
#define PWM1_TIMER_UPDOWN_CNT_USED                                                          \
                ((PWM1__COUNT_UPDOWN0 == PWM1_TC_COUNTER_MODE)                  ||\
                 (PWM1__COUNT_UPDOWN1 == PWM1_TC_COUNTER_MODE))

#define PWM1_PWM_UPDOWN_CNT_USED                                                            \
                ((PWM1__CENTER == PWM1_PWM_ALIGN)                               ||\
                 (PWM1__ASYMMETRIC == PWM1_PWM_ALIGN))               
        
#define PWM1_PWM_PR_INIT_VALUE              (1u)
#define PWM1_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_PWM1_H */

/* [] END OF FILE */
