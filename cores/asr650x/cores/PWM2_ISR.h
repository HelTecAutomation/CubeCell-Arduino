/*******************************************************************************
* File Name: PWM2_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_PWM2_ISR_H)
#define CY_ISR_PWM2_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

    
#define PWM2_ISR__INTC_CLR_EN_REG CYREG_CM0P_ICER
#define PWM2_ISR__INTC_CLR_PD_REG CYREG_CM0P_ICPR
#define PWM2_ISR__INTC_MASK 0x800000u
#define PWM2_ISR__INTC_NUMBER 23u
#define PWM2_ISR__INTC_PRIOR_MASK 0xC0000000u
#define PWM2_ISR__INTC_PRIOR_NUM 3u
#define PWM2_ISR__INTC_PRIOR_REG CYREG_CM0P_IPR5
#define PWM2_ISR__INTC_SET_EN_REG CYREG_CM0P_ISER
#define PWM2_ISR__INTC_SET_PD_REG CYREG_CM0P_ISPR
    
/* Interrupt Controller API. */
void PWM2_ISR_Start(void);
void PWM2_ISR_StartEx(cyisraddress address);
void PWM2_ISR_Stop(void);

CY_ISR_PROTO(PWM2_ISR_Interrupt);

void PWM2_ISR_SetVector(cyisraddress address);
cyisraddress PWM2_ISR_GetVector(void);

void PWM2_ISR_SetPriority(uint8 priority);
uint8 PWM2_ISR_GetPriority(void);

void PWM2_ISR_Enable(void);
uint8 PWM2_ISR_GetState(void);
void PWM2_ISR_Disable(void);

void PWM2_ISR_SetPending(void);
void PWM2_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the PWM2_ISR ISR. */
#define PWM2_ISR_INTC_VECTOR            ((reg32 *) PWM2_ISR__INTC_VECT)

/* Address of the PWM2_ISR ISR priority. */
#define PWM2_ISR_INTC_PRIOR             ((reg32 *) PWM2_ISR__INTC_PRIOR_REG)

/* Priority of the PWM2_ISR interrupt. */
#define PWM2_ISR_INTC_PRIOR_NUMBER      PWM2_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable PWM2_ISR interrupt. */
#define PWM2_ISR_INTC_SET_EN            ((reg32 *) PWM2_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the PWM2_ISR interrupt. */
#define PWM2_ISR_INTC_CLR_EN            ((reg32 *) PWM2_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the PWM2_ISR interrupt state to pending. */
#define PWM2_ISR_INTC_SET_PD            ((reg32 *) PWM2_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the PWM2_ISR interrupt. */
#define PWM2_ISR_INTC_CLR_PD            ((reg32 *) PWM2_ISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_PWM2_ISR_H */


/* [] END OF FILE */
