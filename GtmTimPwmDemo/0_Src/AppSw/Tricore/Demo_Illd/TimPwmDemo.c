/**
 * \file Template.c
 * \brief Demo Template
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

#include "TimPwmDemo.h"
#include "Gtm/Std/IfxGtm.h"
#include "Gtm/Tim/In/IfxGtm_Tim_In.h"
#include "Gtm/Tom/Pwm/IfxGtm_Tom_Pwm.h"
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "ConfigurationIsr.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define TOM_PWM_OUTPUT_FREQUECY_HZ (100000)	/* output frequency is 100KHz */
#define TOM_PWM_OUTPUT_DUTY_PERCENT (50)	/* output duty cycle is 50 % */
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
TimPwmDemo_Info_T TimPwmDemo_Info; /**< \brief Demo information */
IfxGtm_Tim_In_Config TimPwmDemo_Config; /* Config Structure for TIM */
IfxGtm_Tim_In TimPwmDemo_Handler;	/* Handler for TIM config */
IfxGtm_Tom_Pwm_Driver TomPwm_Handler;	/* Handler for TOM PWM */
IfxGtm_Tom_Pwm_Config TomPwm_Config;	/* Config structure for TOM */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void TimMeasurementInit(void); /* Tim configuration for measurement */
static void TomPwmInit(void);	/* TOM configuration for PWM generation */
static void TomPwmStart(void);	/* Start the PWM generation from TOM */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/* Declare the ISR function */
IFX_INTERRUPT(TimDemo_ISR, 0, ISR_PRIORITY_TIMDEMO);

void TimDemo_ISR(void)
{
	IfxCpu_enableInterrupts();
	printf("ISR HIT");
	TimPwmDemo_Info.isrHitCount ++;
	IfxGtm_Tim_In_onIsr(&TimPwmDemo_Handler);
}

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void TimPwmDemo_init(void)
{

	/* enable GTM before anything else */
	 IfxGtm_enable(&MODULE_GTM);

	/* Initialize the TOM for output */
	TomPwmInit();

	/* Initialize the TIM  for measurement */
	TimMeasurementInit();

	/* Start the PWM drive by TOM */
	TomPwmStart();


}

/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void TimPwmDemo_run(void)
{
    /*printf("TimPwmDemo_run() called\n");*/ /* un-comment for debug */

    IfxGtm_Tim_In *handler = &TimPwmDemo_Handler; /* point to the handler of TIM channel */

    /* Read into the information variable */

    if (handler->newData == TRUE) /* If New data is present */
    {
    	handler->newData = FALSE;
    	if (handler->dataCoherent == TRUE) /* If data is coherent */
    	{
    		TimPwmDemo_Info.dataCoherent = TRUE;
    		handler->dataCoherent = FALSE;

    		/* Calculate Duty in percent */
    		TimPwmDemo_Info.duty_percent = (handler->pulseLengthTick * 100) / handler->periodTick;
			printf("duty cycle measured = %d", (int)TimPwmDemo_Info.duty_percent );


    		/* Calculate Frequency in Hz */
    		TimPwmDemo_Info.frequency_hz = handler->captureClockFrequency / handler->periodTick;
			printf("frequency measured in Hz = %d", (int)TimPwmDemo_Info.frequency_hz);
    	}
    	else
    	{
    		TimPwmDemo_Info.dataCoherent = FALSE; /* data is not coherent */
    	}
    }

}

/* TIM measurement initialization */
static void TimMeasurementInit( void )
{
    printf("TIM init called\n");


	/* enable interrupts */
	IfxCpu_enableInterrupts();

	/* Initialize the Config Structure */
    IfxGtm_Tim_In_Config *config = &TimPwmDemo_Config;
    IfxGtm_Tim_In_initConfig(config, &MODULE_GTM);

    /* Configure the Channel used */
    config->gtm                          = &MODULE_GTM;
    config->timIndex                     = IfxGtm_Tim_0;
    config->channelIndex                 = IfxGtm_Tim_Ch_0;


    /* Configure Interrupt parameters */
    config->irqMode                      = IfxGtm_IrqMode_pulseNotify;
    config->isrProvider                  = ISR_PROVIDER_TIMDEMO;
    config->isrPriority                  = ISR_PRIORITY_TIMDEMO;

    /* Enable the CMU clock for TIM */
    MODULE_GTM.CMU.CLK_EN.B.EN_CLK0 = 0x2;     /* enable the CMU clock 0 now */

    /* Configure the Capture Configurations */
    config->capture.irqOnNewVal          = TRUE; /* give me an interrupt on NEWVAL */
    config->capture.irqOnCntOverflow     = FALSE;
    config->capture.irqOnEcntOverflow    = FALSE;
    config->capture.irqOnDatalost        = FALSE;
    config->capture.clock                = IfxGtm_Cmu_Clk_0;
    config->capture.mode                 = Ifx_Pwm_Mode_leftAligned;

    /* Configure Timeout */
    config->timeout.irqOnTimeout         = FALSE;
    config->timeout.clock                = IfxGtm_Cmu_Clk_0;
    config->timeout.timeout              = 0.0;

    /* Configure the Input */
    config->filter.input                 = IfxGtm_Tim_In_Input_currentChannel;
    config->filter.inputPin              = &IfxGtm_TIM0_0_P02_0_IN; /* Use Port 02 channel 0 */
    config->filter.inputPinMode          = IfxPort_InputMode_noPullDevice;

    /* Configure Filter parameters */
    config->filter.risingEdgeMode        = IfxGtm_Tim_In_ConfigFilterMode_none;
    config->filter.fallingEdgeMode       = IfxGtm_Tim_In_ConfigFilterMode_none;
    config->filter.risingEdgeFilterTime  = 0;
    config->filter.fallingEdgeFilterTime = 0;
    config->filter.clock                 = IfxGtm_Cmu_Tim_Filter_Clk_0;

    /* Initialize the TIM Channel Now */
    IfxGtm_Tim_In_init(&TimPwmDemo_Handler, config);

    /* Read the TIM configuration information here */
    TimPwmDemo_Info.TimModule = (uint8)config->timIndex;
    TimPwmDemo_Info.TimChannel = (uint8)config->channelIndex;
    TimPwmDemo_Info.CaptureFrequency = TimPwmDemo_Handler.captureClockFrequency;

    printf("TIM capture frequency = %d", (int)TimPwmDemo_Info.CaptureFrequency);
}

/* Initialize the TOM PWM module for output */
static void TomPwmInit (void)
{
	IfxGtm_Tom_Pwm_Config *config = &TomPwm_Config;

	IfxGtm_Tom_Pwm_initConfig(config, &MODULE_GTM); /* initialize the config */

	/* Configure the TOM channel */
    config->tom                      = IfxGtm_Tom_0;
    config->tomChannel               = IfxGtm_Tom_Ch_0;

    /* Configure the clock */
    config->clock                    = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk0; /* Use FX clock 0 (divide by 1) */

    /* Configure the period and dutycycle counts */

    MODULE_GTM.CMU.CLK_EN.B.EN_FXCLK = 0x2;     /* enable the FX clock now */
    TimPwmDemo_Info.tomChannelFreq = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM, (IfxGtm_Cmu_Fxclk)config->clock, FALSE);

    printf("TOM channel frequency = %d",(int)TimPwmDemo_Info.tomChannelFreq);

    config->period                   = (uint32)(TimPwmDemo_Info.tomChannelFreq / TOM_PWM_OUTPUT_FREQUECY_HZ);
    config->dutyCycle                = (uint32)( (TOM_PWM_OUTPUT_DUTY_PERCENT * config->period)/ 100 );

    /* output parameters */
    config->signalLevel              = Ifx_ActiveState_high;
    config->oneShotModeEnabled       = FALSE;
    config->synchronousUpdateEnabled = TRUE;
    config->immediateStartEnabled    = FALSE;

    /* Interrupt parameters */
    config->interrupt.ccu0Enabled    = FALSE;
    config->interrupt.ccu1Enabled    = FALSE;
    config->interrupt.mode           = IfxGtm_IrqMode_pulseNotify;
    config->interrupt.isrProvider    = ISR_PROVIDER_TIMDEMO;
    config->interrupt.isrPriority    = 0;

    /* Output pin parameters */
    config->pin.outputPin            = &IfxGtm_TOM0_0_TOUT109_P10_7_OUT;
    config->pin.outputMode           = IfxPort_OutputMode_pushPull;
    config->pin.padDriver            = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    /* Initialize the TOM channel now */
    IfxGtm_Tom_Pwm_init(&TomPwm_Handler, config);

    /* Read into the info variable now */
    TimPwmDemo_Info.PwmFrequency = TOM_PWM_OUTPUT_FREQUECY_HZ;
    TimPwmDemo_Info.pwmduty_percent = TOM_PWM_OUTPUT_DUTY_PERCENT;
    TimPwmDemo_Info.TomModule = config->tom;
    TimPwmDemo_Info.TomChannel = config->tomChannel;
    TimPwmDemo_Info.pwmduty_counts = config->dutyCycle;
    TimPwmDemo_Info.pwmperiod_counts = config->period;

    printf("TOM PWM duty counts = %d",(int)TimPwmDemo_Info.pwmduty_counts );
    printf("TOM PWM period counts = %d",(int)TimPwmDemo_Info.pwmperiod_counts );

}

static void TomPwmStart (void)
{
	IfxGtm_Tom_Pwm_start(&TomPwm_Handler, TRUE);
}
