/**
 * \file EdsadcBasicDemo.c
 * \brief Demo EdsadcBasicDemo
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
#include "EdsadcBasicDemo.h"
#include "Stm/Std/IfxStm.h"

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
App_EdsadcBasic g_EdsadcBasic; /**< \brief Demo information */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
static uint8 edsadcChannelAvailable[4] = {1, 0, 0, 1};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void EdsadcBasicDemo_init(void)
{
    uint32 chn;

    /* disable pull-up devices for analog pads of P00 (assigned to some analog pins) */
    #if defined(P00_IOCR0)
    P00_IOCR0.U = 0x00000000;
    #endif
    #if defined(P00_IOCR4)
    P00_IOCR4.U = 0x00000000;
    #endif
    #if defined(P00_IOCR8)
    P00_IOCR8.U = 0x00000000;
    #endif
    #if defined(P00_IOCR12)
    P00_IOCR12.U = 0x00000000;
    #endif
    #if defined(P01_IOCR0)
    P01_IOCR0.U = 0x00000000;
    #endif
    #if defined(P01_IOCR4)
    P01_IOCR4.U = 0x00000000;
    #endif
	#if defined(P01_IOCR8)
    P01_IOCR8.U = 0x00000000;
	#endif
	#if defined(P01_IOCR12)
    P01_IOCR12.U = 0x00000000;
	#endif
    #if defined(P02_IOCR0)
    P02_IOCR0.U = 0x00000000;
    #endif
    #if defined(P02_IOCR4)
    P02_IOCR4.U = 0x00000000;
    #endif
    #if defined(P02_IOCR8)
    P02_IOCR8.U = 0x00000000;
    #endif
	#if defined(P02_IOCR12)
    P02_IOCR12.U = 0x00000000;
	#endif
    #if defined(P33_IOCR0)
    P33_IOCR0.U = 0x00000000;
    #endif
    #if defined(P33_IOCR4)
    P33_IOCR4.U = 0x00000000;
    #endif
    #if defined(P33_IOCR8)
    P33_IOCR8.U = 0x00000000;
    #endif
    #if defined(P33_IOCR12)
    P33_IOCR12.U = 0x00000000;
    #endif

	printf("EdsadcBasicDemo_init() called\n");

    /* create module config */
    IfxEdsadc_Edsadc_Config        edsadcConfig;
    IfxEdsadc_Edsadc_initModuleConfig(&edsadcConfig, &MODULE_EDSADC);
	edsadcConfig.modulatorClockMode = IfxEdsadc_ModulatorClockGeneration_unsyncMode;

    /* initialize module */
    IfxEdsadc_Edsadc               edsadc;
    IfxEdsadc_Edsadc_initModule(&edsadc, &edsadcConfig);

    /* create channel config */
    IfxEdsadc_Edsadc_ChannelConfig edsadcChannelConfig;
    IfxEdsadc_Edsadc_initChannelConfig(&edsadcChannelConfig, &edsadc);

    /* modify default configuration */
    edsadcChannelConfig.modulator.positiveInput      = IfxEdsadc_InputConfig_inputPin;
    edsadcChannelConfig.modulator.negativeInput      = IfxEdsadc_InputConfig_inputPin;
    edsadcChannelConfig.modulator.inputGain          = IfxEdsadc_InputGain_factor1;
    edsadcChannelConfig.modulator.inputPin           = IfxEdsadc_InputPin_a;
    edsadcChannelConfig.modulator.modulatorClockFreq = 40.0e6;
	
    edsadcChannelConfig.combFilter.decimationFactor  = 32;
    edsadcChannelConfig.combFilter.startValue        = 32;

    edsadcChannelConfig.firFilter.fir0Enabled        = TRUE;
    edsadcChannelConfig.firFilter.fir1Enabled        = TRUE;
    edsadcChannelConfig.firFilter.offsetCompensation = IfxEdsadc_OffsetCompensationFilter_disabled;
    edsadcChannelConfig.firFilter.fir1DecimateRate   = TRUE;
    edsadcChannelConfig.firFilter.prefilterEnabled   = TRUE;

    /* initialize channels */
    for (chn = 0; chn < 1; ++chn)
    {
        if (edsadcChannelAvailable[chn])
        {
            edsadcChannelConfig.channelId = (IfxEdsadc_ChannelId)chn;

            IfxEdsadc_Edsadc_initChannel(&g_EdsadcBasic.edsadcChannel[chn], &edsadcChannelConfig);
        }
    }

    /* start conversions */
    IfxEdsadc_Edsadc_startScan(&edsadc, 0x3fff, 0);
    IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0,13) );
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void EdsadcBasicDemo_run(void)
{
    uint32 i, j;
    uint32 chn;
    sint32 result;

    printf("EdsadcBasicDemo_run() called\n");

	printf("EDSADC Channel Results:\n");

    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 100000; j++)
        {}

        for (chn = 0; chn < 1; ++chn)
        {
            if (edsadcChannelAvailable[chn])
            {
			result = IfxEdsadc_Edsadc_getMainResult(&g_EdsadcBasic.edsadcChannel[chn]);
			printf("%d - Channel #%d: %d\n", i, chn, result);
            }
        }
    }
}