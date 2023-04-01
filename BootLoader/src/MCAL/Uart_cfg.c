/*
 * Uart_cfg.c
 *
 *  Created on: Apr 12, 2022
 *      Author: Ahmed Soliman
 */



#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "Usart.h"
#include "Usart_cfg.h"



uart_config_t u1 = {
		.Uart_Channel = USART1,
		.Uart_BUADRATE = BUADRATE_VALUE,
		.Uart_CommMode = UART_TxRxBOTH_EN,
		.Uart_NumStopB = Uart_StopBit,
		.Uart_INTREN = Uart_INTR_EN,
};

