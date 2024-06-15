/**
 * \file DmaLinkedListDemo.c
 * \brief Dma Linked List transfers Demo
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
#include "ConfigurationIsr.h"
#include "Ifx_Assert.h"
#include "DmaLinkedListDemo.h"

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
App_DmaLinkedList g_DmaLinkedList;  /**< \brief Dma Linkedlist Global data */

/* The aligment is set 64 bit since circular buffer is configured to 64 bit.   /
 * The User needs to configure the same depeding on their use-case            */

volatile uint32 IFX_ALIGN(64) source[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS];  /**< \brief source buffer */
volatile uint32 IFX_ALIGN(64) destination[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS];  /**< \brief destination buffer */

Ifx_DMA_CH IFX_ALIGN(256) linkedList[NUM_LINKED_LIST_ITEMS];  /**< \brief Linked List storage. IMPORTANT: it has to be aligned to an 256bit address, otherwise DMA can't read it */

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
void IfxDmaLinkedListDemo_init(void)
{
    uint32                   i;

    /* create module config*/
    IfxDma_Dma_Config        dmaConfig;
    IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);

    /* initialize module */
    IfxDma_Dma               dma;
    IfxDma_Dma_initModule(&dma, &dmaConfig);

    /* initial channel configuration */
    IfxDma_Dma_ChannelConfig cfg;
    IfxDma_Dma_initChannelConfig(&cfg, &dma);

    /* following settings are used by all transactions */
    cfg.transferCount           = NUM_TRANSFERED_WORDS;
    cfg.requestMode             = IfxDma_ChannelRequestMode_completeTransactionPerRequest;
    cfg.moveSize                = IfxDma_ChannelMoveSize_32bit;
    cfg.shadowControl           = IfxDma_ChannelShadow_linkedList;

    cfg.channelInterruptEnabled = TRUE;
    cfg.channelInterruptControl = IfxDma_ChannelInterruptControl_thresholdLimitMatch;

    /* generate linked list items */
    for (i = 0; i < NUM_LINKED_LIST_ITEMS; ++i)
    {
        cfg.sourceAddress      =
            IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &source[i]);
        cfg.destinationAddress =
            IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &destination[i]);

        /* address to next transaction set */
        cfg.shadowAddress =
            IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)&linkedList[(i + 1) % NUM_LINKED_LIST_ITEMS]);

        /* transfer first transaction set into DMA channel */
        if (i == 0)
        {
            IfxDma_Dma_initChannel(&g_DmaLinkedList.drivers.chn, &cfg);
        }
        else
        {
            /* transfer into linked list storage */
            IfxDma_Dma_initLinkedListEntry((void *)&linkedList[i], &cfg);  

            /* - activate SCH (transaction request) for each entry, expect for the first one (linked list terminated here) */
            linkedList[i].CHCSR.B.SCH = 1;
        }

    }
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void IfxDmaLinkedListDemo_run(void)
{
    uint32 errors = 0;
    uint32 i, j;

    printf("Starting linked list transfer\n");

    for (i = 0; i < NUM_LINKED_LIST_ITEMS; ++i)
    {
        uint32 *src = (uint32 *)&source[i];

        for (j = 0; j < NUM_TRANSFERED_WORDS; ++j)
        {
            *src++ = 0x10000000 + i * 0x00010000 + j + 1;
        }
    }

    /* clear service request flag */
    (IfxDma_Dma_getSrcPointer(&g_DmaLinkedList.drivers.chn))->B.CLRR = 1;

    /* start linked list transaction */
    IfxDma_Dma_startChannelTransaction(&g_DmaLinkedList.drivers.chn);

    while (!(IfxDma_Dma_getSrcPointer(&g_DmaLinkedList.drivers.chn))->B.SRR)
    {}

    /* check transfered values against expected values */
    printf("Checking destinationBuffer against expected values... \n");

    for (i = 0; i < NUM_LINKED_LIST_ITEMS; ++i)
    {
        uint32 *src = (uint32 *)&source[i];
        uint32 *dst = (uint32 *)&destination[i];

        for (j = 0; j < NUM_TRANSFERED_WORDS; ++j)
        {
            if (*dst++ != *src++)
            {
                ++errors;
            }
        }
    }

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, errors == 0);

    if (errors)
    {
        printf("ERROR: Dma destination data doesn't match with expected data (%lu mismatches)\n", errors);
    }
    else
    {
        printf("OK: Dma destination data matches with expected data\n");
    }
}
