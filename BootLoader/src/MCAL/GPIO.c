/*
 * GPIO.c
 *
 *  Created on: Mar 19, 2022
 *      Author: Ahmed Soliman
 */

#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "GPIO.h"





#define MAX_PIN_NUM               (u8)16


#define CHCK_MODE_VALUE           (u8)0x00
#define CHCK_SPEED_VALUE          (u8)1
#define CHCK_PINNUMBER_VALUE      (u8)2
#define CHCK_PORT_VALUE           (u8)4
#define CHCK_PINSTATE_VALUE       (u8)4
#define CHCK_u16SHIFT_VALUE       (u8)8
#define CHCK_u64SHIFT_VALUE       (u8)32
#define CHCK_MODESHIFT_VALUE      (u8)128
#define CHCK_PORTSHIFT_VALUE      (u8)28
#define CHCK_u8WRITE_VALUE        (u8)5
#define CHCK_AF_NUMBER            (u8)5



/* Mask To Get Dir/Type/ PUPD from mask */

#define GET_PUORPD     (u8)0x03
#define GET_TYPE       (u8)0x08
#define GET_MODE       (u8)0x60
#define CLR_MSK_2bit   (u8)0x03
#define CLR_MSK_1bit   (u8)0x01
#define CLR_MAS_4BIT   (u8)0x0F
#define RST_MASK_BSRR  (u8)16



typedef struct{
	volatile u32 GPIOx_MODER;
	volatile u32 GPIOx_OTYPER;
	volatile u32 GPIOx_OSPEEDR;
	volatile u32 GPIOx_PUPDER;
	volatile u32 GPIOx_IDR;
	volatile u32 GPIOx_ODR;
	volatile u32 GPIOx_BSRR;
	volatile u32 GPIOx_LCKR;
	volatile u32 GPIOx_AFRL;
	volatile u32 GPIOx_AFRH;
}Gpio_reg;






Gpio_tenuErrorStatus Gpio_initPin(Gpio_PinCfg_t * Add_Gpio_Pincfg_GpioPin)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	u8 Loc_u8idx = 0;
	Gpio_reg *Loc_GpioReg = Add_Gpio_Pincfg_GpioPin->Gpio_PinPort;
	u32 Loc_u32TempReg = 0x00;
	if ( ( Add_Gpio_Pincfg_GpioPin->Gpio_PinMode & CHCK_MODESHIFT_VALUE ) != CHCK_MODE_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgMode_Error;
	}
	else if ((Add_Gpio_Pincfg_GpioPin->Gpio_PinSpeed >> CHCK_u16SHIFT_VALUE) != CHCK_SPEED_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgSpeed_Error;
	}
	else if ((Add_Gpio_Pincfg_GpioPin->Gpio_PinNumber >> CHCK_u64SHIFT_VALUE) != CHCK_PINNUMBER_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgPinNumber_Error;
	}
	else
	{
		Loc_u32TempReg = ((u32)(Add_Gpio_Pincfg_GpioPin->Gpio_PinNumber));
		for(Loc_u8idx = 0; Loc_u8idx < MAX_PIN_NUM; Loc_u8idx++)
		{
			if((Loc_u32TempReg & (0x01<<Loc_u8idx)))
			{
				Loc_u32TempReg = (Loc_GpioReg)->GPIOx_MODER;
				Loc_u32TempReg &= ~(CLR_MSK_2bit << (Loc_u8idx*2));
				Loc_u32TempReg |= (((((u8)Add_Gpio_Pincfg_GpioPin->Gpio_PinMode)&GET_MODE)>>5) << (Loc_u8idx *2));
				(Loc_GpioReg)->GPIOx_MODER = Loc_u32TempReg;

				Loc_u32TempReg = ((Loc_GpioReg))->GPIOx_OTYPER;
				Loc_u32TempReg &= ~(CLR_MSK_1bit << Loc_u8idx);
				Loc_u32TempReg |= (((((u8)Add_Gpio_Pincfg_GpioPin->Gpio_PinMode)&GET_TYPE)>>3) << Loc_u8idx);
				((Loc_GpioReg))->GPIOx_OTYPER = Loc_u32TempReg;

				Loc_u32TempReg = ((Loc_GpioReg))->GPIOx_PUPDER;
				Loc_u32TempReg &= ~(CLR_MSK_2bit << (Loc_u8idx*2));
				Loc_u32TempReg |= (((((u8)Add_Gpio_Pincfg_GpioPin->Gpio_PinMode)&GET_PUORPD)) << (Loc_u8idx*2));
				((Loc_GpioReg))->GPIOx_PUPDER = Loc_u32TempReg;


				((Loc_GpioReg))->GPIOx_OSPEEDR = (((u8)Add_Gpio_Pincfg_GpioPin->Gpio_PinSpeed) << (Loc_u8idx*2));

			}
			else
			{
				// do Nothing
			}
		}

	}
	return Loc_enuErrorState;

}


Gpio_tenuErrorStatus Gpio_SetPinValue(Gpio_PinCfg_t * Add_Gpio_Pincfg,u64 Copy_u64PinState)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	Gpio_reg *Loc_GpioReg = Add_Gpio_Pincfg->Gpio_PinPort;
	if((Copy_u64PinState >> CHCK_u64SHIFT_VALUE) != CHCK_PINSTATE_VALUE)
	{
		Loc_enuErrorState = Gpio_enuSetPin_State_Error;
	}
	else if ( ( Add_Gpio_Pincfg->Gpio_PinMode & CHCK_MODESHIFT_VALUE ) != CHCK_MODE_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgMode_Error;
	}
	else if ((Add_Gpio_Pincfg->Gpio_PinSpeed >> CHCK_u16SHIFT_VALUE) != CHCK_SPEED_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgSpeed_Error;
	}
	else
	{
		(Loc_GpioReg)->GPIOx_BSRR = (u32)Copy_u64PinState;
	}
	return Gpio_enuPinCfgPinPort_Error;
}



Gpio_tenuErrorStatus Gpio_WritePinValue(void* Copy_u16PinPort,u64 Copy_u16PinNum,u8 Copy_u64PinState)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	Gpio_reg *Loc_GpioReg = Copy_u16PinPort;
	if((Copy_u16PinNum >> CHCK_u64SHIFT_VALUE) != CHCK_PINNUMBER_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgPinNumber_Error;
	}
	else
	{
		if(Copy_u64PinState == Gpio_u8PIN_HIGH)
		{
			(Loc_GpioReg)->GPIOx_BSRR = (u32)Copy_u16PinNum;
		}
		else if (Copy_u64PinState == Gpio_u8PIN_LOW)
		{
			(Loc_GpioReg)->GPIOx_BSRR = ((u32)Copy_u16PinNum << RST_MASK_BSRR);
		}

	}

	return Loc_enuErrorState;
}


Gpio_tenuErrorStatus Gpio_GetPinValue(Gpio_PinCfg_t * Add_Gpio_Pincfg,pu8 Add_u8PinState)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	Gpio_reg *Loc_GpioReg = Add_Gpio_Pincfg->Gpio_PinPort;
	if ( ( Add_Gpio_Pincfg->Gpio_PinMode & CHCK_MODESHIFT_VALUE ) != CHCK_MODE_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgMode_Error;
	}
	else if ((Add_Gpio_Pincfg->Gpio_PinSpeed >> CHCK_u16SHIFT_VALUE) != CHCK_SPEED_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgSpeed_Error;
	}
	else if ((Add_Gpio_Pincfg->Gpio_PinNumber >> CHCK_u64SHIFT_VALUE) != CHCK_PINNUMBER_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgPinNumber_Error;
	}
	else if(Add_u8PinState == NULL)
	{
		Loc_enuErrorState = Gpio_enuNUL_Ptr_Error;
	}
	else
	{
		if(Loc_GpioReg->GPIOx_IDR & (u32)(Add_Gpio_Pincfg->Gpio_PinNumber))
		{
			*Add_u8PinState = Gpio_u8PIN_HIGH;
		}
		else
		{
			*Add_u8PinState = Gpio_u8PIN_LOW;
		}
	}

	return Loc_enuErrorState;
}



Gpio_tenuErrorStatus Gpio_ReadPinValue(void* Copy_vidPort,u64 Copy_u64PinNumber,pu8 Add_u8PinState)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	Gpio_reg *Loc_GpioReg = Copy_vidPort;
	if (Copy_u64PinNumber != CHCK_PINNUMBER_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgPinNumber_Error;
	}
	else
	{
		if(Loc_GpioReg->GPIOx_IDR & (u32)(Copy_u64PinNumber))
		{
			*Add_u8PinState = Gpio_u8PIN_HIGH;
		}
		else
		{
			*Add_u8PinState = Gpio_u8PIN_LOW;
		}
	}

	return Loc_enuErrorState;
}




Gpio_tenuErrorStatus Gpio_AlternativeFunctionPin(u64 Copy_u64PinNum ,u16 Copy_u16AF_Num, void * Copy_vidPORT)
{
	Gpio_tenuErrorStatus Loc_enuErrorState = Gpio_enuOK;
	u8 Loc_u8idx = 0;
	u32 Loc_u32TempReg = 0x00;
	Gpio_reg *Loc_GpioReg = Copy_vidPORT;
	if ((Copy_u64PinNum >> CHCK_u64SHIFT_VALUE) != CHCK_PINNUMBER_VALUE)
	{
		Loc_enuErrorState = Gpio_enuPinCfgPinNumber_Error;
	}
	else if((Copy_u16AF_Num >> CHCK_u16SHIFT_VALUE) != CHCK_AF_NUMBER)
	{
		Loc_enuErrorState = Gpio_enuAFNumber_Error;
	}
	else
	{
		Loc_u32TempReg = ((u32)(Copy_u64PinNum));
		for(Loc_u8idx = 0; Loc_u8idx < MAX_PIN_NUM; Loc_u8idx++)
		{
			if((Loc_u32TempReg & (0x01<<Loc_u8idx)))
			{
				if(Loc_u8idx <= 7)
				{
					Loc_u32TempReg = Loc_GpioReg->GPIOx_AFRL;
					Loc_u32TempReg &= ~(CLR_MAS_4BIT<<(4*Loc_u8idx));
					Loc_u32TempReg |= (((u8)Copy_u16AF_Num) << (4*Loc_u8idx));;
					Loc_GpioReg->GPIOx_AFRL = Loc_u32TempReg;
				}
				else
				{
					Loc_u32TempReg = Loc_GpioReg->GPIOx_AFRH;
					Loc_u32TempReg &= ~(CLR_MAS_4BIT<<(4*(Loc_u8idx-8)));
					Loc_u32TempReg |= (((u8)Copy_u16AF_Num) << (4*Loc_u8idx));;
					Loc_GpioReg->GPIOx_AFRH = Loc_u32TempReg;
				}

			}

		}
	}

	return Loc_enuErrorState;
}




