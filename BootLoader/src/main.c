/*
 * BOOTLOADER FOR STM32-M4
 *
 *  make sure that the mem.ld file is edit the Flash memory
 *  ORIGIN : 0x08000000
 *  SIZE   : 16k the size of the first sector
 *  Created on: Apr 24, 2022
 *      Author: Ahmed Soliman
 */

#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "MCAL/Usart.h"
#include "BootApp/Boot.h"


func_t  AppFun ;


int
main(int argc, char* argv[])
{
	u8 data = 0;
	AppFun =  (func_t)(((app_info_t*)(APP_INFO_SECOTR))->App_EntryPoint);
	if(((app_info_t*)(APP_INFO_SECOTR))->App_EntryPoint == NO_APP_VALUE)
	{
		Boot_FlashingInit();

		while (1)
		{
			if(data == PROGRAMMING_REQ)
			{
				Boot_FlashingTask();

			}
			else
			{
				Uart_ReceiveSynch(&data);
			}
		}
	}
	else
	{
		asm("LDR R5, =0x20010000");
		asm("mov SP,R5");
		AppFun();
	}

	return 0;
}

