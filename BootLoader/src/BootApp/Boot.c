/*
 * Boot.c
 *
 *  Created on: May 13, 2022
 *      Author: Ahmed Soliman
 */
#include "diag/Trace.h"

#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "../MCAL/Rcc.h"
#include "../MCAL/GPIO.h"
#include "../MCAL/NVIC.h"
#include "../MCAL/Usart.h"
#include "../MCAL/FlashDriver.h"
#include "Boot.h"

#define DATA_BUFFER_MAX_SIZE             4096   // 4K DATA
#define SECTOR_SIZE                      16384  // THE SIZE OF THE SECTOR 16K
#define ADDRESS_APPINFO_SECTOR       (0x08004000L)
#define ADDRESS_FIRSTAPP_SECTOR      (0x08008000L)

#define OK                1
#define NOK               0

/* BOOT STATE MACHINE FRAMES */
typedef enum {
	DOWENLOAD_REQ_FRAME,
	DOWENLOAD_DATA_FRAME,
	DOWENLOAD_DONE_FRAME,
}flashing_States;





u8 Application[1];
pu8 App_ptr = Application;
app_info_t AppInfo;
extern func_t  AppFun ;


/*
 *  SENDING A NEGATIVE RESPONCE 0XF7
 */

static void Send_NegativeResponce(void)
{
	Uart_SendByteSynch(NEGATIVE_RESPONCE);
}

/*
 * INTIALLIZE UART AND ENABLE ITS CLOCK
 * ERASE THE APPLICATION SECTOR
 */
re_t Boot_FlashingInit(void)
{
	re_t Loc_enuReturn = TRUE;
	/* ENABLE CLOCK TO THE UART AND GPIOB */
	Rcc_enuPerpheralClk_Control(Rcc_u8APB2_BUS,Rcc_u32USART1EN_Perpheral_APB2,Rcc_u8ClkON_State);
	Rcc_enuPerpheralClk_Control(Rcc_u8AHB1_BUS,Rcc_u32GPIOBEN_Perpheral_AHB1, Rcc_u8ClkON_State);

	//	/* ENABLE UART INTERRUPT */
	//	NVIC_EnableIRQ(USART1_IRQn);

	/* CONFIGURE THE TX AND RX PIN IN GPIO */
	Gpio_PinCfg_t Rx,Tx;
	Rx.Gpio_PinMode = Gpio_u8PinMode_AFPP;
	Rx.Gpio_PinNumber = Gpio_u64PinNumber_PIN7;
	Rx.Gpio_PinPort = GPIOB;
	Rx.Gpio_PinSpeed = Gpio_u16SpeedCfg_HIGH;
	Gpio_initPin(&Rx);
	Gpio_AlternativeFunctionPin(Gpio_u64PinNumber_PIN7,Gpio_u16AF7_UART12,GPIOB);

	Tx.Gpio_PinMode = Gpio_u8PinMode_AFPP;
	Tx.Gpio_PinNumber = Gpio_u64PinNumber_PIN6;
	Tx.Gpio_PinPort = GPIOB;
	Tx.Gpio_PinSpeed = Gpio_u16SpeedCfg_HIGH;
	Gpio_initPin(&Tx);
	Gpio_AlternativeFunctionPin(Gpio_u64PinNumber_PIN6,Gpio_u16AF7_UART12,GPIOB);

	/*
	 *  CONFIGURE UART BUADRATE , INTERRUPT ENABLE , MODE IN UART CONFIGURATION FILE SPCIEFIED FOR BOOTLOADER APP
	 */
	Uart_Init();

	return Loc_enuReturn;
}

/*
 * FLASHING STATES
 */
re_t Boot_FlashingTask(void)
{
	re_t Loc_enuReturn = TRUE;
	static flashing_States Flashing_RxState = DOWENLOAD_REQ_FRAME ; // INDICTION TO WHAT FRAME STATE THAT WILL RECEIVE
	u8 Rx_Byte[4097] = {0};    // MAKING A BUFFER HOLDER OF MAX 4K DATA AND 1 BYTE FOR COMMAND
	u16 size = 8;        // THE FIRST SOFTWARE FRAME NUMBER OF SIZE IS 8 BYTES
	u16 Loc_u16Iter = 0;
	u16 Uart_PreSize = 0;
	static u8 NegativeFlag = OK;
	/* CASTING TO GET THE FRAME FORM */
	FrameForm_Cmd *Frame_cmd = (FrameForm_Cmd *) Rx_Byte; //CASTING THE BUFFER TO THE FRAME FORM OF THE COMMAND FRAME
	FrameForm_Data *Frame_Data = (FrameForm_Data *) Rx_Byte;
	Buffer_t Uart_Data={
			.data = Rx_Byte,
			.size = size,
	};
	Buffer_t Flashing_Data = {
			.data = Frame_Data->Data,
			.size = DATA_BUFFER_MAX_SIZE,
	};

	switch(Flashing_RxState)
	{
	case DOWENLOAD_REQ_FRAME:

		Uart_SendByteSynch(PROGRAMMING_REQ + POSTIVE_VALUE);
		Uart_ReceiveBufSynchZCopy(&Uart_Data);
		if(Frame_cmd->Cmd == FIRST_FRAME_START)
		{
			AppInfo.size = Frame_cmd->len;                     // THE LENGTH OF THE APPLICATION BYTES
			AppInfo.App_EntryPoint = *((u32*)Frame_cmd->Data); // CAST TO GET THE THE 4 BYTE FOR ENTRY POINT ADDRESS
			Uart_SendByteSynch(FIRST_FRAME_START + POSTIVE_VALUE); //SENDING A POSITIVE RESPONCE
			/* ERASE FLASH SECTORS DEPEND ON APPLICATION SIZE */
			size = (AppInfo.size / SECTOR_SIZE);
			if(AppInfo.size%SECTOR_SIZE != 0)
			{
				size++;
			}
			/* LIMITE THE SIZE OF THE APPLICATION FOR NOW TO 3 SECTOR */
			if(size > 3)
			{
				size = 3;
			}
			for(Loc_u16Iter = 0; Loc_u16Iter < size; Loc_u16Iter++)
			{
				/* EACH ITERATION THE NEXT SECTOR WILL ERASE STARTING FROM SECTOR1 CAUSE SECTOR0 FOR BOOTLOADER */
				FlashDriver_SectorErase(SECTOR2_16KB+(Loc_u16Iter * SECTOR_SIZE));
			}

			/* 	MAKE SURE THE STATE IS RECIEVED CORRECTLY */
			if(NegativeFlag == OK)
			{
				Flashing_RxState = DOWENLOAD_DATA_FRAME;
			}

			else
			{
				/* IF THERE WAS AN ERROR IN THE FIRST FRAME COMMAND BYTE */
				Send_NegativeResponce();
				NegativeFlag = NOK;
			}
		}
		break;
	case DOWENLOAD_DATA_FRAME:
		size = (AppInfo.size / DATA_BUFFER_MAX_SIZE);
		if(AppInfo.size%DATA_BUFFER_MAX_SIZE != 0)
		{
			size++;
		}
		if(AppInfo.size <= DATA_BUFFER_MAX_SIZE)
		{
			/* IF THE APPLICATION SIZE SMALLER THAT THAN MAX BUFFER SIZE ENABLE UART TO RX UNTILL THE APP SIZE */
			Uart_Data.size = AppInfo.size+1; //ADD 1 BYTES FOR COMMAND
		}
		else
		{
			Uart_Data.size = 4097;  // AT FIRST ENABLE UART TO GET THE BUFFER SIZE
		}
		for(Loc_u16Iter = 0; Loc_u16Iter < size; Loc_u16Iter++)
		{
			/* EACH TIME WE RX DATA BUFFER WITH MAX SIZE 4K-4BYTE OF THE LENGTH AND COMMAND BYTE */
			Uart_ReceiveBufSynchZCopy(&Uart_Data);
			if(Frame_Data->Cmd == DATA_FRAME)
			{
				FlashDriver_Unlock();
				FlashDriver_BufferProgram(&Flashing_Data,(pu32)(ADDRESS_FIRSTAPP_SECTOR+(Uart_PreSize)));
				FlashDriver_Lock();
				Uart_PreSize += Flashing_Data.size;
				/* AFTER EACH BUFFER RX IT SEND POSITIVE RESPONCE */
				Uart_SendByteSynch(DATA_FRAME+POSTIVE_VALUE);

				/* DECIDE THE NEXT UART RX BUFFER */
				if((AppInfo.size -((Loc_u16Iter+1) * DATA_BUFFER_MAX_SIZE)) <=  DATA_BUFFER_MAX_SIZE)
				{
					Uart_Data.size = AppInfo.size -((Loc_u16Iter+1)*DATA_BUFFER_MAX_SIZE) + 1;
					/* NOTE THAT FLASH PROGRAMING SIZE IS 4 BYTE */
					Flashing_Data.size = (AppInfo.size -((Loc_u16Iter+1)*DATA_BUFFER_MAX_SIZE));

				}
				else
				{
					Uart_Data.size = 4097;
				}

			}
			else
			{
				/* THERE WAS AN ERROE IN THE COMMAN FRAME */
				Send_NegativeResponce();
				NegativeFlag = NOK;

			}
		}
		/* 	MAKE SURE THE STATE IS RECIEVED CORRECTLY TO GO TO THE NEXT */
		if(NegativeFlag == OK)
		{
			Flashing_RxState = DOWENLOAD_DONE_FRAME;
		}
		break;
	case DOWENLOAD_DONE_FRAME:
		Uart_Data.size = 1; // WAIT FOR ONE BYE INDICATING THAT THE TRANSFER IS DONE
		Uart_ReceiveBufSynchZCopy(&Uart_Data);
		if(Frame_Data->Cmd == COMMUNICATION_DONE)
		{
			Uart_SendByteSynch(COMMUNICATION_DONE + POSTIVE_VALUE);
			/* FLASH THE APPLICATION INFO AT THE SECTOR 1 SPCIFIC FOR INFO */
			Flashing_Data.data = (pu8)&AppInfo;
			Flashing_Data.size = 8;
			FlashDriver_Unlock();
			FlashDriver_BufferProgram(&Flashing_Data,((pu32)ADDRESS_APPINFO_SECTOR));
			FlashDriver_Lock();
			AppFun =  (func_t)(((app_info_t*)(APP_INFO_SECOTR))->App_EntryPoint);
			AppFun();

		}
		else
		{
			/* THERE WAS AN ERROE IN THE COMMAN FRAME */
			Send_NegativeResponce();
			NegativeFlag = NOK;
		}

		break;
	}


	return Loc_enuReturn;
}


