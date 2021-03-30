/**
 ******************************************************************************
 * @file    sec_regs.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for secure algorithm registers
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRYPTO
 * @{
 * @defgroup REGS
 * @{
 */
 
#ifndef WYSEC_H
#define WYSEC_H

#include "type_define.h"


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

//---------------- custom MUST and ONLY need redefine these ---------
#define WYSEC_ARAM_MIX_BASE     (0x40030000) /*!< ARAM0 base address */
#define WYSEC_ARAM_ALONG_BASE   (0x40031000) /*!< ARAM1 base address */

#define WYSEC_AHB_BASE          (0x40032000) /*!< SAE AHB base address */

#define WYSEC_SAE_BASE       (WYSEC_AHB_BASE + 0x00000000)  /*!< SAE base address */
#define WYSEC_RNG_BASE       (WYSEC_AHB_BASE + 0x00001000)  /*!< RNG base address */
//-------------------------------------------------------------------

/* testbench monitor address when simulation verification */
#define WYSEC_DBG_BASE       (WYSEC_ARAM_MIX_BASE + 0xf00)   /*!< DBG base address */

#define SAE             ((sae_t      *) WYSEC_SAE_BASE    )  /*!< SAE peripheral */
#define RNG             ((rng_t      *) WYSEC_RNG_BASE    )  /*!< RNG peripheral */
#define DBG             ((dbg_t      *) WYSEC_DBG_BASE    )  /*!< DBG peripheral */


/*---------------------------------------------------------------------------*/
/*                        DEBUG(for printf debug)                            */
/*---------------------------------------------------------------------------*/
typedef struct
{
  volatile   UINT32  PRINT;  /*!< Addr:0x3000_0F00 (R/W) */
  volatile   UINT32  SIMEND; /*!< Addr:0x3000_0F04 (R/W) */
	
} dbg_t;
   
#endif  /* WYSEC_H */

/**
 * @} 
 * @}
 * @}
 */
