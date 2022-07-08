//------------------------------------------------------------------------------
// Copyright 2020 DigSol Sweden AB
//------------------------------------------------------------------------------
//   The information in this document is the property of DigSol Sweden AB.
//   Except as specifically authorized in writing by DigSol Sweden AB
//   the receiver of this document shall keep the information contained
//   herein confidential and shall protect the same in whole or in part
//   from disclosure and dissemination to third parties.
//   Disclosure and dissemination to the receiver's employees shall
//   only be made on a strict need to know basis.
//------------------------------------------------------------------------------
#ifndef BOARD2_H
#define BOARD2_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <xc.h>


//------------------------------------------------------------------------------
// Common settings
//------------------------------------------------------------------------------
#define KICK_WD			1		        //xxx 0 or 1


//------------------------------------------------------------------------------
// ds30 Loader settings
//------------------------------------------------------------------------------
#define	HELLO_TIMEOUT	3000			//xxx
#define COMM_TIMEOUT	3000 			//xxx
#define HELLOTRIES		7				//xxx


//------------------------------------------------------------------------------
// ds30 SD Card Loader settings
//------------------------------------------------------------------------------
#define LOG_TO_UART         1		        //xxx 0 or 1
#define LOG_TO_FILE         1		        //xxx 0 or 1
#define VERBOSITY           3		        //xxx 0=silent, 1=errors, 2=+basic operation info, 3=+everything
#define LOG_FILE_NAME       "ds30_log.txt"  //8.3 filename
#define IS_DS30_SD_CARD_LOADER
                

//------------------------------------------------------------------------------
// Clocks
//------------------------------------------------------------------------------
#define SYSCLK  		8000000L		//xxx
#define PBCLK			40000000L		//xxx


//------------------------------------------------------------------------------
// UART
//------------------------------------------------------------------------------
#define USE_UART1						//xxx
#define	BAUDRATE		9600			//xxx
#define USE_BRGH		0				//xxx 0 or 1

#define	USE_TXENABLE	0				//xxx 0 or 1
#define	TXE_DELAY 		10				//xxx time in us to wait before transmitting after pulling the tx enable pin high
#define	TXE_TRIS		TRISCbits.TRISC3//xxx tris register containing tx enable
#define	TXE_LAT			LATCbits.LATC3	//xxx port register containing tx enable

#define USE_RXENABLE    0
#define RXE_TRIS        TRISFbits.TRISF0//xxx
#define RXE_LAT         LATFbits.LATF0	//xxx


//------------------------------------------------------------------------------
// SPI
//------------------------------------------------------------------------------
//#define USE_SW_SPI
#define USE_SPI6
#define SPI_TARGET_SPEED_FAST   4000000


//------------------------------------------------------------------------------
// SD lib settings
//------------------------------------------------------------------------------

// Write protect signal
#define USE_SD_WP           0                       //xxx 0 or 1
#if USE_SD_WP == 0
    #define SD_WP           1                       //1=writing log file disabled, 0=writing log file enabled if LOG_TO_FILE i set to 1
#else
    #define SD_WP_TRIS      TRISBbits.TRISB0
    #define SD_WP           PORTBbits.RB0
#endif

// Card detect signal
#define USE_SD_CD           0                       //xxx 0 or 1
#if USE_SD_CD == 0
    #define SD_CD           0                       //always 0
#else
    #define SD_CD_TRIS      TRISBbits.TRISB0        //xxx
    #define SD_CD           PORTBbits.RB0           //xxx
#endif

// Chip select signal
#define SD_CS_TRIS          TRISBbits.TRISB0        //xxx
#define SD_CS               LATBbits.LATB0          //xxx

// SPI pins
#define SCK_TRIS            TRISBbits.TRISB0        //xxx
#define SCK_LAT             LATBbits.LATB0          //xxx

#define SDI_TRIS            TRISBbits.TRISB0        //xxx
#define SDI_PORT            PORTBbits.RB0           //xxx

#define SDO_TRIS            TRISBbits.TRISB0        //xxx
#define SDO_LAT             LATBbits.LATB0          //xxx


//------------------------------------------------------------------------------
// USB
//------------------------------------------------------------------------------
#define	USE_USB
#define	_USB


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//BOARD2_H
