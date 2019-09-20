/*******************************************************************************
* File Name: ADC_SAR_Seq_intClock.c
* Version 2.20
*
*  Description:
*   Provides system API for the clocking, interrupts and watchdog timer.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <cydevice_trm.h>
#include "ADC_SAR_Seq_intClock.h"

#if defined CYREG_PERI_DIV_CMD

/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_StartEx
********************************************************************************
*
* Summary:
*  Starts the clock, aligned to the specified running clock.
*
* Parameters:
*  alignClkDiv:  The divider to which phase alignment is performed when the
*    clock is started.
*
* Returns:
*  None
*
*******************************************************************************/
void ADC_SAR_Seq_intClock_StartEx(uint32 alignClkDiv)
{
    /* Make sure any previous start command has finished. */
    while((ADC_SAR_Seq_intClock_CMD_REG & ADC_SAR_Seq_intClock_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and enable. */
    ADC_SAR_Seq_intClock_CMD_REG =
        ((uint32)ADC_SAR_Seq_intClock__DIV_ID << ADC_SAR_Seq_intClock_CMD_DIV_SHIFT)|
        (alignClkDiv << ADC_SAR_Seq_intClock_CMD_PA_DIV_SHIFT) |
        (uint32)ADC_SAR_Seq_intClock_CMD_ENABLE_MASK;
}

#else

/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_Start
********************************************************************************
*
* Summary:
*  Starts the clock.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/

void ADC_SAR_Seq_intClock_Start(void)
{
    /* Set the bit to enable the clock. */
    ADC_SAR_Seq_intClock_ENABLE_REG |= ADC_SAR_Seq_intClock__ENABLE_MASK;
}

#endif /* CYREG_PERI_DIV_CMD */


/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_Stop
********************************************************************************
*
* Summary:
*  Stops the clock and returns immediately. This API does not require the
*  source clock to be running but may return before the hardware is actually
*  disabled.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void ADC_SAR_Seq_intClock_Stop(void)
{
#if defined CYREG_PERI_DIV_CMD

    /* Make sure any previous start command has finished. */
    while((ADC_SAR_Seq_intClock_CMD_REG & ADC_SAR_Seq_intClock_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and disable. */
    ADC_SAR_Seq_intClock_CMD_REG =
        ((uint32)ADC_SAR_Seq_intClock__DIV_ID << ADC_SAR_Seq_intClock_CMD_DIV_SHIFT)|
        ((uint32)ADC_SAR_Seq_intClock_CMD_DISABLE_MASK);

#else

    /* Clear the bit to disable the clock. */
    ADC_SAR_Seq_intClock_ENABLE_REG &= (uint32)(~ADC_SAR_Seq_intClock__ENABLE_MASK);
    
#endif /* CYREG_PERI_DIV_CMD */
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_SetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Modifies the clock divider and the fractional divider.
*
* Parameters:
*  clkDivider:  Divider register value (0-65535). This value is NOT the
*    divider; the clock hardware divides by clkDivider plus one. For example,
*    to divide the clock by 2, this parameter should be set to 1.
*  fracDivider:  Fractional Divider register value (0-31).
* Returns:
*  None
*
*******************************************************************************/
void ADC_SAR_Seq_intClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional)
{
    uint32 maskVal;
    uint32 regVal;
    
#if defined (ADC_SAR_Seq_intClock__FRAC_MASK) || defined (CYREG_PERI_DIV_CMD)
    
	/* get all but divider bits */
    maskVal = ADC_SAR_Seq_intClock_DIV_REG & 
                    (uint32)(~(uint32)(ADC_SAR_Seq_intClock_DIV_INT_MASK | ADC_SAR_Seq_intClock_DIV_FRAC_MASK)); 
	/* combine mask and new divider vals into 32-bit value */
    regVal = maskVal |
        ((uint32)((uint32)clkDivider <<  ADC_SAR_Seq_intClock_DIV_INT_SHIFT) & ADC_SAR_Seq_intClock_DIV_INT_MASK) |
        ((uint32)((uint32)clkFractional << ADC_SAR_Seq_intClock_DIV_FRAC_SHIFT) & ADC_SAR_Seq_intClock_DIV_FRAC_MASK);
    
#else
    /* get all but integer divider bits */
    maskVal = ADC_SAR_Seq_intClock_DIV_REG & (uint32)(~(uint32)ADC_SAR_Seq_intClock__DIVIDER_MASK);
    /* combine mask and new divider val into 32-bit value */
    regVal = clkDivider | maskVal;
    
#endif /* ADC_SAR_Seq_intClock__FRAC_MASK || CYREG_PERI_DIV_CMD */

    ADC_SAR_Seq_intClock_DIV_REG = regVal;
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_GetDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Divide value of the clock minus 1. For example, if the clock is set to
*  divide by 2, the return value will be 1.
*
*******************************************************************************/
uint16 ADC_SAR_Seq_intClock_GetDividerRegister(void)
{
    return (uint16)((ADC_SAR_Seq_intClock_DIV_REG & ADC_SAR_Seq_intClock_DIV_INT_MASK)
        >> ADC_SAR_Seq_intClock_DIV_INT_SHIFT);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_intClock_GetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock fractional divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Fractional Divide value of the clock
*  0 if the fractional divider is not in use.
*
*******************************************************************************/
uint8 ADC_SAR_Seq_intClock_GetFractionalDividerRegister(void)
{
#if defined (ADC_SAR_Seq_intClock__FRAC_MASK)
    /* return fractional divider bits */
    return (uint8)((ADC_SAR_Seq_intClock_DIV_REG & ADC_SAR_Seq_intClock_DIV_FRAC_MASK)
        >> ADC_SAR_Seq_intClock_DIV_FRAC_SHIFT);
#else
    return 0u;
#endif /* ADC_SAR_Seq_intClock__FRAC_MASK */
}


/* [] END OF FILE */
