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

#if defined(USE_SW_SPI)
#include <stdint.h>
#include "board.h"
#include "ds30_common.h"
#include "ds30_timer.h"
    

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
static uint8_t slow_mode;


//------------------------------------------------------------------------------
// spi_master_init()
//------------------------------------------------------------------------------
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
    SCK_LAT     = 0;
    slow_mode   = slow;
}// spi_master_init()


//------------------------------------------------------------------------------
// spi_exchange_byte()
//------------------------------------------------------------------------------  
uint8_t spi_exchange_byte( uint8_t data_out )
{
    uint8_t i;
    uint8_t data_in     = 0;

    for ( i=0; i<8; i++ ) {
        CLEAR_WDT();

        SCK_LAT = 0;
        if ( data_out & 0x80 ) {
            SDO_LAT = 1;
        } else {
            SDO_LAT = 0;
        }            
        SCK_LAT = 1;

        data_out <<= 1;
        data_in  <<= 1;

        if ( SDI_PORT ) {
            data_in |= 1;
        }

        if ( slow_mode ) {
            delay_us( 3 );
        } else {
            delay_us( 1 );
        }
    }

    return data_in;
}// spi_exchange_byte()


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif //USE_SW_SPI
