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

#include <string.h>
#include <stdint.h>
#include "board.h"
#include "ds30_comm.h"
#include "ds30_timer.h"
#include "ds30_common.h"


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
typedef enum {
    NOT_INITIALIZED = 0,
    NOT_DETERMINED  = 1,
    UART            = 2, 
    USB             = 3
} comm_type_t;


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
static comm_type_t  comm_type   = NOT_INITIALIZED;


//------------------------------------------------------------------------------
// comm_exit()
//------------------------------------------------------------------------------
void comm_exit(void)
{
    uart_exit();
    usb_exit();
}// comm_exit()


//------------------------------------------------------------------------------
// comm_clear_rx()
//------------------------------------------------------------------------------
void comm_clear_rx(void)
{
     //only for CAN
}// comm_clear_rx()


//------------------------------------------------------------------------------
// comm_init()
//------------------------------------------------------------------------------
void comm_init(void)
{    
    uart_init();
    usb_init();
    
    comm_type = NOT_DETERMINED;
}// comm_init()


//------------------------------------------------------------------------------
// comm_read_byte()
//------------------------------------------------------------------------------
//#if !defined(IS_DS30_SD_CARD_LOADER)
uint8_t comm_read_byte( uint8_t* const result )
{
    switch ( comm_type ) {
        case NOT_DETERMINED:            
            *result = 1;
            timer_start_ms( HELLO_TIMEOUT );

            for ( ;; ) {
                CLEAR_WDT();

                if ( timer_timeout() ) {
                    return 0;
                }
                if ( uart_data_available() ) {
                    comm_type = UART;                    
                    usb_exit();
                    return uart_read_byte( result );
                    
                } else if ( usb_data_available() ) {
                    comm_type = USB;
                    return usb_read_byte( result );
                }
            }
            
        case UART:
            return uart_read_byte( result );

        case USB:
            return usb_read_byte( result );
            
        case NOT_INITIALIZED:
            *result = 1;
            return 0;
                    
    }
}// comm_read_byte()
//#endif

			
//------------------------------------------------------------------------------
// comm_send_bytes()
//------------------------------------------------------------------------------
void comm_send_bytes( const uint8_t* data, const uint16_t count ) 
{
    switch ( comm_type ) {
        case NOT_DETERMINED:   
        case UART:
            uart_send_bytes( data, count );
            break;
        
        case USB:
            usb_send_bytes( data, count );
            break;
            
        case NOT_INITIALIZED:
            return;
    }
}// comm_send_bytes()


//------------------------------------------------------------------------------
// comm_send_byte()
//------------------------------------------------------------------------------
void comm_send_byte( const uint8_t byte )
{
	comm_send_bytes( &byte, 1 );
}// comm_send_byte()
		
			
//------------------------------------------------------------------------------
// comm_send_string()
//------------------------------------------------------------------------------
void comm_send_string( const char* string )
{
	comm_send_bytes( string, strlen(string) );
}// comm_send_string()
		
			
//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
