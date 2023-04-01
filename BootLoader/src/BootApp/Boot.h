/*
 * Boot.h
 *
 *  Created on: May 13, 2022
 *      Author: Ahmed Soliman
 */

#ifndef BOOTAPP_BOOT_H_
#define BOOTAPP_BOOT_H_

#define PROGRAMMING_REQ        0x10
#define FIRST_FRAME_START      0x34
#define DATA_FRAME             0x36
#define COMMUNICATION_DONE     0x37
#define NEGATIVE_RESPONCE      0xF7
#define POSTIVE_VALUE          0x40
#define NO_APP_VALUE           0xFFFFFFFFL
#define APP_INFO_SECOTR        (0x08004000L)

/*FLASHING SEQUENCE
 *
 * 1) SESSION CONTROL (RESEIVED)
 * 		0X10 -> INDICATE THAT THAT THERE WAS ASKING FOR PROGRAMMING
 * 2) POSITIVE RESPONCE ( SEND  )
 * 		ALWAYS BE THE LAST RESEIVED COMMAND BYTE + 0X40
 *
 * 3) DOWENLOAD FUNCTION UNIT (SOFWARE FRAM CONTAING A MULTIBLE OF UART FRAME )
 * 		A) 0X34 + ENTRYPOINT_ADDRESS + TOTAL SIZE OF THE BUTE SHOULD RECIEVED
 * 			POSITIVE RESPONCE
 * 		B) 0x36 + DATA
 * 			AFTER GET ALL THE DATA SEND A POSITIVE RESPONCE
 * 		C) TRANSFER DONE 0X37
 * 			RESPONCE FLAG
 *
 * 	4) START FLASHING USING A FLASH DRIVER
 *
 * 	5) JUMP TO APPLICTION
 *
 * 	make sure to store the data in app_info_t struct
 *
 *
 */
typedef void (* func_t)(void);

typedef struct{
	u8 Cmd;      //HOLD THE COMMAND IN THE FRAME
	u8 Res;      // RESERVED BIT
	u16 len;     // LENGTH OF THE NUMBER OF THE DATA BYTE WILL RESEIVE
	u8 Data[1];  // DATA
}FrameForm_Cmd;


typedef struct{
	u8 Cmd;      //HOLD THE COMMAND IN THE FRAME
	u8 Data[1];  // DATA
}FrameForm_Data;


/* DATA TYPES DEFINE THE APPLICTION INFORMATION */
typedef struct {
	u32 size ;              // HOLD THE TOTAL NUMBER OF THE APPLICTION BYTES
	u32 App_EntryPoint; // HOLD THE APPLICATION ENTRY POINT
}app_info_t;





/* DUMMY ENUM TO RETURN VALUE OF THE FUNCTION CAUESE IF NEED A RETURN IN FUTURE */
typedef enum{
	FALSE,
	TRUE,
}re_t;

/*
 * INTIALLIZE UART AND ENABLE ITS CLOCK
 */
re_t Boot_FlashingInit(void);

/*
 * FLASHING STATES
 */
re_t Boot_FlashingTask(void);






#endif /* BOOTAPP_BOOT_H_ */
