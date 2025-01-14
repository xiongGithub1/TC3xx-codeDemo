/**
 * \file Ccu6TpwmDemo.c
 * \brief Demo Ccu6TpwmDemo
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
#include "Ccu6TpwmDemo.h"

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

App_Ccu6Tpwm g_Ccu6Tpwm; /**< \brief Demo information */

/* pin configuration */
IfxCcu6_TPwm_Pins pins = {
    NULL_PTR,                              /* CC60Out pin not used */
    NULL_PTR,                              /* CC61Out pin not used */
    NULL_PTR,                              /* CC62Out pin not used */
    NULL_PTR,                              /* COUT60 pin not used */
    NULL_PTR,                              /* COUT61 pin not used */
    NULL_PTR,                              /* COUT62 pin not used */
    &IfxCcu60_COUT63_P00_0_OUT,            /* COUT63 pin */
    IfxPort_OutputMode_pushPull,
    IfxPort_PadDriver_cmosAutomotiveSpeed1,
    NULL_PTR,
    NULL_PTR,
    IfxPort_InputMode_pullUp
};

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_Ccu6Tpwm_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Transmit
 * \{ */

IFX_INTERRUPT(ccu6_ISR_Tpwm, 0, ISR_PRIORITY_CCU6_TPWM);

/** \} */

/** \} */

/** \brief Handle <isrName> interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_TPWM
 * \isrPriority \ref ISR_PRIORITY_TPWM
 *
 */
void ccu6_ISR_Tpwm(void)
{
    IfxCpu_enableInterrupts();

    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13CompareMatch);
    g_Ccu6Tpwm.count++;
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void Ccu6TpwmDemo_init(void)
{
    /* disable interrupts */
    boolean             interruptState = IfxCpu_disableInterrupts();

    /* create configuration */
    IfxCcu6_TPwm_Config tPwmConfig;
    IfxCcu6_TPwm_initModuleConfig(&tPwmConfig, &MODULE_CCU60);

    /* configure the frequency of the timer in case of internal start */
    /* this frequency will be set for the timer block selected later */
    tPwmConfig.base.frequency = 400000;

    /* configure the period of the timer */
    tPwmConfig.base.period = 100;

    /* configure the waiting time in case of delayed T13 start in sync with T12 */
    tPwmConfig.base.waitingTime = 20;

    /* select the active state of the output */
    tPwmConfig.base.activeState = Ifx_ActiveState_high;

    /* select the timer through which PWM is to be generated */
    tPwmConfig.timer = IfxCcu6_TimerId_t13;

    /* configure the clock for internal mode */
    tPwmConfig.clock.t13ExtClockEnabled = FALSE;

    /* configure the selcted timer block */
    tPwmConfig.timer13.counterValue = 0;
    tPwmConfig.timer13.compareValue = 100;

    /*select the channel out for modulation */
    tPwmConfig.channelOut = IfxCcu6_ChannelOut_cout3;

    tPwmConfig.pins = &pins;

    /* configure the interrupts */
    tPwmConfig.interrupt1.source         = IfxCcu6_InterruptSource_t13CompareMatch;
    tPwmConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_2;
    tPwmConfig.interrupt1.priority       = ISR_PRIORITY_CCU6_TPWM;
    tPwmConfig.interrupt1.typeOfService  = ISR_PROVIDER_CCU6_TPWM;

    /* configure input and output triggers */
    tPwmConfig.trigger.t12ExtInputTrigger   = NULL_PTR;
    tPwmConfig.trigger.t13ExtInputTrigger   = NULL_PTR;
    tPwmConfig.trigger.t13InSyncWithT12     = FALSE;

    tPwmConfig.trigger.outputTriggerEnabled = FALSE;

    /* initialize the module */
    IfxCcu6_TPwm_initModule(&g_Ccu6Tpwm.drivers.tPwm, &tPwmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    printf("Ccu6 Tpwm is initialised\n");

    IfxCcu6_TPwm_start(&g_Ccu6Tpwm.drivers.tPwm);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void Ccu6TpwmDemo_run(void)
{}
