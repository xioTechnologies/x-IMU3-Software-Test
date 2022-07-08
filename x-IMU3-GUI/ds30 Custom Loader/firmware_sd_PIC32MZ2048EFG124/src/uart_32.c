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
#include "ds30_comm.h"

#if defined(USE_UART)
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "ds30_common.h"
#include "ds30_comm.h"
#include "ds30_timer.h"


//------------------------------------------------------------------------------
// Register defines
//------------------------------------------------------------------------------
#if defined(USE_UART1)
	#define UMODE	    U1MODE					
	#define UMODEbits   U1MODEbits				
   	#define USTA  		U1STA					
   	#define USTAbits	U1STAbits				
   	#define UBRG		U1BRG					
   	#define UTXREG		U1TXREG					
   	#define	URXREG		U1RXREG					
	#define	UIFS		IFS0					
	#define	URXIF		U1RXIF					
	#define	UTXIF		U1TXIF					

#elif defined(USE_UART2)
	#define UMODE	    U2MODE					
	#define UMODEbits   U2MODEbits				
   	#define USTA  		U2STA					
   	#define USTAbits	U2STAbits				
   	#define UBRG		U2BRG					
   	#define UTXREG		U2TXREG					
   	#define	URXREG		U2RXREG					
	#define	UIFS		IFS1					
	#define	URXIF		U2RXIF					
	#define	UTXIF		U2TXIF					

#elif defined(USE_UART3)
	#define UMODE	    U3MODE					
	#define UMODEbits   U3MODEbits				
   	#define USTA  		U3STA					
   	#define USTAbits	U3STAbits				
   	#define UBRG		U3BRG					
   	#define UTXREG		U3TXREG					
   	#define	URXREG		U3RXREG					
	#define	UIFS		IFS5					
	#define	URXIF		U3RXIF					
	#define	UTXIF		U3TXIF					

#elif defined(USE_UART4)
	#define UMODE	    U4MODE					
	#define UMODEbits   U4MODEbits				
   	#define USTA  		U4STA					
   	#define USTAbits	U4STAbits				
   	#define UBRG		U4BRG					
   	#define UTXREG		U4TXREG					
   	#define	URXREG		U4RXREG					
	#define	UIFS		IFS5					
	#define	URXIF		U4RXIF					
	#define	UTXIF		U4TXIF					

#elif defined(USE_UART5)
	#define UMODE	    U5MODE					
	#define UMODEbits   U5MODEbits				
   	#define USTA  		U5STA					
   	#define USTAbits	U5STAbits				
   	#define UBRG		U5BRG					
   	#define UTXREG		U5TXREG					
   	#define	URXREG		U5RXREG					
	#define	UIFS		IFS5					
	#define	URXIF		U5RXIF					
	#define	UTXIF		U5TXIF					

#elif defined(USE_UART6)
	#define UMODE	    U6MODE					
	#define UMODEbits   U6MODEbits				
   	#define USTA  		U6STA					
   	#define USTAbits	U6STAbits				
   	#define UBRG		U6BRG					
   	#define UTXREG		U6TXREG					
   	#define	URXREG		U6RXREG					
	#define	UIFS		IFS5					
	#define	URXIF		U6RXIF					
	#define	UTXIF		U6TXIF					
	
#else
	#error "Unknown UART"	   	
#endif

				
//------------------------------------------------------------------------------
// Baud rate calculations
//------------------------------------------------------------------------------
#if USE_BRGH == 1
	#define UARTBR		( (((PBCLK / BAUDRATE) / 2) - 1) / 2 )	/*brg calculation with rounding*/
#else
	#define	UARTBR		( (((PBCLK / BAUDRATE) / 8) - 1) / 2 )	/*brg calculation with rounding*/
#endif

#if USE_ABAUD == 0
	#if USE_BRGH == 1
		#define REALBR	( PBCLK / (4 * (UARTBR+1)) )
	#else
		#define REALBR	( PBCLK / (16 * (UARTBR+1)) )
	#endif
	
	#define BAUDERR	( (1000 * ( BAUDRATE - REALBR)) / BAUDRATE )
	#if ( (BAUDERR > 25) || (BAUDERR < -25) )				
		#error "Baudrate error is more than 2.5%. Remove this check or try another baudrate and/or clockspeed."
	#endif 
#endif


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
//#if !defined(IS_DS30_SD_CARD_LOADER)
static uint8_t first_read = 1;
//#endif
static uint8_t is_initialized = 0;


//------------------------------------------------------------------------------
// uart_tx_finished()
//------------------------------------------------------------------------------
void uart_wait_tx(void)
{
    timer_start_ms( COMM_TIMEOUT );
	while ( !USTAbits.TRMT && !timer_timeout() ) {        
        CLEAR_WDT();
    }
}// uart_exit()


//------------------------------------------------------------------------------
// uart_exit()
//------------------------------------------------------------------------------
void uart_exit(void)
{
    uart_wait_tx();
	UMODE = 0;
	USTA = 0;
}// uart_exit()


//------------------------------------------------------------------------------
// uart_data_available()
//------------------------------------------------------------------------------
uint8_t uart_data_available(void)
{	
	return (USTAbits.URXDA ? 1 : 0);
}// uart_data_available()


//------------------------------------------------------------------------------
// uart_init()
//------------------------------------------------------------------------------
void uart_init(void)
{
    if ( is_initialized ) return;
    is_initialized = 1;
    
	UMODE               	= 0;
	USTA                	= 0;

	#if USE_TXENABLE == 1
		TXE_TRIS            = 0;
		TXE_LAT             = 0;
	#endif

	#if USE_BRGH == 1
		UMODEbits.BRGH  	= 1;
	#endif

    #if USE_ABAUD == 0
		UBRG                = UARTBR;
		UMODEbits.UARTEN    = 1;	//enable uart
		USTA                = 0;
		USTAbits.URXEN      = 1;	//enable receive
		USTAbits.UTXEN      = 1;    //enable transmit
	#else
		//UBRG              = 0xffff;	//errata workaround
		UMODEbits.ABAUD     = 1;
		UMODEbits.UARTEN    = 1;        //enable uart

		timer_start_ms( HELLO_TIMEOUT );
		while ( UMODEbits.ABAUD ) {			
			CLEAR_WDT();
			if ( timer_timeout() ) {
				goto_user_app();
			}
		}
		USTAbits.UTXEN = 1;		//enable transmit	
		//(void)URXREG;
		uart_send_byte( (uint8_t)'K' );
		
	#endif
}// uart_init()


//------------------------------------------------------------------------------
// uart_read_byte()
//------------------------------------------------------------------------------
//#if !defined(IS_DS30_SD_CARD_LOADER)
uint8_t uart_read_byte( uint8_t* const result )
{	
	*result = 1;
	
	if ( first_read ) {
		first_read = 0;
		timer_start_ms( HELLO_TIMEOUT );
	} else {
		timer_start_ms( COMM_TIMEOUT );	
	}
	
	do {		
		CLEAR_WDT();
		if ( timer_timeout() ) {
			return 0;
		}
	} while ( !USTAbits.URXDA );
	
	*result = 0;
	return URXREG;
}// uart_read_byte()
//#endif

			
//------------------------------------------------------------------------------
// uart_send_bytes()
//------------------------------------------------------------------------------
void uart_send_bytes( const uint8_t* data, const uint16_t count ) 
{
	uint32_t i;
	
	#if USE_TXENABLE == 1
		TXE_LAT = 1;
		timer_start_us( 50 );
		while ( !timer_timeout() );
	#endif

	for ( i=0; i<count; i++ ) {
        timer_start_us( ((1000000 / BAUDRATE) * 10 * 2) + 500 );
		do {
            CLEAR_WDT();         
            if ( timer_timeout() ) {
                return;
            }
        } while ( USTAbits.UTXBF );
		UTXREG = *data++;
	}
	
	#if USE_TXENABLE == 1
		while ( !USTAbits.TRMT );
		TXE_LAT = 0;
		
		// Wait one character time(+some more) and then clear the rx buffer
		// This may be needed if Rx is not pulled up because the RS-485 transceiver Rx output is high-z while driving
		// If rx becomes low while floating it will be detected as a start bit
		timer_start_us( (1000000*13)/BAUDRATE ); while ( !timer_timeout() );
		while ( USTAbits.URXDA ) {
			(void)URXREG;
		}
	#endif
}// uart_send_bytes()


//------------------------------------------------------------------------------
// uart_send_byte()
//------------------------------------------------------------------------------
void uart_send_byte( const uint8_t byte )
{
	uart_send_bytes( &byte, 1 );
}// uart_send_byte()
		
			
//------------------------------------------------------------------------------
// uart_send_string()
//------------------------------------------------------------------------------
void uart_send_string( const char* string )
{
	uart_send_bytes( string, strlen(string) );
}// uart_send_string()
		
			
//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif //defined(USE_UART)
