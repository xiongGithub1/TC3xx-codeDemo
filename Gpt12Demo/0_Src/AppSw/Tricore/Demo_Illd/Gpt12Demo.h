/**
 * \file Gpt12.h
 * \brief Demo Gpt12
 *
 * \version iLLD_Demos_1_0_1_15_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_Gpt12_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_Gpt12_SrcDoc
 * \defgroup IfxLld_Demo_Gpt12_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_Gpt12_SrcDoc_Main
 */

#ifndef GPT12_H
#define GPT12_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>
#include "ConfigurationIsr.h"
#include "Stm/Std/IfxStm.h"
#include "Gpt12/IncrEnc/IfxGpt12_IncrEnc.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    uint32           dummy;     /* Dummy variable */
    IfxGpt12_IncrEnc incrEnc;
    struct
    {
        IfxPort_Pin A;            /**< \brief A */
        IfxPort_Pin B;            /**< \brief B */
        IfxPort_Pin Z;            /**< \brief Z */
    } interface;
    struct
    {
        boolean          run;                  /* Status: TRUE: the encoder turn */
        IfxStdIf_Pos_Dir direction;            /* Direction*/
        uint32           step;                 /* Current step */
        sint32           rawPosition;          /* Expected rawPosition */
        sint32           positionMask;         /* Mask for rawPosition value */
        uint32           resolution;           /* Encoder resolution */
        uint32           multiplicationFactor; /* Encoder multiplication factor */
        float32          updatePeriod;         /* Refresh period */
        float32          speed;                /* Requested encoder speed in rad/s */
    } control;
    struct
    {
        float32          speed;
        float32          rawPosition;
        IfxStdIf_Pos_Dir direction;
    }status;
} App_Gpt12;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_Gpt12 g_Gpt12;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void Gpt12Demo_init(void);
IFX_EXTERN void Gpt12Demo_run(void);

#endif