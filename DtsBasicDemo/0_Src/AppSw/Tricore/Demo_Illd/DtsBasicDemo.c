/**
 * \file DtsBasicDemo.c
 * \brief Demo DtsBasicDemo
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
#include <Dts/Dts/IfxDts_Dts.h>
#include "DtsBasicDemo.h"
#include "Stm/Std/IfxStm.h"
#include "_Utilities/Ifx_Assert.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define DTS_WAIT_TIME (200) /* wait time for Dts result in us */
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void DtsBasicDemo_init(void)
{
    /* Get the default configuration */
    IfxDts_Dts_Config dtsConfig;
    IfxDts_Dts_initModuleConfig(&dtsConfig);

    /* adapt the default configuration if required */
    dtsConfig.lowerTemperatureLimit = -35; /* SMU alarm if temperature value is below this Celsius value */
    dtsConfig.upperTemperatureLimit = 150; /* SMU alarm if temperature value is above this Celsius value */

    /* Module initialisation */
    IfxDts_Dts_initModule(&dtsConfig);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void DtsBasicDemo_run(void)
{
    float32 temperature;

	IfxStm_waitTicks(&MODULE_STM0,IfxStm_getTicksFromMicroseconds(&MODULE_STM0,DTS_WAIT_TIME) );

    /* convert result to Celsius */
    temperature = IfxDts_Dts_getTemperatureCelsius();

#ifdef SIMULATION
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, (temperature == 16));
#endif
    printf("DTS Temperature: %3.1f�C\n", temperature);
}
