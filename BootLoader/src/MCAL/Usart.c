/*
 * Usart.c
 *
 *  Created on: Apr 12, 2022
 *      Author: Ahmed Soliman
 */
#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "Usart.h"
#include "Usart_cfg.h"
#include "Dma.h"



#define CLR_COMMMODE_BITS    (0x0012)
#define CLR_STOPBITS         (0x2000)
#define CLR_INTERENRx_BITS   (0x0020)
#define CLR_INTERENTx_BITS   (0x00C0)

#define DMA_TxENABLE         (0x80)

#define TC_FLAG           6
#define TXE_FLAG          7
#define RXNE_FLAG         0x20



typedef struct{
	volatile u32 USART_SR;
	volatile u32 USART_DR;
	volatile u32 USART_BRR;
	volatile u32 USART_CR1;
	volatile u32 USART_CR2;
	volatile u32 USART_CR3;
	volatile u32 USART_GTPR;
}usart_reg_t;




/*GLOBLA BUFFER COPY FROM APPLICATION  TO SEND DATA */
Buffer_t APP_TxBUF_Copy;
/*GLOBLA BUFFER COPY FROM APPLICATION  TO RESEIVE DATA */
Buffer_t APP_RxBUF_Copy;
/* GLOBAL ENUM UART Tx STATE INDICATION */
static Uart_state_t* Uart_TxState;
/* GLOBAL ENUM UART Rx STATE INDICATION */
static Uart_state_t* Uart_RxState ;


extern uart_config_t u1;


/* INTR HANDLE FOR CAHNNEL UART 1*/
void USART1_IRQHandler(void)
{
	volatile u32 Loc_u32StatusReg = ((usart_reg_t*)USART1)->USART_SR;
	if(GET_BIT(Loc_u32StatusReg,TC_FLAG) == 1 && *Uart_TxState == Uart_BUSY)
	{
		if(APP_TxBUF_Copy.size == APP_TxBUF_Copy.used)
		{
			//THAT MEAN THE BYTE IS SEND SO CALL NOTIFY DONE FUNCTION IF NEEDED
			*Uart_TxState = Uart_TxDONE;
			(((usart_reg_t*)USART1)->USART_CR1) &= ~(CLR_INTERENTx_BITS);
			/* DISABLE THE TE */
			((usart_reg_t*)USART1)->USART_CR1 &= ~(UART_TxONLY_EN);
		}
		else
		{
			/* SEND THE CURRENT DATA IN THE BUFFER TO DR */
			((usart_reg_t *)USART1)->USART_DR = APP_TxBUF_Copy.data[APP_TxBUF_Copy.used];
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_TxBUF_Copy.used++;

		}
	}

	if((Loc_u32StatusReg & RXNE_FLAG) != 0)
	{
		/* CHECK THE STATE OF THE RXE IF IT 1 THEAT THE MEAN THAT THE DATA IN DR READY TO BE READ */
		if(APP_RxBUF_Copy.used == APP_RxBUF_Copy.size)
		{
			/* THAT MEAN THAT THE BUFFER IS FULLY RECEIVED */
			*Uart_RxState = Uart_RxDONE;
			(((usart_reg_t*)USART1)->USART_CR1) &= ~(CLR_INTERENRx_BITS);
			/* DISABLE THE TE */
			((usart_reg_t*)USART1)->USART_CR1 &= ~(UART_RxONLY_EN);
		}
		else
		{
			/* HOLD THE CURRENT DATA FROM THE DR TO DATA POSITION */
			APP_RxBUF_Copy.data[APP_RxBUF_Copy.used] = ((usart_reg_t*)USART1)->USART_DR;
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_RxBUF_Copy.used++;
		}
	}
}

/* INTR HANDLE FOR CAHNNEL UART 2*/
void USART2_IRQHandler(void)
{
	volatile u32 Loc_u32StatusReg = ((usart_reg_t*)USART2)->USART_SR;
	if(GET_BIT(Loc_u32StatusReg,TC_FLAG) == 1 && (((usart_reg_t*)USART2)->USART_CR1 & UART_TxONLY_EN))
	{
		if(APP_TxBUF_Copy.size == APP_TxBUF_Copy.used)
		{
			//THAT MEAN THE BYTE IS SEND SO CALL NOTIFY DONE FUNCTION IF NEEDED
			*Uart_TxState = Uart_TxDONE;
			(((usart_reg_t*)USART2)->USART_CR1) &= ~(CLR_INTERENTx_BITS);
			/* DISABLE THE TE */
			((usart_reg_t*)USART2)->USART_CR1 &= ~(UART_TxONLY_EN);
		}
		else
		{
			/* SEND THE CURRENT DATA IN THE BUFFER TO DR */
			((usart_reg_t *)USART2)->USART_DR = APP_TxBUF_Copy.data[APP_TxBUF_Copy.used];
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_TxBUF_Copy.used++;

		}
	}



	if((Loc_u32StatusReg & RXNE_FLAG) != 0)
	{
		/* CHECK THE STATE OF THE RXE IF IT 1 THEAT THE MEAN THAT THE DATA IN DR READY TO BE READ */
		if(APP_RxBUF_Copy.used == APP_RxBUF_Copy.size)
		{
			/* THAT MEAN THAT THE BUFFER IS FULLY RECEIVED */
			*Uart_RxState = Uart_RxDONE;
		}
		else
		{
			/* HOLD THE CURRENT DATA FROM THE DR TO DATA POSITION */
			APP_RxBUF_Copy.data[APP_RxBUF_Copy.used] = ((usart_reg_t*)USART2)->USART_DR;
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_RxBUF_Copy.used++;
		}
	}
}

/* INTR HANDLE FOR CAHNNEL UART 3*/
void USART6_IRQHandler(void)
{
	volatile u32 Loc_u32StatusReg = ((usart_reg_t*)USART6)->USART_SR;
	if(GET_BIT(Loc_u32StatusReg,TC_FLAG) == 1 && (((usart_reg_t*)USART6)->USART_CR1 & UART_TxONLY_EN))
	{
		if(APP_TxBUF_Copy.size == APP_TxBUF_Copy.used)
		{
			//THAT MEAN THE BYTE IS SEND SO CALL NOTIFY DONE FUNCTION IF NEEDED
			*Uart_TxState = Uart_TxDONE;
			(((usart_reg_t*)USART6)->USART_CR1) &= ~(CLR_INTERENTx_BITS);
			/* DISABLE THE TE */
			((usart_reg_t*)USART6)->USART_CR1 &= ~(UART_TxONLY_EN);
		}
		else
		{
			/* SEND THE CURRENT DATA IN THE BUFFER TO DR */
			((usart_reg_t *)USART6)->USART_DR = APP_TxBUF_Copy.data[APP_TxBUF_Copy.used];
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_TxBUF_Copy.used++;

		}
	}


	if((Loc_u32StatusReg & RXNE_FLAG) != 0)
	{
		/* CHECK THE STATE OF THE RXE IF IT 1 THEAT THE MEAN THAT THE DATA IN DR READY TO BE READ */
		if(APP_RxBUF_Copy.used == APP_RxBUF_Copy.size)
		{
			/* THAT MEAN THAT THE BUFFER IS FULLY RECEIVED */
			*Uart_RxState = Uart_RxDONE;
		}
		else
		{
			/* HOLD THE CURRENT DATA FROM THE DR TO DATA POSITION */
			APP_RxBUF_Copy.data[APP_RxBUF_Copy.used] = ((usart_reg_t*)USART6)->USART_DR;
			/* INCREAMENT THE VALUE OF THE USED IN THE BUFFER */
			APP_RxBUF_Copy.used++;
		}
	}
}


/* UART INIT FUUNCTION */
uart_error_t Uart_Init(void)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	u16 Loc_u16Reg = 0;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	/*
	 * NOTE
	 *
	 * THE VALUE OF THE OVER8 WILL LET TO THE DEFAULT VALUE 0 SET THE FRACTION BITS AS 4 BITS
	 *
	 * THE VALUE OF THE M BIT IS LET AS DEFUALT VALUE TO KEEP THE DATA BITS IN THE FRAME 8 BITS
	 */

	/* ENBALE UART COMMUNICATION */
	(Loc_Uart_Reg)->USART_CR1 |= UART_ENABLE;

	/* CONFIGURE THE NUMBER OF THE STOP BIT */
	Loc_u16Reg = (Loc_Uart_Reg)->USART_CR2;
	Loc_u16Reg &= ~(CLR_STOPBITS);       //CLR THE BITS OF THE STOP SELECT
	Loc_u16Reg |= u1.Uart_NumStopB;     // ASSIGN THE NEW VALUE
	(Loc_Uart_Reg)->USART_CR2 = Loc_u16Reg;

	/* SET THE BUAD RATE VALUE IN USART_BRR */
	(Loc_Uart_Reg)->USART_BRR  = u1.Uart_BUADRATE;


	/* INIT THE BUSY INDICATION FLAG TO BE DONE */
	u1.Uart_RxState = Uart_RxDONE;
	u1.Uart_TxState = Uart_TxDONE;



	return Loc_enuErrorState;
}

/* SEND A BUFFER OF DATA USING ASYNCHRONOUS FUNCTION NOTE TO USE THIS FUNCTION ENABLE INTERRUPT*/
uart_error_t Uart_SendBufferAsychZCopy(Buffer_t* Data)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	u32 Loc_u32Reg = 0;
	if(u1.Uart_TxState == Uart_TxDONE)
	{
		if(Data->data != NULL)
		{
			APP_TxBUF_Copy.data = Data->data ;
			APP_TxBUF_Copy.size = Data->size;
			APP_TxBUF_Copy.used = 0;
			Uart_TxState = &(u1.Uart_TxState);
			u1.Uart_TxState = Uart_BUSY;
			/* SET THE MODE TO ENABLE TX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_TxONLY_EN;
			/* SEND THE FIRST BUTE MANUALLY */
			(Loc_Uart_Reg)->USART_DR = APP_TxBUF_Copy.data[APP_TxBUF_Copy.used];
			APP_TxBUF_Copy.used++;
			/* CONFIGURE THE INTERRUPT ENABLE BITS */
			Loc_u32Reg = (Loc_Uart_Reg)->USART_CR1;
			Loc_u32Reg &= ~(CLR_INTERENTx_BITS);   //CLR THE BITS OF THE Interrupt enable SELECT
			Loc_u32Reg |= CLR_INTERENTx_BITS;     // ASSIGN THE NEW VALUE
			(Loc_Uart_Reg)->USART_CR1 = Loc_u32Reg;
		}
		else
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
	}
	else
	{
		/* INDICATED THAT UART STILL NOT FINISH THE LAST PROCESS */
		Loc_enuErrorState = Uart_Error_Busy;
	}
	return Loc_enuErrorState;
}


/* SEND A BUFFER OF DATA USING ASYNCHRONOUS FUNCTION NOTE TO USE THIS FUNCTION ENABLE INTERRUPT*/
uart_error_t Uart_SendBufferSynchZCopy(Buffer_t* Data)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	u32 Loc_u32Reg = 0;
	if(u1.Uart_TxState == Uart_TxDONE)
	{
		if(Data->data != NULL)
		{
			Data->used = 0;
			u1.Uart_TxState = Uart_BUSY;
			/* SET THE MODE TO ENABLE TX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_TxONLY_EN;
			while(Data->used < Data->size)
			{
				/* SEND THE FIRST BUTE MANUALLY */
				(Loc_Uart_Reg)->USART_DR = Data->data[Data->used];
				/*WAIT UNTILL TC FLAG BE 1 TO INDCTATE THAT THE TX COMPELETE */
				while(GET_BIT( ((Loc_Uart_Reg)->USART_SR), TC_FLAG) == 0);
				Data->used++;
			}

		}
		else
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
	}
	else
	{
		/* INDICATED THAT UART STILL NOT FINISH THE LAST PROCESS */
		Loc_enuErrorState = Uart_Error_Busy;
	}
	return Loc_enuErrorState;
}



/* SEND A BYTE ASYNCHRONOUS FUNCTION */
uart_error_t Uart_SendByteAsynch(u8 Byte)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	u32 Loc_u32Reg = 0;
	if(u1.Uart_TxState == Uart_TxDONE)
	{
		/* HOLDE THE BYTE IN THE BUFFER VALUE */
		APP_TxBUF_Copy.data = &Byte;
		APP_TxBUF_Copy.size = 1;
		APP_TxBUF_Copy.used = 0;

		Uart_TxState = &(u1.Uart_TxState);
		u1.Uart_TxState = Uart_BUSY;

		/* SET THE MODE TO ENABLE TX */
		(Loc_Uart_Reg)->USART_CR1 |= UART_TxONLY_EN;

		/* SEND THE FIRST BUTE MANUALLY */
		(Loc_Uart_Reg)->USART_DR = APP_TxBUF_Copy.data[APP_TxBUF_Copy.used];
		APP_TxBUF_Copy.used++;

		/* CONFIGURE THE INTERRUPT ENABLE BITS */
		Loc_u32Reg = (Loc_Uart_Reg)->USART_CR1;
		Loc_u32Reg &= ~(CLR_INTERENTx_BITS);   //CLR THE BITS OF THE Interrupt enable SELECT
		Loc_u32Reg |= CLR_INTERENTx_BITS;     // ASSIGN THE NEW VALUE
		(Loc_Uart_Reg)->USART_CR1 = Loc_u32Reg;
	}
	else
	{
		/* INDICATED THAT UART STILL NOT FINISH THE LAST PROCESS */
		Loc_enuErrorState = Uart_Error_Busy;
	}

	return Loc_enuErrorState;
}

/* SEND A BYTE SYNCHRONOUS FUNCTION */
uart_error_t Uart_SendByteSynch(u8 Byte)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	if(u1.Uart_TxState == Uart_TxDONE)
	{
		/* CHANGE THE VALUE OF THE FLAG TO BE BUSY  TO AVOID ANY INTERRUPT SITUATION */
		u1.Uart_TxState = Uart_BUSY;
		/* SET THE MODE TO ENABLE TX */
		(Loc_Uart_Reg)->USART_CR1 |= UART_TxONLY_EN;
		/*WRITE THE BYTE INSIDE THE DR TO PREPARE TO TRANSMIT */
		(Loc_Uart_Reg)->USART_DR = Byte;
		/*WAIT UNTILL TC FLAG BE 1 TO INDCTATE THAT THE TX COMPELETE */
		while(GET_BIT( ((Loc_Uart_Reg)->USART_SR), TC_FLAG) == 0);
		/* CHANGE THE VALUE OF THE FLAG TO BE DONE */
		u1.Uart_TxState = Uart_TxDONE;
	}
	else
	{
		/* IF TX STATE NOT DONE THAT MEAN IT BUSY NOW SO RETURN AN ERROR */
		Loc_enuErrorState = Uart_Error_Busy;
	}

	return Loc_enuErrorState;

}


/* RECEIVE A BYTE ASYNCHRONOUS FUNCTION NOTE TO USE THIS FUNCTION ENABLE INTERRUPT */
uart_error_t Uart_ReceiveBuffAsynchZCopy (Buffer_t* Buffer_Receive)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	if(u1.Uart_RxState == Uart_RxDONE)
	{
		if(Buffer_Receive->data != NULL)
		{
			/*COPY THE SIZE THE USER WANT TO RECEIVE BUFFER TO OUR STATIC GOLBAL */
			APP_RxBUF_Copy.size = Buffer_Receive->size;
			/* COPY THE ADDRESS OF THE POSITION TO HOLD THE DATA ON IT */
			APP_RxBUF_Copy.data = Buffer_Receive->data;
			/* MAKE THE USED VARIABLE START FROM 0 */
			APP_RxBUF_Copy.used = 0;

			Uart_RxState = &(u1.Uart_RxState);
			u1.Uart_RxState = Uart_BUSY;

			/* SET THE MODE TO ENABLE RX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_RxONLY_EN;
			(Loc_Uart_Reg)->USART_SR = 0;
			(Loc_Uart_Reg)->USART_CR1 |= CLR_INTERENRx_BITS;     // ASSIGN THE NEW VALUE

		}
		else
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
	}
	else
	{
		/* IF RX STATE NOT DONE THAT MEAN IT BUSY NOW SO RETURN AN ERROR */
		Loc_enuErrorState = Uart_Error_Busy;
	}
	return Loc_enuErrorState;
}


/* RECEIVE A BYTE SYNCHRONOUS FUNCTION NOTE TO USE THIS FUNCTION ENABLE INTERRUPT */
uart_error_t Uart_ReceiveBufSynchZCopy (Buffer_t* Buffer_Receive)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	u16 Loc_u16Idx = 0;
	if(u1.Uart_RxState == Uart_RxDONE)
	{
		if(Buffer_Receive->data != NULL)
		{
			Buffer_Receive->used = 0;
			u1.Uart_RxState = Uart_BUSY;

			/* SET THE MODE TO ENABLE RX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_RxONLY_EN;
			while(Buffer_Receive->used < Buffer_Receive->size)
			{
				/* RECIEVE BYTES MANUALLY */
				/* CHECK THE STATE OF THE RXE IF IT 1 THEAT THE MEAN THAT THE DATA IN DR READY TO BE READ */
				while(((Loc_Uart_Reg)->USART_SR & RXNE_FLAG) == 0);
				((Loc_Uart_Reg)->USART_SR) &= ~RXNE_FLAG;
				/* GET THE DATA FROM DR */
				Buffer_Receive->data[Buffer_Receive->used]= ((Loc_Uart_Reg)->USART_DR);
				Buffer_Receive->used++;
			}

			u1.Uart_RxState = Uart_RxDONE;

		}
		else
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
	}
	else
	{
		/* IF RX STATE NOT DONE THAT MEAN IT BUSY NOW SO RETURN AN ERROR */
		Loc_enuErrorState = Uart_Error_Busy;
	}
	return Loc_enuErrorState;
}


/* RECEIVE A BYTE SYNCHRONOUS FUNCTION */
uart_error_t Uart_ReceiveSynch (pu8 Byte_Receive)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	if(u1.Uart_RxState == Uart_RxDONE)
	{
		if(Byte_Receive == NULL)
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
		else
		{
			/* CHANGE THE VALUE OF THE FLAG TO BE BUSY */
			u1.Uart_RxState = Uart_BUSY;
			/* SET THE MODE TO ENABLE RX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_RxONLY_EN;
			/* CHECK THE STATE OF THE RXE IF IT 1 THEAT THE MEAN THAT THE DATA IN DR READY TO BE READ */
			while(((Loc_Uart_Reg)->USART_SR & RXNE_FLAG) == 0);
			/* GET THE DATA FROM DR */
			*Byte_Receive = ((Loc_Uart_Reg)->USART_DR);
			/* CHANGE THE VALUE OF THE FLAG TO BE DONE */
			u1.Uart_RxState = Uart_RxDONE;
		}
	}
	else
	{
		/* IF RX STATE NOT DONE THAT MEAN IT BUSY NOW SO RETURN AN ERROR */
		Loc_enuErrorState = Uart_Error_Busy;
	}

	return Loc_enuErrorState;
}

//DMA1 ->  stream5 -> uart2_rx			stream6 -> uart2_tx		Channel 4

//DMA2 ->  stream2_5 -> uart1_rx			stream7 -> uart1_tx		Channel 4

//DMA2->   stream1_2 -> uart6_rx			stream6_7 -> uart6_tx		Channel 5

/* UART TX WITH DMA */
uart_error_t Uart_SendBufferZCopy_DMA(Buffer_t* Data)
{
	uart_error_t Loc_enuErrorState = Uart_Error_Ok;
	usart_reg_t * Loc_Uart_Reg = u1.Uart_Channel;
	Dma_cfg_t dma_cfg;
	if(u1.Uart_TxState == Uart_TxDONE)
	{
		if(Data->data != NULL)
		{
			/* SET THE MODE TO ENABLE TX */
			(Loc_Uart_Reg)->USART_CR1 |= UART_TxONLY_EN;
			/* SET UART ENABLE DMA TX */
			Loc_Uart_Reg->USART_CR3 |= DMA_TxENABLE;
			/* SET FOR UART 1 DMA IN DMA 2 STREAM AND CHANNEL OF TX */
			dma_cfg.Channel = DMA_Channel4;
			dma_cfg.Stream  = DMA_Stream7;
			dma_cfg.DMA_Num = DMA2;
			/* SELECT THE MODE */
			dma_cfg.Mode = DMA_NormalMode;
			/* SET MEMORY DATA SIZE AND PERPHERAL DATA SIZE AS 1 BYTE */
			dma_cfg.MemData_Size = DMA_MSIZE_Byte;
			dma_cfg.PerphData_Size = DMA_PSIZE_Byte;

			/* THE DIRECTION OF THE DATA IS FROM PERPH TO MEMORY */
			dma_cfg.Direction = DMA_MEM_to_PERPH;

			/* IN THIS CASE THE MEMORY ADDRESS WILL AUTO INCREAMENT */
			dma_cfg.MemAutoInc = DMA_MEM_AUTOINC_EN;

			/* THE PERPHRAL INCREAMENT DISABLE */
			dma_cfg.PerphAutoInc = DMA_AUTOINC_DISABLE;

			/* SET UAER_DR AS ADDRESS OF THE SOURCE IN DMA CONTROLLER */
			dma_cfg.SrcAdd = (u32)Data->data;

			/* WRITE THE MEMORY ADDRESS AS ADDRESS OF THE RECIEVER IN DMA CONTROLLER */
			dma_cfg.DisAdd = (u32)(&(Loc_Uart_Reg->USART_DR));

			/* CONFIGURE THE TOTAL NUMBER OF BYTES */
			dma_cfg.DataLength = Data->size;

			/* DMA CHANNEL PERIORITY HIGH */
			dma_cfg.Channel_Priority = DMA_Priority_HIGH;

			/* CONFIGURE THE DMA INTERRUPT DISABLE */
			dma_cfg.Interrupt_Enable = DMA_INTR_DISABLE;

			DMA_Init(&dma_cfg);

			/* CLEAR THE TC FLAG */
			Loc_Uart_Reg->USART_SR &= ~(0x40);

			/* ACTIVATE DMA CHANNEL */
			DMA_Start(&dma_cfg);
		}
		else
		{
			Loc_enuErrorState = Uart_NULLptr_Error;
		}
	}
	else
	{
		/* IF TX STATE NOT DONE THAT MEAN IT BUSY NOW SO RETURN AN ERROR */
		Loc_enuErrorState = Uart_Error_Busy;
	}
	return Loc_enuErrorState;
}
