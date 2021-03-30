/*******************************************************************************
* File Name: PWM2.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the PWM2
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

#if !defined(CY_TCPWM_PWM2_H)
#define CY_TCPWM_PWM2_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* PWM2 */
#define PWM2_cy_m0s8_tcpwm_1__CC CYREG_TCPWM_CNT6_CC
#define PWM2_cy_m0s8_tcpwm_1__CC_BUFF CYREG_TCPWM_CNT6_CC_BUFF
#define PWM2_cy_m0s8_tcpwm_1__COUNTER CYREG_TCPWM_CNT6_COUNTER
#define PWM2_cy_m0s8_tcpwm_1__CTRL CYREG_TCPWM_CNT6_CTRL
#define PWM2_cy_m0s8_tcpwm_1__INTR CYREG_TCPWM_CNT6_INTR
#define PWM2_cy_m0s8_tcpwm_1__INTR_MASK CYREG_TCPWM_CNT6_INTR_MASK
#define PWM2_cy_m0s8_tcpwm_1__INTR_MASKED CYREG_TCPWM_CNT6_INTR_MASKED
#define PWM2_cy_m0s8_tcpwm_1__INTR_SET CYREG_TCPWM_CNT6_INTR_SET
#define PWM2_cy_m0s8_tcpwm_1__PERIOD CYREG_TCPWM_CNT6_PERIOD
#define PWM2_cy_m0s8_tcpwm_1__PERIOD_BUFF CYREG_TCPWM_CNT6_PERIOD_BUFF
#define PWM2_cy_m0s8_tcpwm_1__STATUS CYREG_TCPWM_CNT6_STATUS
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMD CYREG_TCPWM_CMD
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_MASK 0x40u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_SHIFT 6u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_MASK 0x4000u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_SHIFT 14u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_MASK 0x40000000u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_SHIFT 30u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_MASK 0x400000u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_SHIFT 22u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL CYREG_TCPWM_CTRL
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK 0x40u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL_SHIFT 6u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE CYREG_TCPWM_INTR_CAUSE
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_MASK 0x40u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_SHIFT 6u
#define PWM2_cy_m0s8_tcpwm_1__TCPWM_NUMBER 6u
#define PWM2_cy_m0s8_tcpwm_1__TR_CTRL0 CYREG_TCPWM_CNT6_TR_CTRL0
#define PWM2_cy_m0s8_tcpwm_1__TR_CTRL1 CYREG_TCPWM_CNT6_TR_CTRL1
#define PWM2_cy_m0s8_tcpwm_1__TR_CTRL2 CYREG_TCPWM_CNT6_TR_CTRL2
    
/* ClockPWM2 */
#define ClockPWM2__CTRL_REGISTER CYREG_PERI_PCLK_CTL12
#define ClockPWM2__DIV_ID 0x00000042u
#define ClockPWM2__DIV_REGISTER CYREG_PERI_DIV_16_CTL2
#define ClockPWM2__PA_DIV_ID 0x000000FFu   
    
    
/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} PWM2_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  PWM2_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define PWM2_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define PWM2_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define PWM2_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define PWM2_QUAD_ENCODING_MODES            (0lu)
#define PWM2_QUAD_AUTO_START                (0lu)

/* Signal modes */
#define PWM2_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define PWM2_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define PWM2_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define PWM2_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define PWM2_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define PWM2_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define PWM2_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define PWM2_TC_RUN_MODE                    (0lu)
#define PWM2_TC_COUNTER_MODE                (0lu)
#define PWM2_TC_COMP_CAP_MODE               (2lu)
#define PWM2_TC_PRESCALER                   (0lu)

/* Signal modes */
#define PWM2_TC_RELOAD_SIGNAL_MODE          (0lu)
#define PWM2_TC_COUNT_SIGNAL_MODE           (3lu)
#define PWM2_TC_START_SIGNAL_MODE           (0lu)
#define PWM2_TC_STOP_SIGNAL_MODE            (0lu)
#define PWM2_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWM2_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define PWM2_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define PWM2_TC_START_SIGNAL_PRESENT        (0lu)
#define PWM2_TC_STOP_SIGNAL_PRESENT         (0lu)
#define PWM2_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWM2_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define PWM2_PWM_KILL_EVENT                 (0lu)
#define PWM2_PWM_STOP_EVENT                 (0lu)
#define PWM2_PWM_MODE                       (4lu)
#define PWM2_PWM_OUT_N_INVERT               (0lu)
#define PWM2_PWM_OUT_INVERT                 (0lu)
#define PWM2_PWM_ALIGN                      (0lu)
#define PWM2_PWM_RUN_MODE                   (0lu)
#define PWM2_PWM_DEAD_TIME_CYCLE            (0lu)
#define PWM2_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define PWM2_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define PWM2_PWM_COUNT_SIGNAL_MODE          (3lu)
#define PWM2_PWM_START_SIGNAL_MODE          (0lu)
#define PWM2_PWM_STOP_SIGNAL_MODE           (0lu)
#define PWM2_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define PWM2_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define PWM2_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define PWM2_PWM_START_SIGNAL_PRESENT       (0lu)
#define PWM2_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define PWM2_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define PWM2_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define PWM2_TC_PERIOD_VALUE                (65535lu)
#define PWM2_TC_COMPARE_VALUE               (65535lu)
#define PWM2_TC_COMPARE_BUF_VALUE           (65535lu)
#define PWM2_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define PWM2_PWM_PERIOD_VALUE               (63000lu)
#define PWM2_PWM_PERIOD_BUF_VALUE           (65535lu)
#define PWM2_PWM_PERIOD_SWAP                (0lu)
#define PWM2_PWM_COMPARE_VALUE              (0lu)
#define PWM2_PWM_COMPARE_BUF_VALUE          (65535lu)
#define PWM2_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define PWM2__LEFT 0
#define PWM2__RIGHT 1
#define PWM2__CENTER 2
#define PWM2__ASYMMETRIC 3

#define PWM2__X1 0
#define PWM2__X2 1
#define PWM2__X4 2

#define PWM2__PWM 4
#define PWM2__PWM_DT 5
#define PWM2__PWM_PR 6

#define PWM2__INVERSE 1
#define PWM2__DIRECT 0

#define PWM2__CAPTURE 2
#define PWM2__COMPARE 0

#define PWM2__TRIG_LEVEL 3
#define PWM2__TRIG_RISING 0
#define PWM2__TRIG_FALLING 1
#define PWM2__TRIG_BOTH 2

#define PWM2__INTR_MASK_TC 1
#define PWM2__INTR_MASK_CC_MATCH 2
#define PWM2__INTR_MASK_NONE 0
#define PWM2__INTR_MASK_TC_CC 3

#define PWM2__UNCONFIG 8
#define PWM2__TIMER 1
#define PWM2__QUAD 3
#define PWM2__PWM_SEL 7

#define PWM2__COUNT_UP 0
#define PWM2__COUNT_DOWN 1
#define PWM2__COUNT_UPDOWN0 2
#define PWM2__COUNT_UPDOWN1 3


/* Prescaler */
#define PWM2_PRESCALE_DIVBY1                ((uint32)(0u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY2                ((uint32)(1u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY4                ((uint32)(2u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY8                ((uint32)(3u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY16               ((uint32)(4u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY32               ((uint32)(5u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY64               ((uint32)(6u << PWM2_PRESCALER_SHIFT))
#define PWM2_PRESCALE_DIVBY128              ((uint32)(7u << PWM2_PRESCALER_SHIFT))

/* TCPWM set modes */
#define PWM2_MODE_TIMER_COMPARE             ((uint32)(PWM2__COMPARE         <<  \
                                                                  PWM2_MODE_SHIFT))
#define PWM2_MODE_TIMER_CAPTURE             ((uint32)(PWM2__CAPTURE         <<  \
                                                                  PWM2_MODE_SHIFT))
#define PWM2_MODE_QUAD                      ((uint32)(PWM2__QUAD            <<  \
                                                                  PWM2_MODE_SHIFT))
#define PWM2_MODE_PWM                       ((uint32)(PWM2__PWM             <<  \
                                                                  PWM2_MODE_SHIFT))
#define PWM2_MODE_PWM_DT                    ((uint32)(PWM2__PWM_DT          <<  \
                                                                  PWM2_MODE_SHIFT))
#define PWM2_MODE_PWM_PR                    ((uint32)(PWM2__PWM_PR          <<  \
                                                                  PWM2_MODE_SHIFT))

/* Quad Modes */
#define PWM2_MODE_X1                        ((uint32)(PWM2__X1              <<  \
                                                                  PWM2_QUAD_MODE_SHIFT))
#define PWM2_MODE_X2                        ((uint32)(PWM2__X2              <<  \
                                                                  PWM2_QUAD_MODE_SHIFT))
#define PWM2_MODE_X4                        ((uint32)(PWM2__X4              <<  \
                                                                  PWM2_QUAD_MODE_SHIFT))

/* Counter modes */
#define PWM2_COUNT_UP                       ((uint32)(PWM2__COUNT_UP        <<  \
                                                                  PWM2_UPDOWN_SHIFT))
#define PWM2_COUNT_DOWN                     ((uint32)(PWM2__COUNT_DOWN      <<  \
                                                                  PWM2_UPDOWN_SHIFT))
#define PWM2_COUNT_UPDOWN0                  ((uint32)(PWM2__COUNT_UPDOWN0   <<  \
                                                                  PWM2_UPDOWN_SHIFT))
#define PWM2_COUNT_UPDOWN1                  ((uint32)(PWM2__COUNT_UPDOWN1   <<  \
                                                                  PWM2_UPDOWN_SHIFT))

/* PWM output invert */
#define PWM2_INVERT_LINE                    ((uint32)(PWM2__INVERSE         <<  \
                                                                  PWM2_INV_OUT_SHIFT))
#define PWM2_INVERT_LINE_N                  ((uint32)(PWM2__INVERSE         <<  \
                                                                  PWM2_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define PWM2_TRIG_RISING                    ((uint32)PWM2__TRIG_RISING)
#define PWM2_TRIG_FALLING                   ((uint32)PWM2__TRIG_FALLING)
#define PWM2_TRIG_BOTH                      ((uint32)PWM2__TRIG_BOTH)
#define PWM2_TRIG_LEVEL                     ((uint32)PWM2__TRIG_LEVEL)

/* Interrupt mask */
#define PWM2_INTR_MASK_TC                   ((uint32)PWM2__INTR_MASK_TC)
#define PWM2_INTR_MASK_CC_MATCH             ((uint32)PWM2__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define PWM2_CC_MATCH_SET                   (0x00u)
#define PWM2_CC_MATCH_CLEAR                 (0x01u)
#define PWM2_CC_MATCH_INVERT                (0x02u)
#define PWM2_CC_MATCH_NO_CHANGE             (0x03u)
#define PWM2_OVERLOW_SET                    (0x00u)
#define PWM2_OVERLOW_CLEAR                  (0x04u)
#define PWM2_OVERLOW_INVERT                 (0x08u)
#define PWM2_OVERLOW_NO_CHANGE              (0x0Cu)
#define PWM2_UNDERFLOW_SET                  (0x00u)
#define PWM2_UNDERFLOW_CLEAR                (0x10u)
#define PWM2_UNDERFLOW_INVERT               (0x20u)
#define PWM2_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define PWM2_PWM_MODE_LEFT                  (PWM2_CC_MATCH_CLEAR        |   \
                                                         PWM2_OVERLOW_SET           |   \
                                                         PWM2_UNDERFLOW_NO_CHANGE)
#define PWM2_PWM_MODE_RIGHT                 (PWM2_CC_MATCH_SET          |   \
                                                         PWM2_OVERLOW_NO_CHANGE     |   \
                                                         PWM2_UNDERFLOW_CLEAR)
#define PWM2_PWM_MODE_ASYM                  (PWM2_CC_MATCH_INVERT       |   \
                                                         PWM2_OVERLOW_SET           |   \
                                                         PWM2_UNDERFLOW_CLEAR)

#if (PWM2_CY_TCPWM_V2)
    #if(PWM2_CY_TCPWM_4000)
        #define PWM2_PWM_MODE_CENTER                (PWM2_CC_MATCH_INVERT       |   \
                                                                 PWM2_OVERLOW_NO_CHANGE     |   \
                                                                 PWM2_UNDERFLOW_CLEAR)
    #else
        #define PWM2_PWM_MODE_CENTER                (PWM2_CC_MATCH_INVERT       |   \
                                                                 PWM2_OVERLOW_SET           |   \
                                                                 PWM2_UNDERFLOW_CLEAR)
    #endif /* (PWM2_CY_TCPWM_4000) */
#else
    #define PWM2_PWM_MODE_CENTER                (PWM2_CC_MATCH_INVERT       |   \
                                                             PWM2_OVERLOW_NO_CHANGE     |   \
                                                             PWM2_UNDERFLOW_CLEAR)
#endif /* (PWM2_CY_TCPWM_NEW) */

/* Command operations without condition */
#define PWM2_CMD_CAPTURE                    (0u)
#define PWM2_CMD_RELOAD                     (8u)
#define PWM2_CMD_STOP                       (16u)
#define PWM2_CMD_START                      (24u)

/* Status */
#define PWM2_STATUS_DOWN                    (1u)
#define PWM2_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   PWM2_Init(void);
void   PWM2_Enable(void);
void   PWM2_Start(void);
void   PWM2_Stop(void);

void   PWM2_SetMode(uint32 mode);
void   PWM2_SetCounterMode(uint32 counterMode);
void   PWM2_SetPWMMode(uint32 modeMask);
void   PWM2_SetQDMode(uint32 qdMode);

void   PWM2_SetPrescaler(uint32 prescaler);
void   PWM2_TriggerCommand(uint32 mask, uint32 command);
void   PWM2_SetOneShot(uint32 oneShotEnable);
uint32 PWM2_ReadStatus(void);

void   PWM2_SetPWMSyncKill(uint32 syncKillEnable);
void   PWM2_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   PWM2_SetPWMDeadTime(uint32 deadTime);
void   PWM2_SetPWMInvert(uint32 mask);

void   PWM2_SetInterruptMode(uint32 interruptMask);
uint32 PWM2_GetInterruptSourceMasked(void);
uint32 PWM2_GetInterruptSource(void);
void   PWM2_ClearInterrupt(uint32 interruptMask);
void   PWM2_SetInterrupt(uint32 interruptMask);

void   PWM2_WriteCounter(uint32 count);
uint32 PWM2_ReadCounter(void);

uint32 PWM2_ReadCapture(void);
uint32 PWM2_ReadCaptureBuf(void);

void   PWM2_WritePeriod(uint32 period);
uint32 PWM2_ReadPeriod(void);
void   PWM2_WritePeriodBuf(uint32 periodBuf);
uint32 PWM2_ReadPeriodBuf(void);

void   PWM2_WriteCompare(uint32 compare);
uint32 PWM2_ReadCompare(void);
void   PWM2_WriteCompareBuf(uint32 compareBuf);
uint32 PWM2_ReadCompareBuf(void);

void   PWM2_SetPeriodSwap(uint32 swapEnable);
void   PWM2_SetCompareSwap(uint32 swapEnable);

void   PWM2_SetCaptureMode(uint32 triggerMode);
void   PWM2_SetReloadMode(uint32 triggerMode);
void   PWM2_SetStartMode(uint32 triggerMode);
void   PWM2_SetStopMode(uint32 triggerMode);
void   PWM2_SetCountMode(uint32 triggerMode);

void   PWM2_SaveConfig(void);
void   PWM2_RestoreConfig(void);
void   PWM2_Sleep(void);
void   PWM2_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define PWM2_BLOCK_CONTROL_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWM2_BLOCK_CONTROL_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define PWM2_COMMAND_REG                    (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWM2_COMMAND_PTR                    ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define PWM2_INTRRUPT_CAUSE_REG             (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWM2_INTRRUPT_CAUSE_PTR             ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define PWM2_CONTROL_REG                    (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__CTRL )
#define PWM2_CONTROL_PTR                    ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__CTRL )
#define PWM2_STATUS_REG                     (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__STATUS )
#define PWM2_STATUS_PTR                     ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__STATUS )
#define PWM2_COUNTER_REG                    (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__COUNTER )
#define PWM2_COUNTER_PTR                    ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__COUNTER )
#define PWM2_COMP_CAP_REG                   (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__CC )
#define PWM2_COMP_CAP_PTR                   ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__CC )
#define PWM2_COMP_CAP_BUF_REG               (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWM2_COMP_CAP_BUF_PTR               ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__CC_BUFF )
#define PWM2_PERIOD_REG                     (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__PERIOD )
#define PWM2_PERIOD_PTR                     ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__PERIOD )
#define PWM2_PERIOD_BUF_REG                 (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWM2_PERIOD_BUF_PTR                 ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define PWM2_TRIG_CONTROL0_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWM2_TRIG_CONTROL0_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define PWM2_TRIG_CONTROL1_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWM2_TRIG_CONTROL1_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define PWM2_TRIG_CONTROL2_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWM2_TRIG_CONTROL2_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define PWM2_INTERRUPT_REQ_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR )
#define PWM2_INTERRUPT_REQ_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR )
#define PWM2_INTERRUPT_SET_REG              (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_SET )
#define PWM2_INTERRUPT_SET_PTR              ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_SET )
#define PWM2_INTERRUPT_MASK_REG             (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWM2_INTERRUPT_MASK_PTR             ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_MASK )
#define PWM2_INTERRUPT_MASKED_REG           (*(reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_MASKED )
#define PWM2_INTERRUPT_MASKED_PTR           ( (reg32 *) PWM2_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define PWM2_MASK                           ((uint32)PWM2_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define PWM2_RELOAD_CC_SHIFT                (0u)
#define PWM2_RELOAD_PERIOD_SHIFT            (1u)
#define PWM2_PWM_SYNC_KILL_SHIFT            (2u)
#define PWM2_PWM_STOP_KILL_SHIFT            (3u)
#define PWM2_PRESCALER_SHIFT                (8u)
#define PWM2_UPDOWN_SHIFT                   (16u)
#define PWM2_ONESHOT_SHIFT                  (18u)
#define PWM2_QUAD_MODE_SHIFT                (20u)
#define PWM2_INV_OUT_SHIFT                  (20u)
#define PWM2_INV_COMPL_OUT_SHIFT            (21u)
#define PWM2_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define PWM2_RELOAD_CC_MASK                 ((uint32)(PWM2_1BIT_MASK        <<  \
                                                                            PWM2_RELOAD_CC_SHIFT))
#define PWM2_RELOAD_PERIOD_MASK             ((uint32)(PWM2_1BIT_MASK        <<  \
                                                                            PWM2_RELOAD_PERIOD_SHIFT))
#define PWM2_PWM_SYNC_KILL_MASK             ((uint32)(PWM2_1BIT_MASK        <<  \
                                                                            PWM2_PWM_SYNC_KILL_SHIFT))
#define PWM2_PWM_STOP_KILL_MASK             ((uint32)(PWM2_1BIT_MASK        <<  \
                                                                            PWM2_PWM_STOP_KILL_SHIFT))
#define PWM2_PRESCALER_MASK                 ((uint32)(PWM2_8BIT_MASK        <<  \
                                                                            PWM2_PRESCALER_SHIFT))
#define PWM2_UPDOWN_MASK                    ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                            PWM2_UPDOWN_SHIFT))
#define PWM2_ONESHOT_MASK                   ((uint32)(PWM2_1BIT_MASK        <<  \
                                                                            PWM2_ONESHOT_SHIFT))
#define PWM2_QUAD_MODE_MASK                 ((uint32)(PWM2_3BIT_MASK        <<  \
                                                                            PWM2_QUAD_MODE_SHIFT))
#define PWM2_INV_OUT_MASK                   ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                            PWM2_INV_OUT_SHIFT))
#define PWM2_MODE_MASK                      ((uint32)(PWM2_3BIT_MASK        <<  \
                                                                            PWM2_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define PWM2_CAPTURE_SHIFT                  (0u)
#define PWM2_COUNT_SHIFT                    (2u)
#define PWM2_RELOAD_SHIFT                   (4u)
#define PWM2_STOP_SHIFT                     (6u)
#define PWM2_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define PWM2_CAPTURE_MASK                   ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                  PWM2_CAPTURE_SHIFT))
#define PWM2_COUNT_MASK                     ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                  PWM2_COUNT_SHIFT))
#define PWM2_RELOAD_MASK                    ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                  PWM2_RELOAD_SHIFT))
#define PWM2_STOP_MASK                      ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                  PWM2_STOP_SHIFT))
#define PWM2_START_MASK                     ((uint32)(PWM2_2BIT_MASK        <<  \
                                                                  PWM2_START_SHIFT))

/* MASK */
#define PWM2_1BIT_MASK                      ((uint32)0x01u)
#define PWM2_2BIT_MASK                      ((uint32)0x03u)
#define PWM2_3BIT_MASK                      ((uint32)0x07u)
#define PWM2_6BIT_MASK                      ((uint32)0x3Fu)
#define PWM2_8BIT_MASK                      ((uint32)0xFFu)
#define PWM2_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define PWM2_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define PWM2_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(PWM2_QUAD_ENCODING_MODES     << PWM2_QUAD_MODE_SHIFT))       |\
         ((uint32)(PWM2_CONFIG                  << PWM2_MODE_SHIFT)))

#define PWM2_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(PWM2_PWM_STOP_EVENT          << PWM2_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(PWM2_PWM_OUT_INVERT          << PWM2_INV_OUT_SHIFT))         |\
         ((uint32)(PWM2_PWM_OUT_N_INVERT        << PWM2_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(PWM2_PWM_MODE                << PWM2_MODE_SHIFT)))

#define PWM2_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(PWM2_PWM_RUN_MODE         << PWM2_ONESHOT_SHIFT))
            
#define PWM2_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(PWM2_PWM_ALIGN            << PWM2_UPDOWN_SHIFT))

#define PWM2_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(PWM2_PWM_KILL_EVENT      << PWM2_PWM_SYNC_KILL_SHIFT))

#define PWM2_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(PWM2_PWM_DEAD_TIME_CYCLE  << PWM2_PRESCALER_SHIFT))

#define PWM2_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(PWM2_PWM_PRESCALER        << PWM2_PRESCALER_SHIFT))

#define PWM2_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(PWM2_TC_PRESCALER            << PWM2_PRESCALER_SHIFT))       |\
         ((uint32)(PWM2_TC_COUNTER_MODE         << PWM2_UPDOWN_SHIFT))          |\
         ((uint32)(PWM2_TC_RUN_MODE             << PWM2_ONESHOT_SHIFT))         |\
         ((uint32)(PWM2_TC_COMP_CAP_MODE        << PWM2_MODE_SHIFT)))
        
#define PWM2_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(PWM2_QUAD_PHIA_SIGNAL_MODE   << PWM2_COUNT_SHIFT))           |\
         ((uint32)(PWM2_QUAD_INDEX_SIGNAL_MODE  << PWM2_RELOAD_SHIFT))          |\
         ((uint32)(PWM2_QUAD_STOP_SIGNAL_MODE   << PWM2_STOP_SHIFT))            |\
         ((uint32)(PWM2_QUAD_PHIB_SIGNAL_MODE   << PWM2_START_SHIFT)))

#define PWM2_PWM_SIGNALS_MODES                                                              \
        (((uint32)(PWM2_PWM_SWITCH_SIGNAL_MODE  << PWM2_CAPTURE_SHIFT))         |\
         ((uint32)(PWM2_PWM_COUNT_SIGNAL_MODE   << PWM2_COUNT_SHIFT))           |\
         ((uint32)(PWM2_PWM_RELOAD_SIGNAL_MODE  << PWM2_RELOAD_SHIFT))          |\
         ((uint32)(PWM2_PWM_STOP_SIGNAL_MODE    << PWM2_STOP_SHIFT))            |\
         ((uint32)(PWM2_PWM_START_SIGNAL_MODE   << PWM2_START_SHIFT)))

#define PWM2_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(PWM2_TC_CAPTURE_SIGNAL_MODE  << PWM2_CAPTURE_SHIFT))         |\
         ((uint32)(PWM2_TC_COUNT_SIGNAL_MODE    << PWM2_COUNT_SHIFT))           |\
         ((uint32)(PWM2_TC_RELOAD_SIGNAL_MODE   << PWM2_RELOAD_SHIFT))          |\
         ((uint32)(PWM2_TC_STOP_SIGNAL_MODE     << PWM2_STOP_SHIFT))            |\
         ((uint32)(PWM2_TC_START_SIGNAL_MODE    << PWM2_START_SHIFT)))
        
#define PWM2_TIMER_UPDOWN_CNT_USED                                                          \
                ((PWM2__COUNT_UPDOWN0 == PWM2_TC_COUNTER_MODE)                  ||\
                 (PWM2__COUNT_UPDOWN1 == PWM2_TC_COUNTER_MODE))

#define PWM2_PWM_UPDOWN_CNT_USED                                                            \
                ((PWM2__CENTER == PWM2_PWM_ALIGN)                               ||\
                 (PWM2__ASYMMETRIC == PWM2_PWM_ALIGN))               
        
#define PWM2_PWM_PR_INIT_VALUE              (1u)
#define PWM2_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_PWM2_H */

/* [] END OF FILE */
