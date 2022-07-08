//------------------------------------------------------------------------------
// Copyright 2018 DigSol Sweden AB
//------------------------------------------------------------------------------
//   The information in this document is the property of DigSol Sweden AB.
//   Except as specifically authorized in writing by DigSol Sweden AB
//   the receiver of this document shall keep the information contained
//   herein confidential and shall protect the same in whole or in part
//   from disclosure and dissemination to third parties.
//   Disclosure and dissemination to the receiver's employees shall
//   only be made on a strict need to know basis.
//------------------------------------------------------------------------------
#ifndef DS30_COMMON_H
#define DS30_COMMON_H


//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#if defined(__XC8)
	#if defined (_PIC12) || defined(_PIC12E) || defined(_PIC12IE)
		#define IS_12F
	#elif defined(_PIC14) || defined(_PIC14E) || defined(_PIC14EX)
		#define IS_16F
	#elif defined(_PIC18)
		#define IS_18F
    #else
        #error ""
	#endif
#endif


//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define MIN(x,y)		( (x) < (y) ? (x) : (y) )

#if KICK_WD == 0
	#define CLEAR_WDT()
#else
	#if defined(__XC8)
		#define CLEAR_WDT()  CLRWDT()

	#elif defined(__XC16)
		#define CLEAR_WDT()  asm("clrwdt")

	#elif defined(__PIC32MX)
		#define CLEAR_WDT()  WDTCONSET=0x01
		
	#elif defined(__PIC32MZ) ||  defined(__PIC32MM)
		#define CLEAR_WDT(x) do { volatile uint16_t* wdtKey = ( (volatile uint16_t *)&WDTCON ) + 1; *wdtKey = 0x5743; } while (0)
	#else
		#error "not implemented"
	#endif
#endif

#if defined(__XC8)
    #define sw_reset()  asm( "reset" )

#elif defined(__XC16)
    #define sw_reset()  asm( "reset" )

#elif defined(__PIC32MX)
    #define sw_reset()  soft_reset()

#elif defined(__PIC32MZ) ||  defined(__PIC32MM)
    #define sw_reset()	soft_reset()
#else
    #error "not implemented"
#endif


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//DS30_COMMON_H 
