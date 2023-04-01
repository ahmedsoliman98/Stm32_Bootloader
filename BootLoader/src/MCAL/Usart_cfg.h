/*
 * Uart_cfg.h
 *
 *  Created on: Apr 12, 2022
 *      Author: Ahmed Soliman
 */

#ifndef MCAL_UART_CFG_H_
#define MCAL_UART_CFG_H_


/* BUADRATE FUNCTION
 *
 * BR =   (FCLK)  / ( 8 * (2 - 0) * (USARTDIV) )
 *
 * DIV_Mantissa[11:4]: mantissa of USARTDIV
 * These 12 bits define the mantissa of the USART Divider (USARTDIV
 *
 * DIV_Fraction[3:0]: fraction of USARTDIV
 * These 4 bits define the fraction of the USART Divider (USARTDIV). When OVER8=1, the
 * DIV_Fraction3 bit is not considered and must be kept cleared.
 *
 * EX BR = 9600   FCLK = 16Mhz  ->   USARTDIV = 104.16667
 *  DIV_Fraction = 0.16667 * 16 = 2.6 ~ 3 The nearest real number
 *  DIV_Mantissa = 104 -> 0x68
 *  USART_BRR =0x683
 *
 */

#define DIV_Mantissa        0x68
#define DIV_Fraction        0x03

/* THE VALUE HOLD IN THE USART_BRR */
#define BUADRATE_VALUE     ((DIV_Mantissa << 4)|DIV_Fraction)

/*********************************************************************************************************/

/* INTERRUPT CONFIG */
#define Uart_INTR_EN      (UART_TCINTER_EN  | UART_TXEINTR_EN | UART_TXEINTR_EN)


/* NUMBER OF STOP BIT */
#define Uart_StopBit      (UART_1STOP_BIT)

#endif /* MCAL_USART_CFG_H_ */
