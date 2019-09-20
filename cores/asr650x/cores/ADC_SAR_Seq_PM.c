/*******************************************************************************
* File Name: ADC_SAR_Seq_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SAR_Seq.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_Seq_BACKUP_STRUCT  ADC_SAR_Seq_backup =
{
    ADC_SAR_Seq_DISABLED,
    0u    
};


/*******************************************************************************
* Function Name: ADC_SAR_Seq_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_Seq_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_Seq_RestoreConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_Seq_backup - modified.
*
*******************************************************************************/
void ADC_SAR_Seq_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    ADC_SAR_Seq_backup.dftRegVal = ADC_SAR_Seq_SAR_DFT_CTRL_REG & (uint32)~ADC_SAR_Seq_ADFT_OVERRIDE;
    ADC_SAR_Seq_SAR_DFT_CTRL_REG |= ADC_SAR_Seq_ADFT_OVERRIDE;
    if((ADC_SAR_Seq_SAR_CTRL_REG  & ADC_SAR_Seq_ENABLE) != 0u)
    {
        if((ADC_SAR_Seq_SAR_SAMPLE_CTRL_REG & ADC_SAR_Seq_CONTINUOUS_EN) != 0u)
        {
            ADC_SAR_Seq_backup.enableState = ADC_SAR_Seq_ENABLED | ADC_SAR_Seq_STARTED;
        }
        else
        {
            ADC_SAR_Seq_backup.enableState = ADC_SAR_Seq_ENABLED;
        }
        ADC_SAR_Seq_StopConvert();
        ADC_SAR_Seq_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((ADC_SAR_Seq_SAR_CTRL_REG & ADC_SAR_Seq_BOOSTPUMP_EN) != 0u)
        {
            ADC_SAR_Seq_SAR_CTRL_REG &= (uint32)~ADC_SAR_Seq_BOOSTPUMP_EN;
            ADC_SAR_Seq_backup.enableState |= ADC_SAR_Seq_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        ADC_SAR_Seq_backup.enableState = ADC_SAR_Seq_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_Seq_backup - used.
*
*******************************************************************************/
void ADC_SAR_Seq_Wakeup(void)
{
    ADC_SAR_Seq_SAR_DFT_CTRL_REG = ADC_SAR_Seq_backup.dftRegVal;
    if(ADC_SAR_Seq_backup.enableState != ADC_SAR_Seq_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((ADC_SAR_Seq_backup.enableState & ADC_SAR_Seq_BOOSTPUMP_ENABLED) != 0u)
        {
            ADC_SAR_Seq_SAR_CTRL_REG |= ADC_SAR_Seq_BOOSTPUMP_EN;
        }
        ADC_SAR_Seq_Enable();
        if((ADC_SAR_Seq_backup.enableState & ADC_SAR_Seq_STARTED) != 0u)
        {
            ADC_SAR_Seq_StartConvert();
        }
    }
}
/* [] END OF FILE */
