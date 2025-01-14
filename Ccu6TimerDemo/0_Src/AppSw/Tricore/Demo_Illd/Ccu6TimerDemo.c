/**
 * \file Ccu6TimerDemo.c
 * \brief Demo Ccu6TimerDemo
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
#include "Ccu6TimerDemo.h"

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

App_Ccu6Timer g_Ccu6Timer; /**< \brief Demo information */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_Ccu6Timer_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Timer
 * \{ */

IFX_INTERRUPT(ccu6_ISR_Timer, 0, ISR_PRIORITY_CCU6_TIMER);

/** \} */

/** \} */

/** \brief Handle Timer interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_TIMER
 * \isrPriority \ref ISR_PRIORITY_TIMER
 *
 */
void ccu6_ISR_Timer(void)
{
    IfxCpu_enableInterrupts();

    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);
    g_Ccu6Timer.count++;
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void Ccu6TimerDemo_init(void)
{
    /* disable interrupts */
    boolean              interruptState = IfxCpu_disableInterrupts();

    /* create configuration */
    IfxCcu6_Timer_Config timerConfig;
    IfxCcu6_Timer_initModuleConfig(&timerConfig, &MODULE_CCU60);

    /* configure the frequency of the timer in case of internal start */
    /* this frequency will be set for the timer block selected later */
    timerConfig.base.t13Frequency = 400000;

    /* configure the period of the timer */
    timerConfig.base.t13Period = 100;

    /* select the timer that needs to be started */
    timerConfig.timer = IfxCcu6_TimerId_t13;

    /* select the synchronous operation if both timers need to be start at the same time */
    /* previous selection of timer block can be ignored in this mode */
    timerConfig.synchronousOperation = FALSE;

    /* configure the clock for internal mode */
    timerConfig.clock.t13ExtClockEnabled   = FALSE;
    timerConfig.clock.t13ExtClockInput     = NULL_PTR;
    timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;

    /* configure the selected timer block */
    timerConfig.timer13.counterValue = 0;

    /* configure the interrupts */
    timerConfig.interrupt1.source         = IfxCcu6_InterruptSource_t13PeriodMatch;
    timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_2;
    timerConfig.interrupt1.priority       = ISR_PRIORITY_CCU6_TIMER;
    timerConfig.interrupt1.typeOfService  = ISR_PROVIDER_CCU6_TIMER;

    /* configure triggers */
    timerConfig.trigger.t13InSyncWithT12 = FALSE;

    /* initialize the module */
    IfxCcu6_Timer_initModule(&g_Ccu6Timer.drivers.timer, &timerConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    printf("Ccu6 Timer is initialised\n");

    IfxCcu6_Timer_start(&g_Ccu6Timer.drivers.timer);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void Ccu6TimerDemo_run(void)
{}
