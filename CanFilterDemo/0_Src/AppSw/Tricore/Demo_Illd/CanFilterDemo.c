/**
 * \file CanFilterDemo.c
 * \brief Demo CanFilterDemo
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

#include "CanFilterDemo.h"

#include <stdio.h>
#include <Cpu/Irq/IfxCpu_Irq.h>
#include "IfxCpu.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
typedef enum
{
	IfxCan_dedicatedBuffers,  /**< \brief Rx dedicated buffers operation */
	IfxCan_fifo0,             /**< \brief Rx FIFO 0 operation */
	IfxCan_fifo1,             /**< \brief Rx FIFO 1 operation */
} IfxCan_MsgMode;
/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

App_CanFilter g_CanFilter;
/* Interrupt processing status */
volatile uint8 InterruptProcessed = 0;
/* Errors in CAN reception */
static uint8 ErrorInCan = 0;
/* Number of messages transmitted */
uint8 messageNum = 0;
/* Data to be transmitted */
uint32 txData[2];
/* Rejected message count */
uint8 rejectNumMsgs = 0;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void dataIntegrityCheck(uint32 canNode, IfxCan_MsgMode rxMsgType);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
volatile uint8 numTxmsgs[3];
volatile uint8 numRxmsgs[3];

/* Filter configurations */
IfxCan_Filter filter[] =
{
		{ 0, IfxCan_FilterElementConfiguration_rejectId,		IfxCan_FilterType_range, 	0x017, 	0x019,	IfxCan_RxBufferId_0 }, /* reject messages with IDs in range 0x17 to 0x19 */
		{ 1, IfxCan_FilterElementConfiguration_storeInRxFifo0, 	IfxCan_FilterType_range, 	0x014, 	0x01A, 	IfxCan_RxBufferId_1 }, /* filter messages with IDs in range 0x014 to 0x1A and store in FIFO0 */
		{ 2, IfxCan_FilterElementConfiguration_storeInRxFifo1,	IfxCan_FilterType_dualId,  	0x184, 	0x187, 	IfxCan_RxBufferId_2 }, /* filter messages with IDs 0x184 or 0x187 store in FIFO1 */
		{ 3, IfxCan_FilterElementConfiguration_storeInRxFifo0, 	IfxCan_FilterType_dualId, 	0x189, 	0x189, 	IfxCan_RxBufferId_3 }, /* filter message with ID 0x189 and store in FIFO0 */
		{ 4, IfxCan_FilterElementConfiguration_storeInRxFifo0, 	IfxCan_FilterType_classic,  0x200, 	0x39F, 	IfxCan_RxBufferId_4 }, /* filter messages according to the ID and mask and store in FIFO0 */
		{ 5, IfxCan_FilterElementConfiguration_rejectId, 		IfxCan_FilterType_classic,  0x201,	0x39F, 	IfxCan_RxBufferId_5 }, /* reject messages according to the ID and mask */
		{ 6, IfxCan_FilterElementConfiguration_storeInRxBuffer, IfxCan_FilterType_none	, 	0x325, 	0x06, 	IfxCan_RxBufferId_6 }, /* filter messages with ID 0x325 and store in dedicated Rx buffer at index 0x6 */
		{ 7, IfxCan_FilterElementConfiguration_storeInRxBuffer,	IfxCan_FilterType_none, 	0x326, 	0x07, 	IfxCan_RxBufferId_7 }, /* filter messages with ID 0x326 and store in dedicated Rx buffer at index 0x7 */
};

#define NUM_OF_FILTERS (sizeof(filter) / sizeof(IfxCan_Filter))

/* Messages to be transmitted */
IfxCan_Message CanMessages [] = {
		/* bufferNumber,"	messageId","	remoteTransmitRequest","	messageIdLength","	errorStateIndicator","	dataLengthCode","	frameMode","	txEventFifoControl","	storeInTxFifoQueue","	readFromRxFifo0",readFromRxFifo1 */
		{0,0x014,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{1,0x015,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{2,0x016,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{3,0x017,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{4,0x018,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{5,0x019,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{6,0x184,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{7,0x187,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{8,0x189,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{9,0x200,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{10,0x201,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_7,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{11,0x220,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{12,0x221,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{13,0x240,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{14,0x241,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{IfxCan_RxBufferId_6 ,0x325,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{IfxCan_RxBufferId_7 ,0x326,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},
		{17,0x327,FALSE,IfxCan_MessageIdLength_standard,FALSE,IfxCan_DataLengthCode_8,IfxCan_FrameMode_standard,FALSE,TRUE,FALSE,FALSE},

};

#define NUM_OF_MSG_IDS (sizeof(CanMessages)/sizeof(IfxCan_Message))

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Handle Tx interrupt on CAN1 Node0.
 *
 * \isrProvider \ref ISR_PROVIDER_CAN1_TX
 * \isrPriority \ref ISR_PRIORITY_CAN1_TX
 *
 */
IFX_INTERRUPT(Can1Tx, 0, IFX_INTPRIO_CAN1_TX)
{
	Ifx_CAN   *canSfr = &MODULE_CAN1;

	if(CAN1_IR0.B.TC == 1)
	{
		Ifx_CAN_N *nodeSfr = IfxCan_getNodePointer(canSfr,(IfxCan_NodeId)0);
		IfxCan_Node_clearInterruptFlag(nodeSfr,IfxCan_Interrupt_transmissionCompleted);
		numTxmsgs[1]++;
	}
}

/** \brief Handle Rx interrupt on CAN0 Node0.
 *
 * \isrProvider \ref ISR_PROVIDER_CAN0_RX
 * \isrPriority \ref ISR_PRIORITY_CAN0_RX
 *
 */
IFX_INTERRUPT(Can0Rx, 0, IFX_INTPRIO_CAN0_RX)
{
	Ifx_CAN   *canSfr =  &MODULE_CAN0;

	if(CAN0_IR0.B.DRX == 1)
	{
		Ifx_CAN_N *nodeSfr = IfxCan_getNodePointer(canSfr,(IfxCan_NodeId)0);
		IfxCan_Node_clearInterruptFlag(nodeSfr,IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
		numRxmsgs[0]++;
		dataIntegrityCheck(0, IfxCan_dedicatedBuffers);
	}

	if(CAN0_IR0.B.RF1N == 1)
	{
		numRxmsgs[0]++;
		Ifx_CAN_N *nodeSfr = IfxCan_getNodePointer(canSfr,(IfxCan_NodeId)0);
		IfxCan_Node_clearInterruptFlag(nodeSfr,IfxCan_Interrupt_rxFifo1NewMessage);
		dataIntegrityCheck(0, IfxCan_fifo1);
	}

	if(CAN0_IR0.B.RF0N == 1)
	{
		numRxmsgs[0]++;
		Ifx_CAN_N *nodeSfr = IfxCan_getNodePointer(canSfr,(IfxCan_NodeId)0);
		IfxCan_Node_clearInterruptFlag(nodeSfr,IfxCan_Interrupt_rxFifo0NewMessage);
		dataIntegrityCheck(0, IfxCan_fifo0);
	}

}

/** \brief Process Dedicated buffer interrupt
 *
 *  Process the data dedicated buffer interrupt data
 */
static void ProcessDedicatedBufferInterrupt(uint32 canNode , IfxCan_MsgMode rxMsgType){

	uint8 DataRecieved=0;
	IfxCan_Message rxMsg;
	uint8 i = 0;
	uint32 data[2];
	IfxCan_Can_initMessage(&rxMsg);

	/* Dummy data */
	data[0] = 0xdeadbeef;
	data[1] = 0xdeadbeef;

	/* Disable reading in FIFOs */
	rxMsg.readFromRxFifo1 = FALSE;
	rxMsg.readFromRxFifo0 = FALSE;

	/* Dedicated buffer occupy buffer 6 */
	if (IfxCan_Node_isRxBufferNewDataUpdated(g_CanFilter.drivers.canNode[canNode].node, IfxCan_RxBufferId_6)) {

		rxMsg.bufferNumber = IfxCan_RxBufferId_6;
		/* Check for received MsgID */
		IfxCan_Can_readMessage(&g_CanFilter.drivers.canNode[canNode], &rxMsg, data);

		if (rxMsg.messageId != 0x325) {
			ErrorInCan++;
		} else {
			DataRecieved = 1;
		}
	}

	/* Dedicated buffer occupy buffer 7 */
	if (IfxCan_Node_isRxBufferNewDataUpdated(g_CanFilter.drivers.canNode[canNode].node, IfxCan_RxBufferId_7)) {

		rxMsg.bufferNumber = IfxCan_RxBufferId_7;
		/* Check for received MsgID */
		IfxCan_Can_readMessage(&g_CanFilter.drivers.canNode[canNode], &rxMsg, data);

		if (rxMsg.messageId != 0x326) {
			ErrorInCan++;
		} else {
			DataRecieved = 1;
		}
	}
	/* Verify the data received */
	if (DataRecieved == 1) {
		for (i = 0; i < 2; ++i)
		{
			if (data[i] != (txData[i]))
			{
				ErrorInCan++;
			}
		}
	}
	InterruptProcessed = 1;
}

/** \brief Process FIFO0 interrupt
 *
 *  Process the data fifo0 interrupt data
 */
static void ProcessFifo0Interrupt(uint32 canNode , IfxCan_MsgMode rxMsgType){
	uint8 DataRecieved=0;
	IfxCan_Message rxMsg;
	uint8 i = 0;
	uint32 data[2];

	IfxCan_Can_initMessage(&rxMsg);

	/* Dummy data */
	data[0] = 0xdeadbeef;
	data[1] = 0xdeadbeef;

	/* Enable FIFO0 reading */
	rxMsg.readFromRxFifo1 = FALSE;
	rxMsg.readFromRxFifo0 = TRUE;


	/* Check for received MsgID */
	IfxCan_Can_readMessage(&g_CanFilter.drivers.canNode[canNode], &rxMsg, data);

	if (rxMsg.messageId != CanMessages[messageNum - 1].messageId ) {
		ErrorInCan++;
	} else {
		DataRecieved = 1;
	}

	/* Verify Data */
	if (DataRecieved == 1) {
		for (i = 0; i < 2; ++i)
		{
			if (data[i] != (txData[i]))
			{
				ErrorInCan++;
			}
		}
	}
	InterruptProcessed = 1;
}

/** \brief Process FIFO1 interrupt
 *
 *  Process the data fifo1 interrupt data
 */
static void ProcessFifo1Interrupt(uint32 canNode , IfxCan_MsgMode rxMsgType){
	uint8 DataRecieved=0;
	IfxCan_Message rxMsg;
	uint8 i = 0;
	uint32 data[2];

	IfxCan_Can_initMessage(&rxMsg);

	/* Dummy data */
	data[0] = 0xdeadbeef;
	data[1] = 0xdeadbeef;

	/* Enable FIFO1 reading */
	rxMsg.readFromRxFifo1 = TRUE;
	rxMsg.readFromRxFifo0 = FALSE;

	/* Check for received MsgID */
	IfxCan_Can_readMessage(&g_CanFilter.drivers.canNode[canNode], &rxMsg, data);

	if (rxMsg.messageId != CanMessages[messageNum - 1].messageId ) {
		ErrorInCan++;
	} else {
		DataRecieved = 1;
	}

	/* Check the data */
	if (DataRecieved == 1) {
		for (i = 0; i < 2; ++i)
		{
			if (data[i] != (txData[i]))
			{
				ErrorInCan++;
			}
		}
	}
	InterruptProcessed = 1;
}


/** \brief Data Integrity Check
 *
 *  Verify the received data with transmitted data
 */
static void dataIntegrityCheck (uint32 canNode , IfxCan_MsgMode rxMsgType)
{

	printf("Receive data\n");

	/* Dedicated buffer interrupt */
	if (rxMsgType == IfxCan_dedicatedBuffers)
	{
		/* Process Dedicated buffer value */
		ProcessDedicatedBufferInterrupt(canNode , rxMsgType);
	}
	if (rxMsgType == IfxCan_fifo0)
	{
		/* Process Dedicated buffer value */
		ProcessFifo0Interrupt(canNode , rxMsgType);
	}
	if (rxMsgType == IfxCan_fifo1)
	{
		/* Process Dedicated buffer value */
		ProcessFifo1Interrupt(canNode , rxMsgType);
	}

}

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void CanFilterDemo_init(void)
{
	IfxScuCcu_Config IfxScuCcuConfig;
	IfxCan_Can_Pins pins;
	boolean  interruptState;
	Ifx_CAN_N *nodeSfr;

	/* disable interrupts */
	interruptState = IfxCpu_disableInterrupts();

	/* Default clock initialisation */
	IfxScuCcu_initConfig(&IfxScuCcuConfig);
	IfxScuCcu_init(&IfxScuCcuConfig);

	/* CAN0 initilaisation */
	{
		/* create module configuration */
		IfxCan_Can_Config canConfig;
		IfxCan_Can_initModuleConfig(&canConfig, &MODULE_CAN0);

		/* initialize module */
		IfxCan_Can_initModule(&g_CanFilter.drivers.can[0], &canConfig);
	}

	/* CAN1 initilaisation */
	{
		/*create module configuration*/
		IfxCan_Can_Config canConfig;
		IfxCan_Can_initModuleConfig(&canConfig, &MODULE_CAN1);

		/*initialize module*/
		IfxCan_Can_initModule(&g_CanFilter.drivers.can[1], &canConfig);
	}


	/*CAN 0 Node0 as receive node*/
	{
		IfxCan_Can_NodeConfig nodeConfig;
		IfxCan_Can_initNodeConfig(&nodeConfig, &g_CanFilter.drivers.can[0]);

		nodeConfig.nodeId = IfxCan_NodeId_0;
		nodeConfig.frame.type = IfxCan_FrameType_receive;

		/* Address offsets in MessageRAM*/
		nodeConfig.messageRAM.standardFilterListStartAddress = 0x110;
		nodeConfig.messageRAM.rxBuffersStartAddress          = 0x200;
		nodeConfig.messageRAM.rxFifo0StartAddress            = 0x300;
		nodeConfig.messageRAM.rxFifo1StartAddress            = 0x400;
		nodeConfig.messageRAM.baseAddress                    = (uint32)&MODULE_CAN0;

		/* Node setting to use FIFO0 , FIFO1 and dedicated buffers*/
		nodeConfig.rxConfig.rxMode 							 = IfxCan_RxMode_sharedAll;
		nodeConfig.rxConfig.rxFifo0Size			= NUM_OF_MSG_IDS;
		nodeConfig.rxConfig.rxFifo1Size			= NUM_OF_MSG_IDS;
		nodeConfig.rxConfig.rxFifo0OperatingMode = IfxCan_RxFifoMode_overwrite;
		nodeConfig.rxConfig.rxFifo1OperatingMode =  IfxCan_RxFifoMode_blocking;

		/* Filter config to accept non matching frames in FIFO1*/
		nodeConfig.filterConfig.standardListSize    = NUM_OF_FILTERS;
		nodeConfig.filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;

		/* Pin configuration*/
		pins.rxPin = &IfxCan_RXD00B_P20_7_IN;
		pins.rxPinMode = IfxPort_InputMode_pullUp;
		pins.txPin = &IfxCan_TXD00_P20_8_OUT;
		pins.txPinMode = IfxPort_OutputMode_pushPull;
		pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
		nodeConfig.pins = &pins;

		/* Interrupt configuration for FIFO0*/
		nodeConfig.interruptConfig.rxf0n.priority = IFX_INTPRIO_CAN0_RX ;
		nodeConfig.interruptConfig.rxf0n.interruptLine =  IfxCan_InterruptLine_14;
		nodeConfig.interruptConfig.rxf0n.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

		/* Interrupt configuration for FIFO1*/
		nodeConfig.interruptConfig.rxf1n.priority = IFX_INTPRIO_CAN0_RX ;
		nodeConfig.interruptConfig.rxf1n.interruptLine =  IfxCan_InterruptLine_13;
		nodeConfig.interruptConfig.rxf1n.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

		/* Interrupt configuration for dedicated buffer*/
		nodeConfig.interruptConfig.reint.priority = IFX_INTPRIO_CAN0_RX ;
		nodeConfig.interruptConfig.reint.interruptLine =  IfxCan_InterruptLine_12;
		nodeConfig.interruptConfig.reint.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

		IfxCan_Can_initNode(&g_CanFilter.drivers.canNode[0], &nodeConfig);

		nodeSfr = IfxCan_getNodePointer(g_CanFilter.drivers.canNode[0].can, nodeConfig.nodeId);

		/* Enable interrupts */
		IfxCan_Node_enableConfigurationChange(nodeSfr);
		IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo0NewMessage);
		IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo1NewMessage);
		IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
		IfxCan_Node_disableConfigurationChange(nodeSfr);

		printf(" Rx Node Base Address  = %x\n",(unsigned int)g_CanFilter.drivers.canNode[0].node);
	}

	/* CAN 1 Node 0 as transmit node*/
	{
		IfxCan_Can_NodeConfig nodeConfig;

		IfxCan_Can_initNodeConfig(&nodeConfig, &g_CanFilter.drivers.can[1]);

		nodeConfig.nodeId = IfxCan_NodeId_0;
		nodeConfig.frame.type = IfxCan_FrameType_transmit;
		nodeConfig.txConfig.dedicatedTxBuffersNumber = 2;
		nodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
		nodeConfig.txConfig.txFifoQueueSize			= NUM_OF_MSG_IDS;

		/* Address offsets in MessageRAM*/
		nodeConfig.messageRAM.txBuffersStartAddress = 0x400;
		nodeConfig.messageRAM.baseAddress           = (uint32)&MODULE_CAN1;

		/* Pin configuration */
		pins.rxPin = &IfxCan_RXD10C_P23_0_IN;
		pins.rxPinMode = IfxPort_InputMode_pullUp;
		pins.txPin = &IfxCan_TXD10_P23_1_OUT;
		pins.txPinMode = IfxPort_OutputMode_pushPull;
		pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
		nodeConfig.pins = &pins;

		/*Enabling interrupts*/
		nodeConfig.interruptConfig.traco.priority  = IFX_INTPRIO_CAN1_TX ;
		nodeConfig.interruptConfig.traco.interruptLine =  IfxCan_InterruptLine_15;
		nodeConfig.interruptConfig.traco.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

		/*initialize module*/
		IfxCan_Can_initNode(&g_CanFilter.drivers.canNode[1], &nodeConfig);

		nodeSfr = IfxCan_getNodePointer(g_CanFilter.drivers.canNode[1].can, nodeConfig.nodeId);

		IfxCan_Node_enableConfigurationChange(nodeSfr);
		IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_transmissionCompleted);
		/* disable configuration change CCCR.CCE = 0 */
		IfxCan_Node_disableConfigurationChange(nodeSfr);

		printf(" Tx Node Base Address  = %x\n",(unsigned int)g_CanFilter.drivers.canNode[1].node);
	}

	/* Filter Configuration for CAN0 Node0 */
	{
		uint8 i =0;
		for( i = 0; i< NUM_OF_FILTERS; i++ ) {
			IfxCan_Can_setStandardFilter(&g_CanFilter.drivers.canNode[0],&filter[i]);
		}
	}

	/* enable interrupts again */
	IfxCpu_restoreInterrupts(interruptState);

	printf("CAN is initialised\n");
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void CanFilterDemo_run(void)
{
	uint8 i= 0;

	printf("Send data\n");

	{
		/* Initialise the message structure */
		for (i = 0; i < NUM_OF_MSG_IDS; i++) {

			messageNum++ ;
			txData[0] = 0xC0C0C01A | CanMessages[messageNum - 1].messageId;
			txData[1] = 0xBA5EBA11 | CanMessages[messageNum - 1].messageId;

			InterruptProcessed = 0;

			/* Transmit the data */
			while (IfxCan_Can_sendMessage(&g_CanFilter.drivers.canNode[1], &CanMessages[i], txData) == IfxCan_Status_notSentBusy);
			/* If the message is supposed to be rejected dont wait for interrupt response */
			if (    CanMessages[messageNum -1].messageId != 0x17  &&
					CanMessages[messageNum -1].messageId != 0x18  &&
					CanMessages[messageNum -1].messageId != 0x19  &&
					CanMessages[messageNum -1].messageId != 0x201 &&
					CanMessages[messageNum -1].messageId != 0x221 &&
					CanMessages[messageNum -1].messageId != 0x241
				)
			{
			   /* Wait till interrupt is processed */
				while(InterruptProcessed == 0);
			}
		}

		/* Number of rejected messages */
		rejectNumMsgs = messageNum - numRxmsgs[0];
		messageNum = 0;
		numRxmsgs[0] = 0;
	}

	/* Rejected message count*/
	if (rejectNumMsgs != 6) {
		ErrorInCan++;
	}
	printf("Receive data\n");

	if (ErrorInCan)
	{
		printf("ERROR: received data doesn't match with expected data (%d mismatches)\n", ErrorInCan);
	}
	else
	{
		printf("OK: received data matches with expected data\n");
	}

	printf("Can Basic data transfers are finished\n");
}

