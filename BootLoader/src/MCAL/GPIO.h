/*
 * GPIO.h
 *
 *  Created on: Mar 19, 2022
 *      Author: Ahmed Soliman
 */

#ifndef GPIO_H_
#define GPIO_H_


/*Port */


#define GPIOA_BASE_ADDRESS        0x40020000
#define GPIOB_BASE_ADDRESS        0x40020400
#define GPIOC_BASE_ADDRESS        0x40020800
#define GPIOD_BASE_ADDRESS        0x40020C00
#define GPIOE_BASE_ADDRESS        0x40021000
#define GPIOH_BASE_ADDRESS        0x40021C00

#define GPIOA     ((void * const)(GPIOA_BASE_ADDRESS))
#define GPIOB     ((void * const)(GPIOB_BASE_ADDRESS))
#define GPIOC     ((void * const)(GPIOC_BASE_ADDRESS))
#define GPIOD     ((void * const)(GPIOD_BASE_ADDRESS))
#define GPIOE     ((void * const)(GPIOE_BASE_ADDRESS))
#define GPIOH     ((void * const)(GPIOH_BASE_ADDRESS))






/* Speed */

#define Gpio_u16SpeedCfg_LOW      (u16)(0x0100)
#define Gpio_u16SpeedCfg_MEDIUM   (u16)(0x0101)
#define Gpio_u16SpeedCfg_HIGH     (u16)(0x0102)
#define Gpio_u16SpeedCfg_VHIGH    (u16)(0x0103)


/* Pin Mode */
#define Gpio_u8PinMode_PP           (u8)(0b00100000)
#define Gpio_u8PinMode_PP_PU        (u8)(0b00100001)
#define Gpio_u8PinMode_PP_PD        (u8)(0b00100010)

#define Gpio_u8PinMode_OD           (u8)(0b00101000)
#define Gpio_u8PinMode_OD_PU        (u8)(0b00101001)
#define Gpio_u8PinMode_OD_PD        (u8)(0b00101010)

#define Gpio_u8PinMode_AFPP         (u8)(0b01000000)
#define Gpio_u8PinMode_AFPP_PU      (u8)(0b01000001)
#define Gpio_u8PinMode_AFPP_PD      (u8)(0b01000010)

#define Gpio_u8PinMode_AFOD         (u8)(0b01001000)
#define Gpio_u8PinMode_AFOD_PU      (u8)(0b01001001)
#define Gpio_u8PinMode_AFOD_PD      (u8)(0b01001010)

#define Gpio_u8PinMode_INFL         (u8)(0b00000000)
#define Gpio_u8PinMode_INPU         (u8)(0b00000001)
#define Gpio_u8PinMode_INPD         (u8)(0b00000010)

#define Gpio_PinMdeo_Analog       (u8)(0b01100000)

/* Pin Number */
#define Gpio_u64PinNumber_PIN0        (0x200000001)
#define Gpio_u64PinNumber_PIN1        (0x200000002)
#define Gpio_u64PinNumber_PIN2        (0x200000004)
#define Gpio_u64PinNumber_PIN3        (0x200000008)
#define Gpio_u64PinNumber_PIN4        (0x200000010)
#define Gpio_u64PinNumber_PIN5        (0x200000020)
#define Gpio_u64PinNumber_PIN6        (0x200000040)
#define Gpio_u64PinNumber_PIN7        (0x200000080)
#define Gpio_u64PinNumber_PIN8        (0x200000100)
#define Gpio_u64PinNumber_PIN9        (0x200000200)
#define Gpio_u64PinNumber_PIN10       (0x200000400)
#define Gpio_u64PinNumber_PIN11       (0x200000800)
#define Gpio_u64PinNumber_PIN12       (0x200001000)
#define Gpio_u64PinNumber_PIN13       (0x200002000)
#define Gpio_u64PinNumber_PIN14       (0x200004000)
#define Gpio_u64PinNumber_PIN15       (0x200008000)

/* Pin State */

#define Gpio_u8PIN_HIGH                       1
#define Gpio_u8PIN_LOW                        0


#define Gpio_u64SET_PIN0HIGH                    0x400000001
#define Gpio_u64SET_PIN1HIGH                    0x400000002
#define Gpio_u64SET_PIN2HIGH                    0x400000004
#define Gpio_u64SET_PIN3HIGH                    0x400000008
#define Gpio_u64SET_PIN4HIGH                    0x400000010
#define Gpio_u64SET_PIN5HIGH                    0x400000020
#define Gpio_u64SET_PIN6HIGH                    0x400000040
#define Gpio_u64SET_PIN7HIGH                    0x400000080
#define Gpio_u64SET_PIN8HIGH                    0x400000100
#define Gpio_u64SET_PIN9HIGH                    0x400000200
#define Gpio_u64SET_PIN10HIGH                   0x400000400
#define Gpio_u64SET_PIN11HIGH                   0x400000800
#define Gpio_u64SET_PIN12HIGH                   0x400001000
#define Gpio_u64SET_PIN13HIGH                   0x400002000
#define Gpio_u64SET_PIN14HIGH                   0x400004000
#define Gpio_u64SET_PIN15HIGH                   0x400008000


#define Gpio_u64SET_PIN0LOW                    0x400010000
#define Gpio_u64SET_PIN1LOW                    0x400020000
#define Gpio_u64SET_PIN2LOW                    0x400040000
#define Gpio_u64SET_PIN3LOW                    0x400080000
#define Gpio_u64SET_PIN4LOW                    0x400100000
#define Gpio_u64SET_PIN5LOW                    0x400200000
#define Gpio_u64SET_PIN6LOW                    0x400400000
#define Gpio_u64SET_PIN7LOW                    0x400800000
#define Gpio_u64SET_PIN8LOW                    0x401000000
#define Gpio_u64SET_PIN9LOW                    0x402000000
#define Gpio_u64SET_PIN10LOW                   0x404000000
#define Gpio_u64SET_PIN11LOW                   0x408000000
#define Gpio_u64SET_PIN12LOW                   0x410000000
#define Gpio_u64SET_PIN13LOW                   0x420000000
#define Gpio_u64SET_PIN14LOW                   0x440000000
#define Gpio_u64SET_PIN15LOW                   0x480000000

/* ALTERNATIVE FUNCTION MASK */
#define Gpio_u16AF0_SYS                        (0x0500)
#define Gpio_u16AF1_TIM12                      (0x0501)
#define Gpio_u16AF2_TIM35                      (0x0502)
#define Gpio_u16AF3_TIM911                     (0x0503)
#define Gpio_u16AF4_I2C12                      (0x0504)
#define Gpio_u16AF5_SPI14                      (0x0505)
#define Gpio_u16AF6_SPI3                       (0x0506)
#define Gpio_u16AF7_UART12                     (0x0507)
#define Gpio_u16AF8_UART6                      (0x0508)
#define Gpio_u16AF9_I2C23                      (0x0509)
#define Gpio_u16AF10_OTGFS                     (0x050A)
#define Gpio_u16AF11                           (0x050B)
#define Gpio_u16AF12_SDIO                      (0x050C)
#define Gpio_u16AF13_SYS                       (0x050D)
#define Gpio_u16AF14_SYS                       (0x050E)
#define Gpio_u16AF15_EVENTOUT                  (0x050F)


typedef struct{
	u8 Gpio_PinMode;
	u16 Gpio_PinSpeed;
	u64 Gpio_PinNumber;
	void* Gpio_PinPort;
}Gpio_PinCfg_t;



/* Error State */
typedef enum
{
	Gpio_enuOK,
	Gpio_enuPinCfgMode_Error,
	Gpio_enuPinCfgSpeed_Error,
	Gpio_enuPinCfgPinNumber_Error,
	Gpio_enuAFNumber_Error,
	Gpio_enuPinCfgPinPort_Error,
	Gpio_enuSetPin_State_Error,
	Gpio_enuNUL_Ptr_Error,
}Gpio_tenuErrorStatus;

Gpio_tenuErrorStatus Gpio_initPin(Gpio_PinCfg_t * Add_Gpio_Pincfg_GpioPin);
Gpio_tenuErrorStatus Gpio_SetPinValue(Gpio_PinCfg_t * Add_Gpio_Pincfg,u64 Copy_u64PinState);
Gpio_tenuErrorStatus Gpio_WritePinValue(void* Copy_u16PinPort,u64 Copy_u16PinNum,u8 Copy_u64PinState);
Gpio_tenuErrorStatus Gpio_ReadPinValue(void* Copy_vidPort,u64 Copy_u64PinNumber,pu8 Add_u8PinState);
Gpio_tenuErrorStatus Gpio_AlternativeFunctionPin(u64 Copy_u64PinNum ,u16 Copy_u16AF_Num,void * PORT);
//Gpio_tenuErrorStatus Gpio_GetPinValue(Gpio_PinCfg_t * Add_Gpio_Pincfg,pu8 Add_u8PinState);


#endif /* GPIO_H_ */
