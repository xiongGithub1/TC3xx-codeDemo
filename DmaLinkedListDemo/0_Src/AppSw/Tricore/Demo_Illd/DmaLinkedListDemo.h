/**
 * \file DmaLinkedListDemo.h
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
 *
 * \defgroup IfxLld_Demo_DmaLinkedListDemo_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_DmaLinkedListDemo_SrcDoc
 * \defgroup IfxLld_Demo_DmaLinkedListDemo_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_DmaLinkedListDemo_SrcDoc_Main
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>
#include <Dma/Dma/IfxDma_Dma.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define NUM_LINKED_LIST_ITEMS 4  /**< \brief No od Dma linked lists */
#define NUM_TRANSFERED_WORDS  16 /**< \brief No of data to be transfered */

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    struct
    {
        IfxDma_Dma_Channel chn;     /**< \brief Dma channel handle */
    }drivers;
} App_DmaLinkedList;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_DmaLinkedList g_DmaLinkedList;

IFX_EXTERN volatile uint32 IFX_ALIGN(64) source[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS];  /**< \brief source buffer */
IFX_EXTERN volatile uint32 IFX_ALIGN(64) destination[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS];  /**< \brief destination buffer */

IFX_EXTERN Ifx_DMA_CH IFX_ALIGN(256) linkedList[NUM_LINKED_LIST_ITEMS];  /**< \brief Linked List storage. IMPORTANT: it has to be aligned to an 256bit address, otherwise DMA can't read it */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void IfxDmaLinkedListDemo_init(void);
IFX_EXTERN void IfxDmaLinkedListDemo_run(void);

#endif
