/**
 * \file IomDemo.h
 * \brief Iom Demo
 * \version iLLD_Demos_1_0_1_15_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_IomDemo_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_IomDemo_SrcDoc
 * \defgroup IfxLld_Demo_IomDemo_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_IomDemo_SrcDoc_Main
 */

#ifndef IOMDEMO_H
#define IOMDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>
#include "ConfigurationIsr.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "Iom/Iom/IfxIom_Iom.h"
#include <Gtm/Tom/PwmHl/IfxGtm_Tom_PwmHl.h>
#include "_Utilities/Ifx_Assert.h"
#include "IfxGtm_regdef.h"
#include <stdio.h>
#include "Cpu0_Main.h"
#include "IfxSmu_reg.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
/* PWM configuration*/
#define CFG_PWM_FREQUENCYREF                  (10001)
#define CFG_PWM_FREQUENCYMON                  (10000)
#define CFG_PWM_ISR_PROVIDER                  (IfxSrc_Tos_cpu0)
#define CFG_PWM_TRIGGER_RISING_EDGE_AT_PERIOD (TRUE)
#define CFG_PWM_TRIGGER_DUTYCYCLE             (0.5)
#define CFG_PWM_DEADTIME                      (2e-6)
#define CFG_PWM_MIN_PULSE                     (1e-6)
#define CFG_PWM_CCX_ACTIVE_STATE              (Ifx_ActiveState_high)
#define CFG_PWM_COUTX_ACTIVE_STATE            (Ifx_ActiveState_high)
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

typedef struct
{
    IfxIom_Iom_EcmConfig ecmConfig;    /**< \brief Event Combiner Module Configuration structure */
    IfxIom_Iom_FpcConfig fpcConfig;    /**< \brief Filter & Prescaler  Configuration structure */
    IfxIom_Iom_LamConfig lamConfig;    /**< \brief Logical Analayser Module Configuration structure */
    IfxIom_Iom           iom;          /**< \brief Iom handle */

    volatile unsigned    counterTimer; /**< \brief counter to track alarm signal   */

    IfxGtm_Tom_Timer     tomTimerref;  /**< \brief Iom Reference signal handle */
    IfxGtm_Tom_PwmHl     tomPwmref;    /**< \brief pwm handle */

    IfxGtm_Tom_Timer     tomTimermon;  /**< \brief Iom Monitor signal handle */
    IfxGtm_Tom_PwmHl     tomPwmmon;    /**< \brief pwm handle */
} App_Iom;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_Iom g_Iom;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void IfxIomDemo_init(void);
void IfxIomDemo_run(void);

#endif