/**
 * \file Ifx_Cfg.h
 * \brief Configuration.
 *
 * \version iLLD_Demos_1_0_1_15_0
 * \copyright Copyright (c) 2019 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *
 * \defgroup IfxLld_Demo_HsslMultiple_SrcDoc_IlldConfig iLLD configuration
 * \ingroup IfxLld_Demo_HsslMultiple_SrcDoc
 */

/* The configuration macros mentioned below can be overridden in Ifx_Cfg.h:
* IFX_CFG_DISABLE_DEFAULT_INTRINSICS - IFX_CFG_DISABLE_DEFAULT_INTRINSICS shall be defined in Ifx_Cfg.h to disable the default intrinsics provided For Ex. NOP,CRC.
* IFX_CFG_USE_COMPILER_DEFAULT_LINKER - IFX_CFG_USE_COMPILER_DEFAULT_LINKER shall be defined in Ifx_Cfg.h to use the default compiler provided linker varaibles and startup
* IFX_USE_SW_MANAGED_INT - IFX_USE_SW_MANAGED_INT shall be defined in Ifx_Cfg.h to use the software based interrupts
* Application Start-up related Macros:
* IFX_CFG_SSW_ENABLE_TRICOREx (x=0-5) - IFX_CFG_SSW_ENABLE_TRICOREx shall be defined to 0 in Ifx_Cfg.h to disable the CPUx.
* IFX_CFG_SSW_ENABLE_TRICOREx_PCACHE (x=0-5) - IFX_CFG_SSW_ENABLE_TRICOREx_PCACHE shall be defined to 0 in Ifx_Cfg.h to disable the PCACHE of CPUx
* IFX_CFG_SSW_ENABLE_TRICOREx_DCACHE (x=0-5) - IFX_CFG_SSW_ENABLE_TRICOREx_DCACHE shall be defined to 0 in Ifx_Cfg.h to disable the DCACHE of CPUx
* SCU related Macros:
* IFX_CFG_SCU_XTAL_FREQUENCY - IFX_CFG_SCU_XTAL_FREQUENCY shall be defined to respective XTAL frequency in Ifx_Cfg.h
* IFX_CFG_SCU_PLL_FREQUENCY - IFX_CFG_SCU_PLL_FREQUENCY shall be defined to desired PLL frequency in Ifx_Cfg.h
* IFX_CFG_SCU_PLL1_FREQUENCY - IFX_CFG_SCU_PLL1_FREQUENCY shall be defined to desired PLL1 frequency in Ifx_Cfg.h
* IFX_CFG_SCU_PLL2_FREQUENCY - IFX_CFG_SCU_PLL2_FREQUENCY shall be defined to desired PLL2 frequency in Ifx_Cfg.h
*/

#ifndef IFX_CFG_H
#define IFX_CFG_H

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_HsslMultiple_SrcDoc_IlldConfig
 * \{ */

/*______________________________________________________________________________
** Configuration for IfxScu_cfg.h
**____________________________________________________________________________*/

/**
 * \name Frequency configuration
 * \{ */

#define IFX_CFG_SCU_XTAL_FREQUENCY (20000000)                       /**< \copydoc IFX_CFG_SCU_XTAL_FREQUENCY */

/** \} */

/** \} */

#endif /* IFX_CFG_H */
