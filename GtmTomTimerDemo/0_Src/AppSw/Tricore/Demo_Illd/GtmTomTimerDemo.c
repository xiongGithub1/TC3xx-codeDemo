/**
 * \file GtmAtomPwmHlDemo.c
 * \brief Demo GtmAtomPwmHlDemo
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include "GtmTomTimerDemo.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

App_GtmTomTimer g_GtmTomTimer; /**< \brief Demo information */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_GtmTomTimer_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Timer_1ms
 * \{ */

IFX_INTERRUPT(ISR_Timer_1ms, 0, ISR_PRIORITY_TIMER_1MS);

/** \} */

/** \} */

/** \brief Handle 1ms interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_TIMER_1MS
 * \isrPriority \ref ISR_PRIORITY_TIMER_1MS
 *
 */
void ISR_Timer_1ms(void)
{
    IfxCpu_enableInterrupts();

    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_GtmTomTimer.drivers.timerOneMs);
    g_GtmTomTimer.isrCounter.slotOneMs++;
}


void GtmTomTimer_initTimer(void)
{
    {   /* GTM TOM configuration */
        IfxGtm_Tom_Timer_Config timerConfig;
        IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);
        timerConfig.base.frequency       = 1000;
        timerConfig.base.isrPriority     = ISR_PRIORITY(INTERRUPT_TIMER_1MS);
        timerConfig.base.isrProvider     = ISR_PROVIDER(INTERRUPT_TIMER_1MS);
        timerConfig.base.minResolution   = (1.0 / timerConfig.base.frequency) / 1000;
        timerConfig.base.trigger.enabled = FALSE;
        timerConfig.tom                  = IfxGtm_Tom_1;
        timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;
        timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1;
        IfxGtm_Tom_Timer_init(&g_GtmTomTimer.drivers.timerOneMs, &timerConfig);

        IfxGtm_Tom_Timer_run(&g_GtmTomTimer.drivers.timerOneMs);
    }
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void GtmTomTimerDemo_init(void)
{
    /* disable interrupts */
    boolean  interruptState = IfxCpu_disableInterrupts();

    /** - GTM clocks */
    Ifx_GTM *gtm = &MODULE_GTM;
    IfxGtm_enable(gtm);
    g_GtmTomTimer.info.gtmFreq = IfxGtm_Cmu_getModuleFrequency(gtm);

    /* Set the global clock frequencies */
    IfxGtm_Cmu_setGclkFrequency(gtm, g_GtmTomTimer.info.gtmFreq);
    g_GtmTomTimer.info.gtmGclkFreq = IfxGtm_Cmu_getGclkFrequency(gtm);

    /** - Initialise the GTM part */
    GtmTomTimer_initTimer();

    printf("Gtm Tom Timer is initialised\n");

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    IfxGtm_Cmu_enableClocks(gtm, IFXGTM_CMU_CLKEN_FXCLK | IFXGTM_CMU_CLKEN_CLK0);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void GtmTomTimerDemo_run(void)
{}
