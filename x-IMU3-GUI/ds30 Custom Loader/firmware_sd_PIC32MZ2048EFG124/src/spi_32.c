//------------------------------------------------------------------------------
// Copyright 2013-2018 DigSol Sweden AB
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

#if defined(USE_SPI)
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "ds30_comm.h"
#include "ds30_common.h"
#include "ds30_timer.h"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#if defined(USE_SPI1)
   	#define	SPISTAT		SPI1STAT	
   	#define	SPISTATbits	SPI1STATbits
   	#define	SPICON1		SPI1CON	
   	#define	SPICON1bits	SPI1CONbits
   	#define SPIBUF		SPI1BUF	
    #define SPIBRG      SPI1BRG

#elif defined(USE_SPI2)
   	#define	SPISTAT		SPI2STAT	
   	#define	SPISTATbits	SPI2STATbits
   	#define	SPICON1		SPI2CON	
   	#define	SPICON1bits	SPI2CONbits
   	#define SPIBUF		SPI2BUF
    #define SPIBRG      SPI2BRG	
   	
#elif defined(USE_SPI3)
   	#define	SPISTAT		SPI3STAT	
   	#define	SPISTATbits	SPI3STATbits
   	#define	SPICON1		SPI3CON	
   	#define	SPICON1bits	SPI3CONbits
   	#define SPIBUF		SPI3BUF	
    #define SPIBRG      SPI3BRG	

#elif defined(USE_SPI4)
   	#define	SPISTAT		SPI4STAT	
   	#define	SPISTATbits	SPI4STATbits
   	#define	SPICON1		SPI4CON	
   	#define	SPICON1bits	SPI4CONbits
   	#define SPIBUF		SPI4BUF	
    #define SPIBRG      SPI4BRG

#elif defined(USE_SPI5)
   	#define	SPISTAT		SPI5STAT	
   	#define	SPISTATbits	SPI5STATbits
   	#define	SPICON1		SPI5CON	
   	#define	SPICON1bits	SPI5CONbits
   	#define SPIBUF		SPI5BUF	
    #define SPIBRG      SPI5BRG

#elif defined(USE_SPI6)
   	#define	SPISTAT		SPI6STAT	
   	#define	SPISTATbits	SPI6STATbits
   	#define	SPICON1		SPI6CON	
   	#define	SPICON1bits	SPI6CONbits
   	#define SPIBUF		SPI6BUF	
    #define SPIBRG      SPI6BRG		
#endif


//------------------------------------------------------------------------------
// Validate SD Card SPI speeds
//------------------------------------------------------------------------------
#if defined(IS_DS30_SD_CARD_LOADER) && !defined(USE_SW_SPI)
    // Defines
    #define MIN_SPEED_SLOW      100000  //dont change
    #define MAX_SPEED_SLOW      400000  //dont change

    // Calculated brgs
    #define SPIBRG_SLOW         ( PBCLK / (2*MAX_SPEED_SLOW) - 1 )
    #define SPIBRG_FAST         ( PBCLK / (2*SPI_TARGET_SPEED_FAST) - 1 )

    // Calculate actual speeds
    #define FSCK_SLOW           ( PBCLK / (2 * (SPIBRG_SLOW+1)) )
    #define FSCK_FAST           ( PBCLK / (2 * (SPIBRG_FAST+1)) )

    // Validate speeds
    #if FSCK_SLOW < MIN_SPEED_SLOW || FSCK_SLOW > MAX_SPEED_SLOW
	    #warning "Unsupported SD Card slow clock."
    #endif
    #if FSCK_FAST > 25000000
	    #warning "Unsupported SD Card fast clock."
    #endif
#endif


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER)
static uint8_t first_read = 1;
#endif


//------------------------------------------------------------------------------
// spi_master_init()
//------------------------------------------------------------------------------
#if defined(IS_DS30_SD_CARD_LOADER)
void spi_master_init( const uint8_t slow )
{
    SD_CS_TRIS  = 0;
    #if USE_SD_CD != 0
        SD_CD_TRIS  = 1; 
    #endif
    #if USE_SD_WP != 0
        SD_WP_TRIS  = 1;
    #endif
    

    SCK_TRIS    = 0; 
    SDI_TRIS    = 1; 
    SDO_TRIS    = 0;
    
    if ( slow ) {
        SPIBRG = SPIBRG_SLOW;
    } else {
        SPIBRG = SPIBRG_FAST;
    }

    SPICON1bits.MSTEN 	= 1;
    SPICON1bits.CKE 	= 1;
    SPICON1bits.CKP 	= 0;
    SPICON1bits.ON   	= 1;
}//spi_master_init()
#endif


//------------------------------------------------------------------------------
// spi_exchange_byte()
//------------------------------------------------------------------------------
#if defined(IS_DS30_SD_CARD_LOADER) 
uint8_t spi_exchange_byte( const uint8_t data )
{
    SPIBUF = data;	
    do {	
        CLEAR_WDT();
    } while( !SPISTATbits.SPIRBF );

    return SPIBUF;
}// spi_exchange_byte()
#endif


//------------------------------------------------------------------------------
// comm_exit()
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER) 
void comm_exit(void)
{		
}//comm_exit()
#endif


//------------------------------------------------------------------------------
// comm_init()
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER) 
void comm_init(void)
{
	SPICON1bits.CKE 	= 1;
	SPICON1bits.CKP 	= 0;	
	SPICON1bits.ON 		= 1;
}// comm_init()
#endif


//------------------------------------------------------------------------------
// comm_read_byte()
//------------------------------------------------------------------------------  
#if !defined(IS_DS30_SD_CARD_LOADER) 
uint8_t comm_read_byte( uint8_t* const result )
{
	*result = 1;
	
	if ( first_read ) {
		first_read = 0;
		timer_start_ms( HELLO_TIMEOUT );
	} else {
		timer_start_ms( COMM_TIMEOUT );	
	}
    
	SPIBUF = 0;	
	do {	
		CLEAR_WDT();

		if ( timer_timeout() ) {
			return 0;
		}
	} while( !SPISTATbits.SPIRBF );	

	uint8_t read_byte = SPIBUF;
	*result = 0;
	return read_byte;
}// comm_read_byte()
#endif


//------------------------------------------------------------------------------
// comm_send_bytes()
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER)
void comm_send_bytes( const uint8_t* data, const uint16_t count )
{
    uint16_t i;
    for ( i=0; i<count; i++ ) {
		timer_start_ms( COMM_TIMEOUT );
		SPIBUF = *data++;
		
		do { 
			CLEAR_WDT();

			if ( timer_timeout() ) {
				return;
			}
		} while ( !SPISTATbits.SPIRBF );

		(void)SPIBUF;
	}	
}// comm_send_bytes()
#endif


//------------------------------------------------------------------------------
// comm_send_byte()
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER) 
void comm_send_byte( const uint8_t byte )
{
	comm_send_bytes( &byte, 1 );
}// comm_send_byte()
#endif


//------------------------------------------------------------------------------
// comm_send_string()
//------------------------------------------------------------------------------
#if !defined(IS_DS30_SD_CARD_LOADER) 
void comm_send_string( const char* str )
{
	comm_send_bytes( (uint8_t*)str, strlen(str) );
}// comm_send_string()
#endif


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif //defined(USE_SPI)
