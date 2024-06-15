/**
 * \file CanSleepWakeupDemo.h
 * \brief Demo CanSleepWakeupDemo
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
 * \defgroup IfxLld_Demo_CanSleepWakeup_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_CanSleepWakeup_SrcDoc
 * \defgroup IfxLld_Demo_CanSleepWakeup_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_CanSleepWakeup_SrcDoc_Main
 */

#ifndef CANSLEEPWAKEUPDEMO_H
#define CANSLEEPWAKEUPDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <Ifx_Types.h>
#include "Configuration.h"
#include "IfxPort.h"
#include <Can/Can/IfxCan_Can.h>


#define TESTED_NODES 2

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFX_INTPRIO_CAN1_TX 	0x3D
#define IFX_INTPRIO_CAN0_RX 	0x30
#define IFX_ISR_PRIORITY_STM 	10
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Can information */
typedef struct
{
    struct
    {
        IfxCan_Can can[2];                     /* CAN handle */
		IfxCan_Can_Node canNode[TESTED_NODES];    /* CAN Node handles */
    }         drivers;

    Ifx_SizeT count;
} App_CanSleepWakeup;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

IFX_EXTERN App_CanSleepWakeup g_CanSleepWakeup;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_EXTERN void CanSleepWakeupDemo_init(void);
IFX_EXTERN void CanSleepWakeupDemo_run(void);

#endif
