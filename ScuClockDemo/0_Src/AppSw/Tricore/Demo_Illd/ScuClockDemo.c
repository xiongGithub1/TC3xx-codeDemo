/**
 * \file ScuClockDemo.c
 * \brief Scu PLL and Clock Demo
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
#include "Cpu0_Main.h"
#include <Scu/Std/IfxScuCcu.h>
#include "ScuClockDemo.h"

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
App_ScuClock g_ScuClock; /**< \brief Scu Clock global data */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/** \brief Default configuration for the PLL initial steps
 */
IfxScuCcu_Config      IfxScuCcu_testClockConfig;

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void IfxScuClockDemo_init(void)
{
    /* ensure that XTAL1 configured for X MHz */
    uint32 fosc = IFX_CFG_SCU_XTAL_FREQUENCY;
    uint32 fpll = IFX_CFG_SCU_PLL_FREQUENCY;

    /* NOTE: Don't add any printf before the clock initialization. Asc won't be initialized properly. */
    IfxScuCcu_initConfig(&IfxScuCcu_testClockConfig);

    /* standard PLL initialisation */
    IfxScuCcu_init(&IfxScuCcu_testClockConfig);

    g_ScuClock.pllfreq = IfxScuCcu_getPllFrequency();
    g_ScuClock.cpufreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_ScuClock.spbfreq = IfxScuCcu_getSpbFrequency();
    g_ScuClock.srifreq = IfxScuCcu_getSriFrequency();
    g_ScuClock.stmfreq = IfxScuCcu_getStmFrequency();

    printf("PLL initialized: Normal mode, fosc: %ld MHz & fpll: %ld MHz\n", fosc / 1000000, fpll / 1000000);
    printf("PLL freq: %ld MHz\n", g_ScuClock.pllfreq / 1000000);
    printf("CPU freq: %ld MHz\n", g_ScuClock.cpufreq / 1000000);
    printf("SPB freq: %ld MHz\n", g_ScuClock.spbfreq / 1000000);
    printf("SRI freq: %ld MHz\n", g_ScuClock.srifreq / 1000000);
    printf("STM freq: %ld MHz\n", g_ScuClock.stmfreq / 1000000);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void IfxScuClockDemo_run(void) {}
