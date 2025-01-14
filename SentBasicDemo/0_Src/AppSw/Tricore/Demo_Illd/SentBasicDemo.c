/**
 * \file SentBasicDemo.c
 * \brief Sent Demo
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include "Cpu0_Main.h"
#include <Cpu/Std/IfxCpu.h>
#include "SentBasicDemo.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define WATCH_DOG_ENABLED FALSE /*By default Watch Dog is disabled, change it
								 *to TRUE if Watch Dog is needed*/
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
App_Sent g_Sent; /**< \brief global data */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static uint32 reverseHexData(uint32 input);
static void   SentInterruptHandler(IfxSent_Sent_Channel *channel);
static void   SentWDIHandler(App_Sent *app);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \addtogroup IfxLld_Demo_QspiCpu_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for sent_ch0
 * \{ */
IFX_INTERRUPT(ISR_sent_ch0, 0, ISR_PRIORITY_SENT_CH0);
/** \} */

/** \} */

/** \brief Handle sent_ch0 interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_sent_ch0
 * \isrPriority \ref ISR_PRIORITY_sent_ch0
 *
 */
void ISR_sent_ch0(void)
{
    IfxCpu_enableInterrupts();
    SentInterruptHandler(&g_Sent.sentChannel);
}

void SentWDIHandler(App_Sent *app)
{
	app->sentWatchDogLapseCount++;
}

static void SentInterruptHandler(IfxSent_Sent_Channel *channel)
{
    Ifx_SENT_CH_INTSTAT interruptStatus = IfxSent_Sent_getAndClearInterruptStatus(channel);

    if (interruptStatus.U)
    {
        /* check for error conditions */
        if (interruptStatus.U & IFXSENT_INTERRUPT_STATUS_ERROR_FLAGS)
        {
            /* Receive Buffer Overflow
             * This bit is set after a frame has been received while the old one was
             * not read from RDRx. I.e. the kernel wants to set any of the two
             * interrupts RSI and RDI and finds any of these two interrupts already
             * set. The old data is overwritten by the new data.
             */
            if (interruptStatus.B.RBI)
            {
                g_Sent.errors++;
            }

            /* Transmit Buffer Underflow
             * This bit is set after data has been completely transferred (PLEN
             * exceeded) and no new data was written to SCRx.
             */
            if (interruptStatus.B.TBI)
            {
                g_Sent.errors++;
            }

            /* Frequency Range Error
             * This bit is set after a Synchronization / Calibration pulse was
             * received that deviates more than +- 25% from the nominal value.
             * The referring data is ignored.
             */
            if (interruptStatus.B.FRI)
            {
                g_Sent.errors++;
            }

            /* Frequency Drift Error
             * This bit is set after a subsequent Synchronization / Calibration
             * pulse was received that deviates more than 1.5625% (1/64) from its
             * predecessor.
             */
            if (interruptStatus.B.FDI)
            {
                g_Sent.errors++;
            }

            /* Wrong Number of Nibbles
             * This bit is set after a more nibbles have been received than expected
             * or a Synchronization / Calibration Pulse is received too early thus
             * too few nibbles have been received
             */
            if (interruptStatus.B.NNI)
            {
                g_Sent.errors++;
            }

            /* Nibbles Value out of Range
             * This bit is set after a too long or too short nibble pulse has been
             * received. I.e. value < 0 or value > 15.
             */
            if (interruptStatus.B.NVI)
            {
                g_Sent.errors++;
            }

            /* CRC Error
             * This bit is set if the CRC check fails.
             */
            if (interruptStatus.B.CRCI)
            {
                g_Sent.errors++;
            }

            /* Wrong Status and Communication Nibble Error
             * In standard Serial Frame Mode (RCR.ESF is cleared), this bit is set
             * if the Status and Communication nibble shows a start bit in a frame
             * other than frame number n x 16.
             * In Extended Serial Frame Mode this bit is without function.
             */
            if (interruptStatus.B.WSI)
            {
                g_Sent.errors++;
            }

            /* Serial Data CRC Error
             * This bit is set if the CRC of the serial message fails.
             * In Extended Serial Message Format, this includes a check of the Serial
             * Communication Nibble for correct 0 values of bit 3 in frames 7, 13 and 18.
             */
            if (interruptStatus.B.SCRI)
            {
                g_Sent.errors++;
            }

            /* Watch Dog Error
             * This bit is set if the Watch Dog Timer of the channel expires.
             */
            if (interruptStatus.B.WDI && WATCH_DOG_ENABLED)
            {
               g_Sent.errors++;
               SentWDIHandler(&g_Sent);
            }
        }

        /* transaction events */

        /* Receive Data
         * RDI is activated when a received frame is moved to a Receive Data
         * Register RDR. Both RDI and RSI will be issued together in normal use
         * cases where the frame size is not bigger than 8 nibbles and CRC is
         * correct or not checked (if RCRx.CDIS is cleared).
         */
        if (interruptStatus.B.RDI)
        {
            /* Ignore RDI bit, useful only when Frame Length is greater than
             * 8 nibbles since it can indicate that end of frame
             */
        }

        /* Receive Success
         * This bit is set at the successfully received end of a frame.
         * Depending on bit RCRx.CDIS this indicates a successful check of the CRC.
         */
        if (interruptStatus.B.RSI)
        {
            /* decode incoming frame */
            IfxSent_Sent_Frame frame;
            uint32             temp;
            IfxSent_Sent_readChannelSerialDataFrame(channel, &frame);
            g_Sent.sentRawData        = frame.data;
            g_Sent.sentStatus         = frame.statusNibble;
            temp                      = reverseHexData(frame.data);
            g_Sent.sentMSensorData    = (temp & 0x00FFFF00) >> 8;
            g_Sent.sentTempSensorData = (temp & 0x000000FF) - 55;

            /* increment counter */
            ++g_Sent.interruptCounter;
        }

        /* Transfer Data
         * This bit is set after the trigger condition was detected. Data to be
         * transferred has been moved internally. Thus a new value can be written
         * to SCRx. This can be used for back to back transfers.
         */
        if (interruptStatus.B.TDI)
        {}

        /* Serial Data Received
         * This bit is set after all serial data bits have been received via the
         * Status and Communication nibble. Depending on bit RCRx.SCDIS this
         * indicates a successful check of the CRC.
         */
        if (interruptStatus.B.SDI)
        {
            /* decode incoming message */
            IfxSent_Sent_SerialMessageFrame message;
            IfxSent_Sent_readChannelSerialMessageFrame(channel, &message);
        }
    }
}


static uint32 reverseHexData(uint32 input)
{
    uint32 result = 0;

    {
        result |= (input & 0x00000FU) << 20;
        result |= (input & 0x0000F0U) << 12;
        result |= (input & 0x000F00U) << 4;

        result |= (input & 0x00F000U) >> 4;
        result |= (input & 0x0F0000U) >> 12;
        result |= (input & 0xF00000U) >> 20;
    }

    return result;
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void SentBasicDemo_init(void)
{
    IfxSent_Sent_Config        sentConfig;
    IfxSent_Sent_ChannelConfig sentChannelConfig;

    const IfxSent_Sent_Pins    sentPins = {
        /* Channel 0 */
        &IfxSent_SENT0C_P02_8_IN, IfxPort_InputMode_pullUp,     /* SENT input */
        &IfxSent_SPC0_P00_1_OUT,  IfxPort_OutputMode_openDrain, /* SENT output */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };

    /* disable interrupts */
    boolean                    interruptState = IfxCpu_disableInterrupts();

    /* clear interrupt counters and global result of this testcase */
    g_Sent.interruptCounter = 0;
    g_Sent.errors           = 0;
    g_Sent.sentWatchDogLapseCount = 0;
    /* create module config */
    IfxSent_Sent_initModuleConfig(&sentConfig, &MODULE_SENT);
    /* initialize module */
    IfxSent_Sent_initModule(&g_Sent.sent, &sentConfig);

    /* create channel config */
    IfxSent_Sent_initChannelConfig(&sentChannelConfig, &g_Sent.sent);
#if !defined(SIMULATION)
    sentChannelConfig.receiveControl.alternateCrcSelected = TRUE;
    sentChannelConfig.receiveControl.statusNibbleEnabled  = TRUE;
    sentChannelConfig.receiveControl.suspendTriggered     = TRUE;
    sentChannelConfig.receiveControl.crcMethodDisabled    = TRUE;
#endif

    
    if (WATCH_DOG_ENABLED == TRUE)
    {
    	sentChannelConfig.watchDogTimerLimit = 1000;/*Watchdog Timer Value*/
    	sentChannelConfig.enabledInterrupts.ALL = sentChannelConfig.enabledInterrupts.ALL | 0x2000;/*Enable Watch Dog Interrupt*/
    }
    /* define tUnit of the external sensor */
    sentChannelConfig.tUnit = 3.0E-6;

    /* ISR priorities and interrupt target */
    sentChannelConfig.interrupt.priority    = ISR_PRIORITY_SENT_CH0;
    sentChannelConfig.interrupt.isrProvider = ISR_PROVIDER_SENT_CH0;

    /* pin configuration */
    {
        sentChannelConfig.pins = &sentPins;

        /* set channel */
        sentChannelConfig.channelId = (IfxSent_ChannelId)0;

        /* initialize channel */
        IfxSent_Sent_initChannel(&g_Sent.sentChannel, &sentChannelConfig);
    }
    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void SentBasicDemo_run(void)
{
    /* wait until 3 interrupts have been executed for all channels */
    printf("SentBasicDemo_run() called\n");

    g_Sent.interruptCounter = 0;

    while (g_Sent.interruptCounter < 3)
    {}
}
