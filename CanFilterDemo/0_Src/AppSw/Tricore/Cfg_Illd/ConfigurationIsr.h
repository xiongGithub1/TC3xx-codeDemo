/**
 * \file ConfigurationIsr.h
 * \brief Interrupts configuration.
 *
 * \version iLLD_Demos_1_0_1_15_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_CanFilter_SrcDoc_InterruptConfig Interrupt configuration
 * \ingroup IfxLld_Demo_CanFilter_SrcDoc
 */

#ifndef CONFIGURATIONISR_H
#define CONFIGURATIONISR_H

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Build the ISR configuration object
 * \param no interrupt priority
 * \param cpu assign CPU number
 */
#define ISR_ASSIGN(no, cpu)  ((no << 8) + cpu)

/** \brief extract the priority out of the ISR object */
#define ISR_PRIORITY(no_cpu) (no_cpu >> 8)

/** \brief extract the service provider  out of the ISR object */
#define ISR_PROVIDER(no_cpu) (no_cpu % 8)

/**
 * \addtogroup IfxLld_Demo_CanFilter_SrcDoc_InterruptConfig
 * \{ */

/**
 * \name Interrupt priority configuration.
 * The interrupt priority range is [1,255]
 * \{ */

#define ISR_PRIORITY_PRINTF_ASC0_TX 5   /**< \brief Define the ASC0 transmit interrupt priority used by printf.c */
#define ISR_PRIORITY_PRINTF_ASC0_EX 6   /**< \brief Define the ASC0 error interrupt priority used by printf.c */

#define ISR_PRIORITY_ASCLIN1_TX     4   /**< \brief Define the ASCLIN1 transmit interrupt priority.  */
#define ISR_PRIORITY_ASCLIN1_RX     8   /**< \brief Define the ASCLIN1 receive interrupt priority.  */
#define ISR_PRIORITY_ASCLIN1_ER     12  /**< \brief Define the ASCLIN1 error interrupt priority.  */

/** \} */

/**
 * \name Interrupt service provider configuration.
 * \{ */

#define ISR_PROVIDER_PRINTF_ASC0 IfxSrc_Tos_cpu0             /**< \brief Define the ASC0 interrupt provider used by printf.c   */

#define ISR_PROVIDER_ASCLIN1     IfxSrc_Tos_cpu0 /**< \brief Define the ASCLIN1 interrupt provider.  */

/** \} */

/**
 * \name Interrupt configuration.
 * \{ */

#define INTERRUPT_PRINTF_ASC0_TX    ISR_ASSIGN(ISR_PRIORITY_PRINTF_ASC0_TX, ISR_PROVIDER_PRINTF_ASC0) /**< \brief Define the ASC0 transmit interrupt priority used by printf.c */
#define INTERRUPT_PRINTF_ASC0_EX    ISR_ASSIGN(ISR_PRIORITY_PRINTF_ASC0_EX, ISR_PROVIDER_PRINTF_ASC0) /**< \brief Define the ASC0 error interrupt priority used by printf.c */

#define INTERRUPT_ASCLIN1_TX        ISR_ASSIGN(ISR_PRIORITY_ASCLIN1_TX, ISR_PROVIDER_ASCLIN1)         /**< \brief Install the ASCLIN1 transmit interrupt handler.  */
#define INTERRUPT_ASCLIN1_RX        ISR_ASSIGN(ISR_PRIORITY_ASCLIN1_RX, ISR_PROVIDER_ASCLIN1)         /**< \brief Install the ASCLIN1 receive interrupt handler.  */
#define INTERRUPT_ASCLIN1_ER        ISR_ASSIGN(ISR_PRIORITY_ASCLIN1_ER, ISR_PROVIDER_ASCLIN1)         /**< \brief Install the ASCLIN1 error interrupt handler.  */

/** \} */

/** \} */
//------------------------------------------------------------------------------

#endif
