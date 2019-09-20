/*******************************************************************************
* File Name: PWM2_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "PWM2.h"

static PWM2_BACKUP_STRUCT PWM2_backup;


/*******************************************************************************
* Function Name: PWM2_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM2_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM2_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM2_Sleep(void)
{
    if(0u != (PWM2_BLOCK_CONTROL_REG & PWM2_MASK))
    {
        PWM2_backup.enableState = 1u;
    }
    else
    {
        PWM2_backup.enableState = 0u;
    }

    PWM2_Stop();
    PWM2_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM2_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM2_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM2_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM2_Wakeup(void)
{
    PWM2_RestoreConfig();

    if(0u != PWM2_backup.enableState)
    {
        PWM2_Enable();
    }
}


/* [] END OF FILE */
