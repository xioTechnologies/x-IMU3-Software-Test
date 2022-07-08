//------------------------------------------------------------------------------
//
// Title:				ds30 SD Card Loader firmware
//
// Copyright: 			Copyright 2011-2018 DigSol Sweden AB
//                                                                             
//------------------------------------------------------------------------------


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
#include "ff.h"
#include "diskio.h"
#include "board.h"
#include "ds30_common.h"
#include "ds30SdCardLoader.h"
#include "ds30Loader.h"
#include "ds30_comm.h"
#include "ds30_misc.h"
#include "ds30_timer.h"


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
static FATFS        fatFs;
static FIL          upd_file;
#if LOG_TO_FILE == 1
    static FIL      log_file;
    static uint8_t  log_file_opened = 0;
#endif
    
// Global, this will be read by the ds30 Loader library
ds30_settings_t ds30_settings = { 
    HELLOTRIES
};
// Global, this will be read by the ds30 Loader library
#if defined(IS_SECURE)    
    ds30_secure_settings_t ds30_secure_settings = { 
        {XTEA_KEY_A,     XTEA_KEY_B,     XTEA_KEY_C,     XTEA_KEY_D},
        {XTEA_MAC_KEY_A, XTEA_MAC_KEY_B, XTEA_MAC_KEY_C, XTEA_MAC_KEY_D}
    };        
#else
    
#endif


//------------------------------------------------------------------------------
// ds30_info()
// Description: this function is called by the ds30 Loader engine with 
//              status and debug information during operation
//------------------------------------------------------------------------------
void ds30_info( const char* const string, const uint8_t verbosity )
{
	CLEAR_WDT();
    if ( verbosity <= VERBOSITY ) {	
		#if LOG_TO_UART == 1
			uart_send_string( string );
		#endif
		
		#if LOG_TO_FILE == 1
            if ( log_file_opened ) {
                FRESULT fr;
                UINT bytes_written;
                UINT bytes_to_write = strlen(string);
				fr = f_write( &log_file, string, bytes_to_write, &bytes_written );
                if ( fr == FR_OK ) {
                    f_sync( &log_file );
                }
                if ( fr != FR_OK  || bytes_written != bytes_to_write ) {
                    #if LOG_TO_UART == 1
                        ds30_sprintf( str_buf, "wr log fail: 0x%c, 0x%c of 0x%c" NEWLINE, (uint8_t)fr, (uint8_t)bytes_written, (uint8_t)bytes_to_write ); 
                        uart_send_string( str_buf );
                    #endif
                }
			}
		#endif
	}
}// ds30_info()


//------------------------------------------------------------------------------
// open_log_file()
//------------------------------------------------------------------------------
#if LOG_TO_FILE == 1
static void open_log_file(void)
{
    if ( SD_WP ) {
        ds30_info( "Card is write protected, no log file is written" NEWLINE, 2 );
    } else {
        FRESULT fr;

        ds30_info( "Open " LOG_FILE_NAME "...", 2 );
        fr = f_open( &log_file, LOG_FILE_NAME, FA_OPEN_APPEND | FA_WRITE );
        if ( fr == FR_OK ) {
            f_sync( &log_file );
            log_file_opened = 1;
            ds30_info( "Log file opened" NEWLINE, 2 );
        } else {
            ds30_sprintf( str_buf, "fail 0x%c" NEWLINE, (uint8_t)fr ); ds30_info( str_buf, 2 );
        }
    }
}// open_log_file()
#endif


//------------------------------------------------------------------------------
// close_log_file()
//------------------------------------------------------------------------------
#if LOG_TO_FILE == 1
void close_log_file(void)
{	    
    if ( !log_file_opened ) {
        return;
    }
    
	ds30_info( "Closing log file...", 2 );
	if ( f_close(&log_file) == FR_OK ) {
        log_file_opened = 0;
        ds30_info( "ok" NEWLINE, 2 );
	} else {
		ds30_info( "failed" NEWLINE, 2 );
	}
}// close_log_file()
#endif


//------------------------------------------------------------------------------
// ds30_activity()
// Description: this function is called by the ds30 Loader engine when a page is 
//              erased and when a row is written
//------------------------------------------------------------------------------
void ds30_activity(void)
{
	CLEAR_WDT();
}// ds30_activity()	


//------------------------------------------------------------------------------
// ds30_exit()
// Description: called by goto_user_app()
//------------------------------------------------------------------------------
void ds30_exit(void)
{
	#if LOG_TO_FILE == 1
        close_log_file();
	#endif		
    
    f_mount( 0, "", 0 );
    
	#if LOG_TO_UART == 1
		comm_exit();
	#endif
}// ds30_exit()


//------------------------------------------------------------------------------
// read_line()
//------------------------------------------------------------------------------
DS30RESULT read_line( char* str, const uint8_t max_len )
{
	uint8_t len = 0;
	
	for ( ;; ) {
    	UINT	bytes_read;
		FRESULT fr 			= f_read( &upd_file, str, 1, &bytes_read );
        if ( fr != FR_OK ) {
            return FAIL_FILE_READ;
        } else if ( !bytes_read ) {
			return FAIL_HEX_INCOMPLETE;
        }
		if ( *str == '\r' ) {
			*str = '\0';
		} else if ( *str == '\n' ) {
			*str = '\0';
			return SUCCESS;
		}
		len++;
		if ( len == max_len ) {
			*str = '\0';
			return FAIL_LINE_LEN;
		}
		str++;
	}
}// read_line()


//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------
int main(void)
{    
    // No code should be put before the call of ds30_early_init()
	ds30_early_init();
		
	// Init the hardware
	board_init();
	#if LOG_TO_UART == 1
		uart_init();
	#endif
    spi_master_init( 1 );
    
    // Test code
    #if defined(DSS_TEST)
    if ( !BTN ) {
        goto_user_app();
    }
    #endif

	// Communication test code, when uncommented received bytes are echoed back
	/*#warning "Communication test code activated"
    comm_init();
	for ( ;; ) {
		uint8_t result;
		uint8_t read_byte = comm_read_byte( &result );
        
		if ( !result ) {
			comm_send_byte( read_byte );
		} else {
            comm_send_bytes( "tmo\r\n", (uint8_t)sizeof("tmo\r\n") );
		}
		CLEAR_WDT();
	}*/
	
	// No card, do UART/USB boot loading
    if ( SD_CD ) {
		ds30_info( "No media detected" NEWLINE, 2 );
        goto do_uart_usb_bl;
    }

    // Init file system
    {
        FRESULT fr;
        ds30_info( "Init fs...", 2 );
        fr = f_mount( &fatFs, "", 1 );
        if ( fr != FR_OK ) {
            ds30_sprintf( str_buf, "err 0x%c" NEWLINE, (uint8_t)fr ); ds30_info( str_buf, 1 );
            	goto do_uart_usb_bl;
        }
        ds30_info( "ok" NEWLINE, 2 );
    }
    
    // Open log file
    #if LOG_TO_FILE == 1        
        open_log_file();
    #endif

    // Find a hex file
    FILINFO fno;
    {
        DIR     dj;
        FRESULT fr = f_findfirst( &dj, &fno, "", "*.hex" );
        
        f_closedir( &dj );
        if ( fr != FR_OK || !fno.fname[0] ) {            
            ds30_info( "No hex file found." NEWLINE, 2 );
            goto do_uart_usb_bl;
        }
    }

	// Open hex file
    {
        ds30_sprintf( str_buf, "Op file %s...", &fno.fname[0] ); ds30_info( str_buf, 2 );
        FRESULT fr = f_open( &upd_file, &fno.fname[0], FA_OPEN_EXISTING | FA_READ );
        if ( fr != FR_OK ) {
            ds30_sprintf( str_buf, "err 0x%c" NEWLINE, (uint8_t)fr ); ds30_info( str_buf, 1 );
            goto do_uart_usb_bl;
        }
        ds30_info( "ok" NEWLINE, 2 );
    }
    
    // Do SD card boot loading   
    {
        DS30RESULT result = ds30_hex_main();
    
        // Close file
        f_close( &upd_file );

        // Output result
        switch ( result ) {        

            // Successful update
            case FINISHED:
                ds30_info( "Update succeeded" NEWLINE, 2 );
                #if LOG_TO_FILE == 1
                    close_log_file();
                #endif
                goto_user_app();	//will perform a software reset, with or without application vector

            // Failed update
            default:
                ds30_sprintf( str_buf, "Update failed: 0x%c" NEWLINE, (uint8_t)result ); ds30_info( str_buf, 1 );
                #if LOG_TO_FILE == 1
                    close_log_file();
                #endif
                goto do_uart_usb_bl;
        }
    }
    goto end;
    
    //
do_uart_usb_bl:
        #if LOG_TO_FILE == 1
            close_log_file();
        #endif
        f_mount( 0, "", 0 );
        
        uart_wait_tx();
		ds30_info( "Starting UART and USB boot loader." NEWLINE, 3 );
        uart_wait_tx();
        
        comm_init();
        for ( ;; ) {
            ds30_main();		//this will return when communication timeout occurs
            goto_user_app_2();	//will return if there is no application vector
        }    
        
end:
    #if !defined(__XC8)
        return 0;
    #endif
}// main()
