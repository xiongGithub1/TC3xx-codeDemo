/**
 * \file CpuSyncDemo.c
 * \brief Cpu Synchronization Demo. It demonstrates how to use Mutex for synchronisation.
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
#include <Cpu/Std/IfxCpu.h>
#include "Cpu0_Main.h"
#include "Ifx_Assert.h"
#include "CpuSyncDemo.h"

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
App_CpuSync g_CpuSync; /**< \brief Cpu Sync global data */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Sample critical Section function
 *
 * This function is called from the run API
 */
static void SampleCriticalSection(void)
{
    volatile uint32 i;

    for (i = 0; i < 2000; i++)
    {
        __nop();
    }
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void IfxCpuSyncDemo_init(void)
{
    printf("IfxCpuSyncDemo_init() called\n");
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void IfxCpuSyncDemo_run(void)
{
    uint32  errors = 0;
    boolean flag1, flag2;

    flag1 = IfxCpu_acquireMutex(&g_CpuSync.resourceLock);

    if (flag1)
    {
        /*Note: Mutex acquired successfully - I */
        /*Note: Trying to acquire the same mutex...  Now it fails as mutex is already acquired. */

        flag2 = IfxCpu_acquireMutex(&g_CpuSync.resourceLock);

        if (!flag2)
        {
            /* Mutex acquire FAILED - II. It is expected as mutex is already acquired. */
        }
        else
        {
            errors++;
        }

        /* critical section */
        SampleCriticalSection();

        /* Mutex released - I. */
        IfxCpu_releaseMutex(&g_CpuSync.resourceLock);
    }
    else
    {
        errors++;
    }

    /* Note: Trying to acquire the same mutex after release. */

    flag2 = IfxCpu_acquireMutex(&g_CpuSync.resourceLock);

    if (flag2)
    {
        /* Note: Mutex acquired successfully - II */
        /* critical section*/
        SampleCriticalSection();
        /* Mutex released - II. */
        IfxCpu_releaseMutex(&g_CpuSync.resourceLock);
    }
    else
    {
        errors++;
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, errors == 0);

    if (errors)
    {
        printf("ERROR: Mutex behaviour is NOT as expected. (%ld mismatches)!\n", errors);
    }
    else
    {
        printf("OK: Mutex behaviour is as expected\n");
    }
}