/**
 * \file QspiEepromDemo.c
 * \brief Qspi connected to EEPROM Demo
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
#include "ConfigurationIsr.h"
#include "Ifx_Assert.h"
#include "QspiEepromDemo.h"

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
App_QspiEeprom g_QspiEeprom; /**< \brief Qspi EEPROM global data */
/**< \brief TX data consisting of EEPROM Read Command(1 byte),
 * Upper byte of Address, Lower byte of address and dummy Tx Data (9 bytes)*/
uint8 QspiEeprom_readcmd_address_txdata[SPI_BUFFER_SIZE] = {3,0,0,0,0,0,0,0,0,0,0,0};
/**< \brief RX data buffer(12 bytes)*/
uint8 QspiEeprom_readcmd_rxdata[SPI_BUFFER_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0};

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \addtogroup IfxLld_Demo_QspiEepromDemo_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Qspi Master (QSPI0) driver
 * \{ */
IFX_INTERRUPT(ISR_qspi0_eeprom_Tx, 0, ISR_PRIORITY_QSPI0_TX);
IFX_INTERRUPT(ISR_qspi0_eeprom_Rx, 0, ISR_PRIORITY_QSPI0_RX);
IFX_INTERRUPT(ISR_qspi0_eeprom_Er, 0, ISR_PRIORITY_QSPI0_ER);
/** \} */

/** \} */

/** \brief Handle qspi0_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI0_TX
 * \isrPriority \ref ISR_PRIORITY_QSPI0_TX
 *
 */
void ISR_qspi0_eeprom_Tx(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrTransmit(&g_QspiEeprom.drivers.spiMaster);
}


/** \brief Handle qspi0_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI0_RX
 * \isrPriority \ref ISR_PRIORITY_QSPI0_RX
 *
 */
void ISR_qspi0_eeprom_Rx(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrReceive(&g_QspiEeprom.drivers.spiMaster);
}


/** \brief Handle qspi0_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI0_ER
 * \isrPriority \ref ISR_PRIORITY_QSPI0_ER
 *
 */
void ISR_qspi0_eeprom_Er(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_QspiEeprom.drivers.spiMaster);
}



/** \brief Qspi Master initilisation
 *
 * This function initialises Qspi0 in master mode.
 */
static void IfxQspi_Masterinit(void)
{
    IfxQspi_SpiMaster_Config        spiMasterConfig;
    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;

    {
        /* create module config */
        IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, &MODULE_QSPI0);

        /* set the maximum baudrate */
        spiMasterConfig.base.maximumBaudrate = 1000000;

        /* ISR priorities and interrupt target */
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI0_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI0_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI0_ER;
        spiMasterConfig.base.isrProvider = ISR_PROVIDER_QSPI0;

        /* pin configuration */
        const IfxQspi_SpiMaster_Pins pins = {&IfxQspi0_SCLK_P20_11_OUT,                               /* SCLK */
                                             IfxPort_OutputMode_pushPull,
                                             &IfxQspi0_MTSR_P20_14_OUT,  IfxPort_OutputMode_pushPull, /* MTSR */
											 &IfxQspi0_MRSTA_P20_12_IN,  IfxPort_InputMode_pullUp,    /* MRST */
                                             IfxPort_PadDriver_cmosAutomotiveSpeed1                   /* pad driver mode */
        };
        spiMasterConfig.pins = &pins;


        /* initialize module */
        IfxQspi_SpiMaster_initModule(&g_QspiEeprom.drivers.spiMaster, &spiMasterConfig);

    }

    {
        /* create channel config */
        IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
            &g_QspiEeprom.drivers.spiMaster);

        /* set the baudrate for this channel */
        spiMasterChannelConfig.base.baudrate = 1000;

        const IfxQspi_SpiMaster_Output slsOutput = {&IfxQspi0_SLSO0_P20_8_OUT,						   /*CS */
                                                    IfxPort_OutputMode_pushPull,
                                                    IfxPort_PadDriver_cmosAutomotiveSpeed1};

        spiMasterChannelConfig.sls.output.pin    = slsOutput.pin;
        spiMasterChannelConfig.sls.output.mode   = slsOutput.mode;
        spiMasterChannelConfig.sls.output.driver = slsOutput.driver;
		
        /*Data valid on Rising Edge of Clock*/
        spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;

        /* initialize channel */
        IfxQspi_SpiMaster_initChannel(&g_QspiEeprom.drivers.spiMasterChannel,
            &spiMasterChannelConfig);
    }


    /*Optional Lead and Trailing delays for CS*/
	g_QspiEeprom.drivers.spiMasterChannel.bacon.B.TRAIL = (uint8)(7);
	g_QspiEeprom.drivers.spiMasterChannel.bacon.B.TPRE = (uint8)(4);
	g_QspiEeprom.drivers.spiMasterChannel.bacon.B.LEAD = (uint8)(7);
	g_QspiEeprom.drivers.spiMasterChannel.bacon.B.LPRE = (uint8)(5);
}



/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void IfxQspiEepromDemo_init(void)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxQspi_Masterinit();

    g_QspiEeprom.eeprom_status = 0;

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}

/** \brief S/W Delay API
 * \param count delay count
 * \return None
 */
void IfxQspiEeprom_Delay(uint32 count)
{
    while(count!=0)
    {
    	count--;
    }
}

/** \brief Send Write Enable Command(0x6) to EEPROM
 * \param None
 * \return None
 */
void IfxQspiEeprom_WriteEnableCommand(void)
{
    uint8 write_enable_command = EEPROM_WRITE_ENABLE_COMMAND_VALUE;
    uint8 eeprom_command_size = EEPROM_WRITE_ENABLE_COMMAND_SIZE;

    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}

    IfxQspi_SpiMaster_exchange(&g_QspiEeprom.drivers.spiMasterChannel, &write_enable_command, NULL_PTR, eeprom_command_size);

    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}
}

/** \brief Checks current status of EEPROM
 * \param None
 * \return 16-bit status value
 */
uint16 IfxQspiEeprom_StatusCheckCommand(void)
{
    uint8 get_status_command[2] = {5,0};
    uint8 received_status[2] = {0,0};
    uint8 eeprom_command_size = EEPROM_READ_COMMAND_SIZE;

    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}

    IfxQspi_SpiMaster_exchange(&g_QspiEeprom.drivers.spiMasterChannel, &get_status_command[0], &received_status[0], eeprom_command_size);

    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}

    uint16 status = (received_status[0] << 8) + received_status[1];
    return status;
}

/** \brief Writes Data to EEPROM
 * \param add_data pointer to data containing eeprom write command, address and data to be programmed
 * \return None
 */
void IfxQspiEeprom_Write(uint8 *add_data, uint16 count)
{
    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}

    uint32 delay_count=0x1000;
    IfxQspiEeprom_Delay(delay_count);

    IfxQspi_SpiMaster_exchange(&g_QspiEeprom.drivers.spiMasterChannel, add_data, NULL_PTR, count);

    while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
           == SpiIf_Status_busy)
    {}

    delay_count=0x3000;
    IfxQspiEeprom_Delay(delay_count);

}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void IfxQspiEepromDemo_run(void)
{
	/*Send Write Enable Command to EEPROM*/
	IfxQspiEeprom_WriteEnableCommand();
	IfxQspiEeprom_Delay(0x10000);
	g_QspiEeprom.eeprom_status = 0x0000;

	/*Get EEPROM Status after sending write enable command*/
	g_QspiEeprom.eeprom_status = IfxQspiEeprom_StatusCheckCommand();

	/*Command - 1 byte, Address - 2 bytes, Data - 2 bytes.
	 * Increasing count will increase number of bytes written/read.*/
	uint16 count = 5;
	uint32 delay_count = 0x1000;
	/*Flag for Write Event*/
	boolean EEPROM_WRITE_EVENT = FALSE;

	/*EEPROM Write Command = 2, Address Higher = 0, Address Lower = 0, Data[0] = 7, Data[1] = 7,
	 * Rest of Data initialized to 1, but not sent as count = 5 only. Increase count value to write more bytes*/
	uint8 writecmd_address_txdata[12] = {2,0,0,0x07,0x07,0x01,0x01,0x01,0x01,0x01,0x01,0x01};

	/*Check if EEPROM Write is enabled, only then write to memory*/
	if((g_QspiEeprom.eeprom_status & 0x0003) == 2)
	{
		EEPROM_WRITE_EVENT = TRUE;

		IfxQspiEeprom_Delay(delay_count);

		/*Write to EEPROM*/
		IfxQspiEeprom_Write(&writecmd_address_txdata[0], count);

		while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
			   == SpiIf_Status_busy)
		{}

		IfxQspiEeprom_Delay(1000000);
		/*Get EEPROM Status after writing data*/
		g_QspiEeprom.eeprom_status = IfxQspiEeprom_StatusCheckCommand();

	}

	IfxQspiEeprom_Delay(100000);
	while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
		   == SpiIf_Status_busy)
	{}

	IfxQspiEeprom_Delay(delay_count);

	/*Read from EEPROM*/
	IfxQspi_SpiMaster_exchange(&g_QspiEeprom.drivers.spiMasterChannel, &QspiEeprom_readcmd_address_txdata[0], &QspiEeprom_readcmd_rxdata[0], count);

	while (IfxQspi_SpiMaster_getStatus(&g_QspiEeprom.drivers.spiMasterChannel)
		   == SpiIf_Status_busy)
	{}
	delay_count=0x100000;
	IfxQspiEeprom_Delay(delay_count);

	if (EEPROM_WRITE_EVENT)
	{
	    printf("EEPROM Write and Read was performed. Checking received data\n");
	    {
	        uint32 errors       = 0;

			for (uint16 i = 3; i < count; i++)
			{
				if (writecmd_address_txdata[i] != QspiEeprom_readcmd_rxdata[i])
				{
					++errors;
				}
			}


	        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, errors == 0);

	        if (errors)
	        {
	            printf("ERROR: EEPROM Read is not consistent (%lu mismatches)\n", errors);
	        }
	        else
	        {
	            printf("OK: Received data matches with expected data\n");
	        }
	    }
	}

	else
	{
		printf("EEPROM write enable was unsuccessful, only EEPROM Read was performed.\n");
	}

}