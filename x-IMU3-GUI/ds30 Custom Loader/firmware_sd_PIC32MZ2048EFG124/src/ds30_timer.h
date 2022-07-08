//------------------------------------------------------------------------------
// Copyright 2011-2018 DigSol Sweden AB
//------------------------------------------------------------------------------
//   The information in this document is the property of DigSol Sweden AB.
//   Except as specifically authorized in writing by DigSol Sweden AB
//   the receiver of this document shall keep the information contained
//   herein confidential and shall protect the same in whole or in part
//   from disclosure and dissemination to third parties.
//   Disclosure and dissemination to the receiver's employees shall
//   only be made on a strict need to know basis.
//------------------------------------------------------------------------------
#ifndef DS30_TIMER_H
#define DS30_TIMER_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"
#if defined(__XC16)
	#include <libpic30.h>   //__delay_us()
#endif


#define NOT_CORE_TIMER


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
#if defined(__XC32)
	typedef uint32_t	time_t;
#else
	typedef uint16_t	time_t;
#endif


//------------------------------------------------------------------------------
// Public function prototypes & macros
//------------------------------------------------------------------------------
void    timer_start_ms( const time_t time );
	
#if defined(__XC8) 
    uint8_t timer_timeout(void);
	#define delay_us(x) __delay_us(x)
    
    #if defined(IS_12F) || defined(IS_16F)
        #define timer_exit();	{T1CON = 0x00; TMR1IF=0;}
    #endif

#elif defined(__XC16)
    uint8_t timer_timeout(void);
	#define delay_us(x) 	__delay_us(x)

#elif defined(__XC32)
    void    timer_start_ns( const time_t time );
	void    timer_start_us( const time_t time );
    #if defined(NOT_CORE_TIMER)
        #define timer_timeout()	IFS0bits.T3IF
    #else
        #define timer_timeout()	IFS0bits.CTIF
    #endif
    #define delay_us(x)		timer_start_us(x); do{}while(!timer_timeout())
    #define delay_ns(x)		timer_start_ns(x); do{}while(!timer_timeout())
#endif


#define delay_ms(x)	timer_start_ms(x); do{}while(!timer_timeout())


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//DS30_TIMER_H
