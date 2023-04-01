/*
 * Uart.h
 *
 *  Created on: Apr 12, 2022
 *      Author: Ahmed Soliman
 */

#ifndef MCAL_UART_H_
#define MCAL_UART_H_





/*UART CHANNEL IN STM32F401CC */
#define USART6       ((void *const)(0x40011400))
#define USART1       ((void *const)(0x40011000))
#define USART2       ((void *const)(0x40004400))

/* COMMUNICATION MODE OPTION */
#define UART_TxONLY_EN          (0x0008)
#define UART_RxONLY_EN          (0x0004)
#define UART_TxRxBOTH_EN        (0x000C)

/* INTERRUPT ENABLE OPTION */
#define UART_RXINTR_EN          (0x0020)
#define UART_TCINTER_EN         (0x0040)
#define UART_TXEINTR_EN         (0x0080)

/* NUMBER OF STOP BITS */
#define UART_1STOP_BIT         (0x0000)
#define UART_2STOP_BIT         (0x2000)

/* UART ENABLE */
#define UART_ENABLE             (0x2000)

typedef enum{
	Uart_TxDONE,
	Uart_RxDONE,
	Uart_BUSY,
}Uart_state_t;

/* UART CONFIG STRUCT TO DECIDE THE UART CHANNEL COMMUNICATION MODE  ENABLE TX OR RX OR BOTH */
typedef struct{
	void * Uart_Channel;     //DECIDE THE NUMBER OF UART CHANNEL
	u16 Uart_BUADRATE;      //DECIDE THE FINAL VALUD WILL LOAD IN THE USART_BRR
	u16 Uart_CommMode;      //DECIDE THE COMMUNICATION ENABLE TX_ONLY OR RX_ONLY OR BOTH
	u16 Uart_NumStopB;      //DECIDE THE NUMBER OF STOP BIT
	u16 Uart_INTREN;        // DECIDE WHICH INTERRUPT ENABLE NOTE IF USE ASYNCH FUNCTION MUST ENABLE ITS INTERRUPT BIT
	Uart_state_t Uart_RxState;  /*  ENUM UART Rx STATE INDICATION */
	Uart_state_t Uart_TxState;  /*  ENUM UART Tx STATE INDICATION */
}uart_config_t;


/* ENUM TO CHECK ERROR AND THE STATE OF THE COMMUNICATION */
typedef enum{
	Uart_Error_Ok,
	Uart_NULLptr_Error,
	Uart_Error_Busy,
}uart_error_t;

/* UART INIT FUUNCTION */
uart_error_t Uart_Init(void);

/* SEND A BUFFER OF DATA USING ASYNCHRONOUS FUNCTION */
uart_error_t Uart_SendBufferAsychZCopy(Buffer_t* Data);

/* SEND A BUFFER OF DATA USING ASYNCHRONOUS FUNCTION */
uart_error_t Uart_SendBufferSynchZCopy(Buffer_t* Data);

/* SEND A BYTE ASYNCHRONOUS FUNCTION */
uart_error_t Uart_SendByteAsynch(u8 Byte_Send);

/* SEND A BYTE SYNCHRONOUS FUNCTION */
uart_error_t Uart_SendByteSynch(u8 Byte);

/* RECEIVE A BYTE ASYNCHRONOUS FUNCTION */
uart_error_t Uart_ReceiveBuffAsynchZCopy (Buffer_t* Buffer_Receive);

/* RECEIVE A BYTE SYNCHRONOUS FUNCTION */
uart_error_t Uart_ReceiveSynch (pu8 Byte_Receive);

/* RECEIVE A BUFFER SYNCHRONOUS FUNCTION */
uart_error_t Uart_ReceiveBufSynchZCopy (Buffer_t* Buffer_Receive);



#endif /* MCAL_USART_H_ */
