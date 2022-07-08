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


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"
#include "ds30_timer.h"


//------------------------------------------------------------------------------
// open_core_timer()
//
// The PIC32 architecture includes a core timer that is available to application
// programs. This timer is implemented in the form of two coprocessor registers:
// the Count register, and the Compare register. The Count register is
// incremented every two system clock (SYSCLK) cycles. The incrementing of Count
// can be optionally suspended during Debug mode. The Compare register is used
// to cause a timer interrupt if desired. An interrupt is generated when the
// Compare register matches the Count register. An interrupt is taken only if it
// is enabled in the interrupt controller.
//------------------------------------------------------------------------------


inline void open_core_timer( uint32_t period )
{	
    asm volatile("mtc0   $0,$9");                   //clear the count reg
    asm volatile("mtc0   %0,$11" : "+r"(period));   //set up the period in the compare reg
}// open_core_timer()


//------------------------------------------------------------------------------
// timer_start_ms()
//------------------------------------------------------------------------------
void timer_start_ms( const time_t time )
{
    #if defined(NOT_CORE_TIMER)
        PR2 =    (SYSCLK / 2 / 1000UL ) * time;
        PR3 = ( ((SYSCLK / 2 / 1000UL ) * time) >> 16 );
        TMR3 = 0;
        TMR2 = 0;
        IFS0bits.T3IF = 0;
        T2CONbits.T32 = 1;
        T2CONbits.ON = 1;
    #else
        IFS0bits.CTIF = 0;
        open_core_timer( (SYSCLK/2UL/1000UL) * time );
    #endif
}// timer_start_ms()


//------------------------------------------------------------------------------
// timer_start_us()
//------------------------------------------------------------------------------
void timer_start_us( const time_t time )
{
    #if defined(NOT_CORE_TIMER)
        PR2 =    (SYSCLK / 2 / 1000000UL ) * time;
        PR3 = ( ((SYSCLK / 2 / 1000000UL ) * time) >> 16 );
        TMR3 = 0;
        TMR2 = 0;
        IFS0bits.T3IF = 0;
        T2CONbits.T32 = 1;
        T2CONbits.ON = 1;
    #else
        IFS0bits.CTIF = 0;
        open_core_timer( ((SYSCLK/2UL/1000UL) * time) / 1000UL );
    #endif
}// timer_start_us()
