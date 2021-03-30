/*******************************************************************************
* File Name: ADC_SAR_Seq.h
* Version 2.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Sequencing Successive Approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifdef __cplusplus 
	extern "C" {
#endif
 


#if !defined(CY_ADC_SAR_SEQ_ADC_SAR_Seq_H)
#define CY_ADC_SAR_SEQ_ADC_SAR_Seq_H

#include "cytypes.h"
#include "CyLib.h"
#include "ADC_SAR_Seq_IRQ.h"    
    
#define ADC_SAR_Seq_cy_psoc4_sar__CLOCK_DIV_ID 0x00000045u
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_ANA_TRIM CYREG_SAR_ANA_TRIM
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_AVG_STAT CYREG_SAR_AVG_STAT
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG00 CYREG_SAR_CHAN_CONFIG0
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG01 CYREG_SAR_CHAN_CONFIG1
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG02 CYREG_SAR_CHAN_CONFIG2
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG03 CYREG_SAR_CHAN_CONFIG3
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG04 CYREG_SAR_CHAN_CONFIG4
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG05 CYREG_SAR_CHAN_CONFIG5
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG06 CYREG_SAR_CHAN_CONFIG6
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG07 CYREG_SAR_CHAN_CONFIG7
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG08 CYREG_SAR_CHAN_CONFIG8
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG09 CYREG_SAR_CHAN_CONFIG9
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG10 CYREG_SAR_CHAN_CONFIG10
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG11 CYREG_SAR_CHAN_CONFIG11
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG12 CYREG_SAR_CHAN_CONFIG12
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG13 CYREG_SAR_CHAN_CONFIG13
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG14 CYREG_SAR_CHAN_CONFIG14
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG15 CYREG_SAR_CHAN_CONFIG15
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_EN CYREG_SAR_CHAN_EN
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT_VALID CYREG_SAR_CHAN_RESULT_VALID
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00 CYREG_SAR_CHAN_RESULT0
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT01 CYREG_SAR_CHAN_RESULT1
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT02 CYREG_SAR_CHAN_RESULT2
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT03 CYREG_SAR_CHAN_RESULT3
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT04 CYREG_SAR_CHAN_RESULT4
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT05 CYREG_SAR_CHAN_RESULT5
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT06 CYREG_SAR_CHAN_RESULT6
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT07 CYREG_SAR_CHAN_RESULT7
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT08 CYREG_SAR_CHAN_RESULT8
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT09 CYREG_SAR_CHAN_RESULT9
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT10 CYREG_SAR_CHAN_RESULT10
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT11 CYREG_SAR_CHAN_RESULT11
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT12 CYREG_SAR_CHAN_RESULT12
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT13 CYREG_SAR_CHAN_RESULT13
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT14 CYREG_SAR_CHAN_RESULT14
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT15 CYREG_SAR_CHAN_RESULT15
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK_VALID CYREG_SAR_CHAN_WORK_VALID
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK00 CYREG_SAR_CHAN_WORK0
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK01 CYREG_SAR_CHAN_WORK1
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK02 CYREG_SAR_CHAN_WORK2
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK03 CYREG_SAR_CHAN_WORK3
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK04 CYREG_SAR_CHAN_WORK4
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK05 CYREG_SAR_CHAN_WORK5
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK06 CYREG_SAR_CHAN_WORK6
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK07 CYREG_SAR_CHAN_WORK7
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK08 CYREG_SAR_CHAN_WORK8
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK09 CYREG_SAR_CHAN_WORK9
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK10 CYREG_SAR_CHAN_WORK10
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK11 CYREG_SAR_CHAN_WORK11
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK12 CYREG_SAR_CHAN_WORK12
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK13 CYREG_SAR_CHAN_WORK13
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK14 CYREG_SAR_CHAN_WORK14
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK15 CYREG_SAR_CHAN_WORK15
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_CTRL CYREG_SAR_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_DFT_CTRL CYREG_SAR_DFT_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR CYREG_SAR_INTR
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_CAUSE CYREG_SAR_INTR_CAUSE
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASK CYREG_SAR_INTR_MASK
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASKED CYREG_SAR_INTR_MASKED
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_SET CYREG_SAR_INTR_SET
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_CLEAR0 CYREG_SAR_MUX_SWITCH_CLEAR0
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_CLEAR1 CYREG_SAR_MUX_SWITCH_CLEAR1
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL CYREG_SAR_MUX_SWITCH_HW_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_STATUS CYREG_SAR_MUX_SWITCH_STATUS
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH0 CYREG_SAR_MUX_SWITCH0
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH1 CYREG_SAR_MUX_SWITCH1
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_NUMBER 0u
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_PUMP_CTRL CYREG_SAR_PUMP_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_COND CYREG_SAR_RANGE_COND
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR CYREG_SAR_RANGE_INTR
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASK CYREG_SAR_RANGE_INTR_MASK
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASKED CYREG_SAR_RANGE_INTR_MASKED
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_SET CYREG_SAR_RANGE_INTR_SET
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_THRES CYREG_SAR_RANGE_THRES
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_CTRL CYREG_SAR_SAMPLE_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME01 CYREG_SAR_SAMPLE_TIME01
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME23 CYREG_SAR_SAMPLE_TIME23
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR CYREG_SAR_SATURATE_INTR
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASK CYREG_SAR_SATURATE_INTR_MASK
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED CYREG_SAR_SATURATE_INTR_MASKED
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_SET CYREG_SAR_SATURATE_INTR_SET
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_START_CTRL CYREG_SAR_START_CTRL
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_STATUS CYREG_SAR_STATUS
#define ADC_SAR_Seq_cy_psoc4_sar__SAR_WOUNDING CYREG_SAR_WOUNDING

/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint32 dftRegVal;
} ADC_SAR_Seq_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define ADC_SAR_Seq__EXTERNAL 0
#define ADC_SAR_Seq__INTERNAL 1

/*  Sample Mode setting constants */
#define ADC_SAR_Seq__FREERUNNING 0
#define ADC_SAR_Seq__HARDWARESOC 1

/*  Reference type setting constants */
#define ADC_SAR_Seq__VDDA_2 0
#define ADC_SAR_Seq__VDDA 1
#define ADC_SAR_Seq__INTERNAL1024 2
#define ADC_SAR_Seq__INTERNAL1024BYPASSED 3
#define ADC_SAR_Seq__INTERNALVREF 4
#define ADC_SAR_Seq__INTERNALVREFBYPASSED 5
#define ADC_SAR_Seq__VDDA_2BYPASSED 6
#define ADC_SAR_Seq__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define ADC_SAR_Seq__DISABLED 0
#define ADC_SAR_Seq__ONE 1
#define ADC_SAR_Seq__TWO 2
#define ADC_SAR_Seq__FOUR 3
#define ADC_SAR_Seq__EIGHT 4
#define ADC_SAR_Seq__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define ADC_SAR_Seq__VSS 0
#define ADC_SAR_Seq__VREF 1
#define ADC_SAR_Seq__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define ADC_SAR_Seq__MODE0 0
#define ADC_SAR_Seq__MODE1 1
#define ADC_SAR_Seq__MODE2 2
#define ADC_SAR_Seq__MODE3 3

#define ADC_SAR_Seq__RES8 0
#define ADC_SAR_Seq__RES10 1

#define ADC_SAR_Seq__RIGHT 0
#define ADC_SAR_Seq__LEFT 1

#define ADC_SAR_Seq__FSIGNED 1
#define ADC_SAR_Seq__FUNSIGNED 0

#define ADC_SAR_Seq__ACCUMULATE 0
#define ADC_SAR_Seq__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define ADC_SAR_Seq_CY_SAR_IP_VER0               	(0u)
#define ADC_SAR_Seq_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define ADC_SAR_Seq_CY_SAR_IP_VER              (ADC_SAR_Seq_CY_SAR_IP_VER0)
#else /* Other devices */
    #define ADC_SAR_Seq_CY_SAR_IP_VER              (ADC_SAR_Seq_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define ADC_SAR_Seq_DEFAULT_SAMPLE_MODE_SEL        (1u)
#define ADC_SAR_Seq_DEFAULT_VREF_SEL               (4u)
#define ADC_SAR_Seq_DEFAULT_NEG_INPUT_SEL          (1u)
#define ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION_SEL     (1u)
#define ADC_SAR_Seq_DEFAULT_JUSTIFICATION_SEL      (0u)
#define ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define ADC_SAR_Seq_DEFAULT_CLOCK_SOURCE           (1u)
#define ADC_SAR_Seq_DEFAULT_VREF_MV_VALUE          (1200)
#define ADC_SAR_Seq_DEFAULT_BUFFER_GAIN            (0u)
#define ADC_SAR_Seq_DEFAULT_AVG_SAMPLES_NUM        (0u)
#define ADC_SAR_Seq_DEFAULT_AVG_SAMPLES_DIV        (0u < 4u) ? (int16)(0x100u >> (7u - 0u)) : (int16)(0x100u >> 4u)
#define ADC_SAR_Seq_DEFAULT_AVG_MODE               (1u)
#define ADC_SAR_Seq_MAX_RESOLUTION                 (12u)
#define ADC_SAR_Seq_DEFAULT_LOW_LIMIT              (0u)
#define ADC_SAR_Seq_DEFAULT_HIGH_LIMIT             (2047u)
#define ADC_SAR_Seq_DEFAULT_COMPARE_MODE           (0u)
#define ADC_SAR_Seq_DEFAULT_ACLKS_NUM              (6u)
#define ADC_SAR_Seq_DEFAULT_BCLKS_NUM              (2u)
#define ADC_SAR_Seq_DEFAULT_CCLKS_NUM              (2u)
#define ADC_SAR_Seq_DEFAULT_DCLKS_NUM              (2u)
#define ADC_SAR_Seq_TOTAL_CHANNELS_NUM             (1u)
#define ADC_SAR_Seq_SEQUENCED_CHANNELS_NUM         (1u)
#define ADC_SAR_Seq_DEFAULT_EN_CHANNELS            (1u)
#define ADC_SAR_Seq_NOMINAL_CLOCK_FREQ             (1000000)
#define ADC_SAR_Seq_INJ_CHANNEL_ENABLED            (0u)
#define ADC_SAR_Seq_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define ADC_SAR_Seq_SINGLE_PRESENT                 (0u)
#define ADC_SAR_Seq_CHANNELS_MODE                  (0u)
#define ADC_SAR_Seq_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - ADC_SAR_Seq_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void ADC_SAR_Seq_Start(void);
void ADC_SAR_Seq_Stop(void);
void ADC_SAR_Seq_Init(void);
void ADC_SAR_Seq_Enable(void);
void ADC_SAR_Seq_StartConvert(void);
void ADC_SAR_Seq_StopConvert(void);
uint32 ADC_SAR_Seq_IsEndConversion(uint32 retMode);
int16 ADC_SAR_Seq_GetResult16(uint32 chan);
void ADC_SAR_Seq_SetChanMask(uint32 mask);
void ADC_SAR_Seq_SetLowLimit(uint32 lowLimit);
void ADC_SAR_Seq_SetHighLimit(uint32 highLimit);
void ADC_SAR_Seq_SetLimitMask(uint32 mask);
void ADC_SAR_Seq_SetSatMask(uint32 mask);
void ADC_SAR_Seq_SetOffset(uint32 chan, int16 offset);
void ADC_SAR_Seq_SetGain(uint32 chan, int32 adcGain);
#if(ADC_SAR_Seq_INJ_CHANNEL_ENABLED)
    void ADC_SAR_Seq_EnableInjection(void);
#endif /* ADC_SAR_Seq_INJ_CHANNEL_ENABLED */
#if(ADC_SAR_Seq_DEFAULT_JUSTIFICATION_SEL == ADC_SAR_Seq__RIGHT)
    float ADC_SAR_Seq_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 ADC_SAR_Seq_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 ADC_SAR_Seq_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End ADC_SAR_Seq_DEFAULT_JUSTIFICATION_SEL == ADC_SAR_Seq__RIGHT */
void ADC_SAR_Seq_Sleep(void);
void ADC_SAR_Seq_Wakeup(void);
void ADC_SAR_Seq_SaveConfig(void);
void ADC_SAR_Seq_RestoreConfig(void);

CY_ISR_PROTO( ADC_SAR_Seq_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define ADC_SAR_Seq_DISABLED                   (0x00u)
#define ADC_SAR_Seq_ENABLED                    (0x01u)
#define ADC_SAR_Seq_STARTED                    (0x02u)
#define ADC_SAR_Seq_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define ADC_SAR_Seq_RETURN_STATUS              (0x01u)
#define ADC_SAR_Seq_WAIT_FOR_RESULT            (0x02u)
#define ADC_SAR_Seq_RETURN_STATUS_INJ          (0x04u)
#define ADC_SAR_Seq_WAIT_FOR_RESULT_INJ        (0x08u)

#define ADC_SAR_Seq_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define ADC_SAR_Seq_RESOLUTION_12              (12u)
#define ADC_SAR_Seq_RESOLUTION_10              (10u)
#define ADC_SAR_Seq_RESOLUTION_8               (8u)

#define ADC_SAR_Seq_10US_DELAY                 (10u)

#define ADC_SAR_Seq_10V_COUNTS                 (10.0F)
#define ADC_SAR_Seq_10MV_COUNTS                (10000)
#define ADC_SAR_Seq_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 ADC_SAR_Seq_initVar;
extern volatile int16 ADC_SAR_Seq_offset[ADC_SAR_Seq_TOTAL_CHANNELS_NUM];
extern volatile int32 ADC_SAR_Seq_countsPer10Volt[ADC_SAR_Seq_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define ADC_SAR_Seq_SAR_CTRL_REG                (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CTRL )
#define ADC_SAR_Seq_SAR_CTRL_PTR                ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CTRL )

#define ADC_SAR_Seq_SAR_SAMPLE_CTRL_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define ADC_SAR_Seq_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define ADC_SAR_Seq_SAR_SAMPLE_TIME01_REG       (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define ADC_SAR_Seq_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define ADC_SAR_Seq_SAR_SAMPLE_TIME23_REG       (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define ADC_SAR_Seq_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define ADC_SAR_Seq_SAR_RANGE_THRES_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_THRES )
#define ADC_SAR_Seq_SAR_RANGE_THRES_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_THRES )

#define ADC_SAR_Seq_SAR_RANGE_COND_REG          (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_COND )
#define ADC_SAR_Seq_SAR_RANGE_COND_PTR          ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_COND )

#define ADC_SAR_Seq_SAR_CHAN_EN_REG             (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_EN )
#define ADC_SAR_Seq_SAR_CHAN_EN_PTR             ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_EN )

#define ADC_SAR_Seq_SAR_START_CTRL_REG          (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_START_CTRL )
#define ADC_SAR_Seq_SAR_START_CTRL_PTR          ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_START_CTRL )

#define ADC_SAR_Seq_SAR_DFT_CTRL_REG            (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_DFT_CTRL )
#define ADC_SAR_Seq_SAR_DFT_CTRL_PTR            ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_DFT_CTRL )

#define ADC_SAR_Seq_SAR_CHAN_CONFIG_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define ADC_SAR_Seq_SAR_CHAN_CONFIG_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define ADC_SAR_Seq_SAR_CHAN_CONFIG_IND         ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define ADC_SAR_Seq_SAR_CHAN_WORK_REG           (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define ADC_SAR_Seq_SAR_CHAN_WORK_PTR           ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define ADC_SAR_Seq_SAR_CHAN_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC_SAR_Seq_SAR_CHAN_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC_SAR_Seq_SAR_CHAN_RESULT_IND         ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00

#define ADC_SAR_Seq_SAR_CHAN0_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC_SAR_Seq_SAR_CHAN0_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define ADC_SAR_Seq_SAR_CHAN1_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define ADC_SAR_Seq_SAR_CHAN1_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define ADC_SAR_Seq_SAR_CHAN2_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define ADC_SAR_Seq_SAR_CHAN2_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define ADC_SAR_Seq_SAR_CHAN3_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define ADC_SAR_Seq_SAR_CHAN3_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define ADC_SAR_Seq_SAR_CHAN4_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define ADC_SAR_Seq_SAR_CHAN4_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define ADC_SAR_Seq_SAR_CHAN5_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define ADC_SAR_Seq_SAR_CHAN5_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define ADC_SAR_Seq_SAR_CHAN6_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define ADC_SAR_Seq_SAR_CHAN6_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define ADC_SAR_Seq_SAR_CHAN7_RESULT_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define ADC_SAR_Seq_SAR_CHAN7_RESULT_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(ADC_SAR_Seq_CY_SAR_IP_VER != ADC_SAR_Seq_CY_SAR_IP_VER0)
    #define ADC_SAR_Seq_SAR_CHAN8_RESULT_REG     (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define ADC_SAR_Seq_SAR_CHAN8_RESULT_PTR     ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define ADC_SAR_Seq_SAR_CHAN9_RESULT_REG     (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define ADC_SAR_Seq_SAR_CHAN9_RESULT_PTR     ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define ADC_SAR_Seq_SAR_CHAN10_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define ADC_SAR_Seq_SAR_CHAN10_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define ADC_SAR_Seq_SAR_CHAN11_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define ADC_SAR_Seq_SAR_CHAN11_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define ADC_SAR_Seq_SAR_CHAN12_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define ADC_SAR_Seq_SAR_CHAN12_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define ADC_SAR_Seq_SAR_CHAN13_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define ADC_SAR_Seq_SAR_CHAN13_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define ADC_SAR_Seq_SAR_CHAN14_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define ADC_SAR_Seq_SAR_CHAN14_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define ADC_SAR_Seq_SAR_CHAN15_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define ADC_SAR_Seq_SAR_CHAN15_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (ADC_SAR_Seq_CY_SAR_IP_VER != ADC_SAR_Seq_CY_SAR_IP_VER0) */

#define ADC_SAR_Seq_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define ADC_SAR_Seq_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define ADC_SAR_Seq_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define ADC_SAR_Seq_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define ADC_SAR_Seq_SAR_STATUS_REG              (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_STATUS )
#define ADC_SAR_Seq_SAR_STATUS_PTR              ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_STATUS )

#define ADC_SAR_Seq_SAR_AVG_START_REG           (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_AVG_STAT )
#define ADC_SAR_Seq_SAR_AVG_START_PTR           ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_AVG_STAT )

#define ADC_SAR_Seq_SAR_INTR_REG                (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR )
#define ADC_SAR_Seq_SAR_INTR_PTR                ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR )

#define ADC_SAR_Seq_SAR_INTR_SET_REG            (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_SET )
#define ADC_SAR_Seq_SAR_INTR_SET_PTR            ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_SET )

#define ADC_SAR_Seq_SAR_INTR_MASK_REG           (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASK )
#define ADC_SAR_Seq_SAR_INTR_MASK_PTR           ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASK )

#define ADC_SAR_Seq_SAR_INTR_MASKED_REG         (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASKED )
#define ADC_SAR_Seq_SAR_INTR_MASKED_PTR         ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_MASKED )

#define ADC_SAR_Seq_SAR_SATURATE_INTR_REG       (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR )
#define ADC_SAR_Seq_SAR_SATURATE_INTR_PTR       ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR )

#define ADC_SAR_Seq_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define ADC_SAR_Seq_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define ADC_SAR_Seq_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define ADC_SAR_Seq_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define ADC_SAR_Seq_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define ADC_SAR_Seq_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define ADC_SAR_Seq_SAR_RANGE_INTR_REG          (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR )
#define ADC_SAR_Seq_SAR_RANGE_INTR_PTR          ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR )

#define ADC_SAR_Seq_SAR_RANGE_INTR_SET_REG      (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define ADC_SAR_Seq_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define ADC_SAR_Seq_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define ADC_SAR_Seq_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define ADC_SAR_Seq_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define ADC_SAR_Seq_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define ADC_SAR_Seq_SAR_INTR_CAUSE_REG          (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_CAUSE )
#define ADC_SAR_Seq_SAR_INTR_CAUSE_PTR          ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(ADC_SAR_Seq_INJ_CHANNEL_ENABLED)
    #define ADC_SAR_Seq_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define ADC_SAR_Seq_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define ADC_SAR_Seq_SAR_INJ_RESULT_REG    (*(reg32 *) ADC_SAR_Seq_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define ADC_SAR_Seq_SAR_INJ_RESULT_PTR    ( (reg32 *) ADC_SAR_Seq_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* ADC_SAR_Seq_INJ_CHANNEL_ENABLED */
    
#define ADC_SAR_Seq_MUX_SWITCH0_REG           (*(reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define ADC_SAR_Seq_MUX_SWITCH0_PTR           ( (reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define ADC_SAR_Seq_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define ADC_SAR_Seq_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define ADC_SAR_Seq_PUMP_CTRL_REG             (*(reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_PUMP_CTRL )
#define ADC_SAR_Seq_PUMP_CTRL_PTR             ( (reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_PUMP_CTRL )

#define ADC_SAR_Seq_ANA_TRIM_REG              (*(reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_ANA_TRIM )
#define ADC_SAR_Seq_ANA_TRIM_PTR              ( (reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_ANA_TRIM )

#define ADC_SAR_Seq_WOUNDING_REG              (*(reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_WOUNDING )
#define ADC_SAR_Seq_WOUNDING_PTR              ( (reg32 *)  ADC_SAR_Seq_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define ADC_SAR_Seq_INTC_NUMBER                (ADC_SAR_Seq_IRQ__INTC_NUMBER)
#define ADC_SAR_Seq_INTC_PRIOR_NUMBER          (ADC_SAR_Seq_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define ADC_SAR_Seq_VREF_INTERNAL1024          (0x00000040Lu)
#define ADC_SAR_Seq_VREF_EXTERNAL              (0x00000050Lu)
#define ADC_SAR_Seq_VREF_VDDA_2                (0x00000060Lu)
#define ADC_SAR_Seq_VREF_VDDA                  (0x00000070Lu)
#define ADC_SAR_Seq_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define ADC_SAR_Seq_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define ADC_SAR_Seq_VREF_INTERNALVREF          (0x00000040Lu)
#define ADC_SAR_Seq_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define ADC_SAR_Seq_NEG_VSSA_KELVIN            (0x00000000Lu)
#define ADC_SAR_Seq_NEG_VSSA                   (0x00000200Lu)
#define ADC_SAR_Seq_NEG_VREF                   (0x00000E00Lu)
#if(ADC_SAR_Seq_TOTAL_CHANNELS_NUM > 1u)
    #define ADC_SAR_Seq_NEG_OTHER              (uint16)((uint16)ADC_SAR_Seq_cy_psoc4_sarmux_8__VNEG0 << 9u)
#else
    #define ADC_SAR_Seq_NEG_OTHER              (0)
#endif /* ADC_SAR_Seq_TOTAL_CHANNELS_NUM > 1u */

#define ADC_SAR_Seq_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define ADC_SAR_Seq_BOOSTPUMP_EN               (0x00100000Lu)

#define ADC_SAR_Seq_NORMAL_PWR                 (0x00000000Lu)
#define ADC_SAR_Seq_HALF_PWR                   (0x01000000Lu)
#define ADC_SAR_Seq_MORE_PWR                   (0x02000000Lu)
#define ADC_SAR_Seq_QUARTER_PWR                (0x03000000Lu)
#define ADC_SAR_Seq_DEEPSLEEP_ON               (0x08000000Lu)

#define ADC_SAR_Seq_DSI_SYNC_CONFIG            (0x10000000Lu)
#define ADC_SAR_Seq_DSI_MODE                   (0x20000000Lu)
#define ADC_SAR_Seq_SWITCH_DISABLE             (0x40000000Lu)
#define ADC_SAR_Seq_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define ADC_SAR_Seq_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define ADC_SAR_Seq_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define ADC_SAR_Seq_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define ADC_SAR_Seq_DATA_ALIGN_LEFT            (0x00000002Lu)
#define ADC_SAR_Seq_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define ADC_SAR_Seq_SE_SIGNED_RESULT           (0x00000004Lu)
#define ADC_SAR_Seq_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define ADC_SAR_Seq_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define ADC_SAR_Seq_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define ADC_SAR_Seq_AVG_CNT_OFFSET             (4u)
#define ADC_SAR_Seq_AVG_CNT_MASK               (0x00000070Lu)
#define ADC_SAR_Seq_AVG_SHIFT                  (0x00000080Lu)

#define ADC_SAR_Seq_CONTINUOUS_EN              (0x00010000Lu)
#define ADC_SAR_Seq_DSI_TRIGGER_EN             (0x00020000Lu)
#define ADC_SAR_Seq_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define ADC_SAR_Seq_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define ADC_SAR_Seq_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define ADC_SAR_Seq_SAMPLE_TIME13_OFFSET       (16u)
#define ADC_SAR_Seq_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define ADC_SAR_Seq_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define ADC_SAR_Seq_RANGE_HIGH_OFFSET          (16u)
#define ADC_SAR_Seq_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define ADC_SAR_Seq_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define ADC_SAR_Seq_CMP_MODE_BELOW             (0x00000000Lu)
#define ADC_SAR_Seq_CMP_MODE_INSIDE            (0x40000000Lu)
#define ADC_SAR_Seq_CMP_MODE_ABOVE             (0x80000000Lu)
#define ADC_SAR_Seq_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define ADC_SAR_Seq_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define ADC_SAR_Seq_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define ADC_SAR_Seq_DLY_INC                    (0x00000001Lu)
#define ADC_SAR_Seq_HIZ                        (0x00000002Lu)
#define ADC_SAR_Seq_DFT_INC_MASK               (0x000F0000Lu)
#define ADC_SAR_Seq_DFT_OUTC_MASK              (0x00700000Lu)
#define ADC_SAR_Seq_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define ADC_SAR_Seq_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define ADC_SAR_Seq_EN_CSEL_DFT                (0x10000000Lu)
#define ADC_SAR_Seq_DCEN                       (0x20000000Lu)
#define ADC_SAR_Seq_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define ADC_SAR_Seq_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define ADC_SAR_Seq_DIFFERENTIAL_EN            (0x00000100Lu)
#define ADC_SAR_Seq_ALT_RESOLUTION_ON          (0x00000200Lu)
#define ADC_SAR_Seq_AVERAGING_EN               (0x00000400Lu)

#define ADC_SAR_Seq_SAMPLE_TIME_SEL_SHIFT      (12u)
#define ADC_SAR_Seq_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define ADC_SAR_Seq_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define ADC_SAR_Seq_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define ADC_SAR_Seq_INJ_TAILGATING             (0x40000000Lu)
#define ADC_SAR_Seq_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define ADC_SAR_Seq_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define ADC_SAR_Seq_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define ADC_SAR_Seq_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define ADC_SAR_Seq_RESULT_MASK                (0x0000FFFFLu)
#define ADC_SAR_Seq_SATURATE_INTR_MIR          (0x20000000Lu)
#define ADC_SAR_Seq_RANGE_INTR_MIR             (0x40000000Lu)
#define ADC_SAR_Seq_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define ADC_SAR_Seq_EOS_MASK                   (0x00000001Lu)
#define ADC_SAR_Seq_OVERFLOW_MASK              (0x00000002Lu)
#define ADC_SAR_Seq_FW_COLLISION_MASK          (0x00000004Lu)
#define ADC_SAR_Seq_DSI_COLLISION_MASK         (0x00000008Lu)
#define ADC_SAR_Seq_INJ_EOC_MASK               (0x00000010Lu)
#define ADC_SAR_Seq_INJ_SATURATE_MASK          (0x00000020Lu)
#define ADC_SAR_Seq_INJ_RANGE_MASK             (0x00000040Lu)
#define ADC_SAR_Seq_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define ADC_SAR_Seq_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define ADC_SAR_Seq_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define ADC_SAR_Seq_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define ADC_SAR_Seq_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define ADC_SAR_Seq_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define ADC_SAR_Seq_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define ADC_SAR_Seq_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define ADC_SAR_Seq_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define ADC_SAR_Seq_WOUNDING_12BIT             (0x00000000Lu)
#define ADC_SAR_Seq_WOUNDING_10BIT             (0x00000001Lu)
#define ADC_SAR_Seq_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define ADC_SAR_Seq_TRIM_COEF                  (2u)

#if(ADC_SAR_Seq_MAX_RESOLUTION == ADC_SAR_Seq_RESOLUTION_10)
    #define ADC_SAR_Seq_ALT_WOUNDING           ADC_SAR_Seq_WOUNDING_10BIT
#else
    #define ADC_SAR_Seq_ALT_WOUNDING           ADC_SAR_Seq_WOUNDING_8BIT
#endif /* ADC_SAR_Seq_MAX_RESOLUTION == ADC_SAR_Seq_RESOLUTION_10 */

#if(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__VDDA_2)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_VDDA_2
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__VDDA)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_VDDA
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__INTERNAL1024)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_INTERNAL1024
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__INTERNAL1024BYPASSED)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_INTERNAL1024BYPASSED
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__INTERNALVREF)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_INTERNALVREF
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__INTERNALVREFBYPASSED)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_INTERNALVREFBYPASSED
#elif(ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__VDDA_2BYPASSED)
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_VDDA_2BYPASSED
#else
    #define ADC_SAR_Seq_DEFAULT_VREF_SOURCE    ADC_SAR_Seq_VREF_EXTERNAL
#endif /* ADC_SAR_Seq_DEFAULT_VREF_SEL == ADC_SAR_Seq__VDDA_2 */

#if(ADC_SAR_Seq_DEFAULT_NEG_INPUT_SEL == ADC_SAR_Seq__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u)
        #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    ADC_SAR_Seq_NEG_VSSA
    #else
        #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    ADC_SAR_Seq_NEG_VSSA_KELVIN
    #endif /* (ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) && (ADC_SAR_Seq_CHANNELS_MODE != 0u))
        #define ADC_SAR_Seq_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define ADC_SAR_Seq_DEFAULT_MUX_SWITCH0     ADC_SAR_Seq_MUX_FW_VSSA_VMINUS
    #endif /* (ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) */
#elif(ADC_SAR_Seq_DEFAULT_NEG_INPUT_SEL == ADC_SAR_Seq__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) && (ADC_SAR_Seq_CHANNELS_MODE != 0u))
        #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    ADC_SAR_Seq_NEG_VREF
    #endif /* (ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) */
    #define ADC_SAR_Seq_DEFAULT_MUX_SWITCH0     0u
#elif (ADC_SAR_Seq_SINGLE_PRESENT != 0u)
    #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    ADC_SAR_Seq_NEG_OTHER
    #define ADC_SAR_Seq_DEFAULT_MUX_SWITCH0     0u
#else
    #define ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT    0u
    #define ADC_SAR_Seq_DEFAULT_MUX_SWITCH0     0u
#endif /* ADC_SAR_Seq_DEFAULT_NEG_INPUT_SEL == ADC_SAR_Seq__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u)
    #define ADC_SAR_Seq_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define ADC_SAR_Seq_DEFAULT_HW_CTRL_NEGVREF ADC_SAR_Seq_SAR_HW_CTRL_NEGVREF
#endif /* (ADC_SAR_Seq_TOTAL_CHANNELS_NUM == 1u) */


#if(ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION_SEL == ADC_SAR_Seq__RES8)
    #define ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION     (ADC_SAR_Seq_ALT_RESOLUTION_8BIT)
    #define ADC_SAR_Seq_DEFAULT_MAX_WRK_ALT        (ADC_SAR_Seq_SAR_WRK_MAX_8BIT)
#else
    #define ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION     (ADC_SAR_Seq_ALT_RESOLUTION_10BIT)
    #define ADC_SAR_Seq_DEFAULT_MAX_WRK_ALT        (ADC_SAR_Seq_SAR_WRK_MAX_10BIT)
#endif /* End ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION_SEL == ADC_SAR_Seq__RES8 */

#if(ADC_SAR_Seq_DEFAULT_JUSTIFICATION_SEL == ADC_SAR_Seq__RIGHT)
    #define ADC_SAR_Seq_DEFAULT_JUSTIFICATION  ADC_SAR_Seq_DATA_ALIGN_RIGHT
#else
    #define ADC_SAR_Seq_DEFAULT_JUSTIFICATION  ADC_SAR_Seq_DATA_ALIGN_LEFT
#endif /* ADC_SAR_Seq_DEFAULT_JUSTIFICATION_SEL == ADC_SAR_Seq__RIGHT */

#if(ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT_SEL == ADC_SAR_Seq__FSIGNED)
    #define ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT ADC_SAR_Seq_DIFF_SIGNED_RESULT
#else
    #define ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT ADC_SAR_Seq_DIFF_UNSIGNED_RESULT
#endif /* ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT_SEL == ADC_SAR_Seq__FSIGNED */

#if(ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT_SEL == ADC_SAR_Seq__FSIGNED)
    #define ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT ADC_SAR_Seq_SE_SIGNED_RESULT
#else
    #define ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT ADC_SAR_Seq_SE_UNSIGNED_RESULT
#endif /* ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT_SEL == ADC_SAR_Seq__FSIGNED */

#if(ADC_SAR_Seq_DEFAULT_SAMPLE_MODE_SEL == ADC_SAR_Seq__FREERUNNING)
    #define ADC_SAR_Seq_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define ADC_SAR_Seq_DSI_TRIGGER        (ADC_SAR_Seq_DSI_TRIGGER_EN | ADC_SAR_Seq_DSI_SYNC_TRIGGER)
#endif /* End ADC_SAR_Seq_DEFAULT_SAMPLE_MODE == ADC_SAR_Seq__FREERUNNING */

#if(ADC_SAR_Seq_INJ_CHANNEL_ENABLED)
    #define ADC_SAR_Seq_SAR_INTR_MASK      (ADC_SAR_Seq_EOS_MASK | ADC_SAR_Seq_INJ_EOC_MASK)
#else
    #define ADC_SAR_Seq_SAR_INTR_MASK      (ADC_SAR_Seq_EOS_MASK)
#endif /* ADC_SAR_Seq_INJ_CHANNEL_ENABLED*/

#if(ADC_SAR_Seq_DEFAULT_AVG_MODE == ADC_SAR_Seq__FIXEDRESOLUTION)
    #define ADC_SAR_Seq_AVG_SHIFT_MODE     ADC_SAR_Seq_AVG_SHIFT
#else
    #define ADC_SAR_Seq_AVG_SHIFT_MODE     0u
#endif /* End ADC_SAR_Seq_DEFAULT_AVG_MODE */

#define ADC_SAR_Seq_COMPARE_MODE           (uint32)((uint32)(ADC_SAR_Seq_DEFAULT_COMPARE_MODE) \
                                                << ADC_SAR_Seq_CMP_OFFSET)

#if(ADC_SAR_Seq_TOTAL_CHANNELS_NUM > 1u)
    #define ADC_SAR_Seq_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define ADC_SAR_Seq_DEFAULT_SWITCH_CONF    ADC_SAR_Seq_SWITCH_DISABLE
#endif /* End ADC_SAR_Seq_TOTAL_CHANNELS_NUM > 1 */

#define ADC_SAR_Seq_DEFAULT_POWER \
       ((ADC_SAR_Seq_NOMINAL_CLOCK_FREQ > (ADC_SAR_Seq_MAX_FREQUENCY / 4)) ? ADC_SAR_Seq_NORMAL_PWR : \
       ((ADC_SAR_Seq_NOMINAL_CLOCK_FREQ > (ADC_SAR_Seq_MAX_FREQUENCY / 8)) ? ADC_SAR_Seq_HALF_PWR : \
                                                                                       ADC_SAR_Seq_QUARTER_PWR))

#define ADC_SAR_Seq_DEFAULT_CTRL_REG_CFG       (ADC_SAR_Seq_DEFAULT_VREF_SOURCE \
                                                   | ADC_SAR_Seq_DEFAULT_SE_NEG_INPUT \
                                                   | ADC_SAR_Seq_DEFAULT_HW_CTRL_NEGVREF \
                                                   | ADC_SAR_Seq_DEFAULT_POWER \
                                                   | ADC_SAR_Seq_DSI_SYNC_CONFIG \
                                                   | ADC_SAR_Seq_DEFAULT_SWITCH_CONF)

#define ADC_SAR_Seq_DEFAULT_SAMPLE_CTRL_REG_CFG (ADC_SAR_Seq_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | ADC_SAR_Seq_DEFAULT_SE_RESULT_FORMAT \
                                                    | ADC_SAR_Seq_DEFAULT_JUSTIFICATION \
                                                    | ADC_SAR_Seq_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(ADC_SAR_Seq_DEFAULT_AVG_SAMPLES_NUM \
                                                   << ADC_SAR_Seq_AVG_CNT_OFFSET) \
                                                    | ADC_SAR_Seq_AVG_SHIFT_MODE \
                                                    | ADC_SAR_Seq_DSI_TRIGGER \
                                                    | ADC_SAR_Seq_EOS_DSI_OUT_EN)

#define ADC_SAR_Seq_DEFAULT_RANGE_THRES_REG_CFG (ADC_SAR_Seq_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)ADC_SAR_Seq_DEFAULT_HIGH_LIMIT << ADC_SAR_Seq_RANGE_HIGH_OFFSET))

#define ADC_SAR_Seq_DEFAULT_SAMPLE_TIME01_REG_CFG (ADC_SAR_Seq_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)ADC_SAR_Seq_DEFAULT_BCLKS_NUM << ADC_SAR_Seq_SAMPLE_TIME13_OFFSET))

#define ADC_SAR_Seq_DEFAULT_SAMPLE_TIME23_REG_CFG (ADC_SAR_Seq_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)ADC_SAR_Seq_DEFAULT_DCLKS_NUM << ADC_SAR_Seq_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_ADC_SAR_Seq_H */

#ifdef __cplusplus 
}
#endif


/* [] END OF FILE */
