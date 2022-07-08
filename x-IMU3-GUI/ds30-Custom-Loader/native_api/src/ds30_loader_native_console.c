//------------------------------------------------------------------------------
//
// Title:				ds30 Loader sample client
//
// Copyright: 			Copyright 2011-2021 DigSol Sweden AB
//
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Copyright 2011-2021 DigSol Sweden AB
//-----------------------------------------------------------------------------
//   The information in this document is the property of DigSol Sweden AB.
//   Except as specifically authorized in writing by DigSol Sweden AB
//   the receiver of this document shall keep the information contained
//   herein confidential and shall protect the same in whole or in part
//   from disclosure and dissemination to third parties.
//   Disclosure and dissemination to the receiver's employees shall
//   only be made on a strict need to know basis.
//-----------------------------------------------------------------------------
//  This code may only be used together with ds30 Loader boot loader firmwares
//  purchased from DigSol Sweden AB.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ds30_loader.h"


//-----------------------------------------------------------------------------
// parse_command_line()
//-----------------------------------------------------------------------------
static int parse_command_line( const int argc, const char** argv, ds30_options_t* const ds30_options )
{
	bool all_ok = true;

	for( int i=1; i<argc; i++ )	{

		//----------------------------------------------------------------------
		// Required
		//----------------------------------------------------------------------

		if( !strcmp(argv[i], "--file") || !strcmp(argv[i], "-f") ) {
			ds30_options->file_name = argv[++i];

		} else if( !strcmp(argv[i], "--port") || !strcmp(argv[i], "-k") ) {
			ds30_options->port_name = argv[++i];

		} else if( !strcmp(argv[i], "--baudrate") || !strcmp(argv[i], "-r") ) {
			ds30_options->baud_rate = strtoul( (char *) argv[++i], NULL, 0 );
			if (ds30_options->baud_rate == 0 ) {
				fprintf(stderr, "Invalid baud rate specified: %s" NEWLINE, argv[i]);
				all_ok = false;
			}

		} else if( !strcmp(argv[i], "--device") || !strcmp(argv[i], "-d") ) {
			ds30_options->device_name = argv[++i];


		//----------------------------------------------------------------------
		// Optional
		//----------------------------------------------------------------------

		} else if( !strcmp(argv[i], "--debugmode") ||  !strcmp(argv[i], "--debug") ||  !strcmp(argv[i], "--dbg") || !strcmp(argv[i], "-l") ) {
			ds30_options->debug_mode = true;

		} else if ( !strcmp(argv[i], "--help") || !strcmp(argv[i], "-h") || !strcmp(argv[i], "/?") ) {
			return -1;

		} else if( !strcmp(argv[i], "--comm") ) {
			i++;
			if ( !strcmp(argv[i], "UART") || !strcmp(argv[i], "uart") ) {
				ds30_options->comm_type = DS30_UART;

			} else if ( !strcmp(argv[i], "socketcan") || !strcmp(argv[i], "SocketCAN") ) {
				ds30_options->comm_type = DS30_SOCKETCAN;

			} else if ( !strcmp(argv[i], "socket") ) {
				ds30_options->comm_type = DS30_SOCKET;

			} else if ( !strcmp(argv[i], "i2c") ) {
				ds30_options->comm_type = DS30_I2C;

			} else if ( !strcmp(argv[i], "PCAN") || !strcmp(argv[i], "pcan") ) {
				ds30_options->comm_type = DS30_PCAN;

			} else if ( !strcmp(argv[i], "Kvaser") || !strcmp(argv[i], "kvaser") ) {
				ds30_options->comm_type = DS30_KVASER;

			} else {
				fprintf( stderr, "Invalid comm specified: %s" NEWLINE, argv[i] );
				all_ok = false;
			}


		//----------------------------------------------------------------------
		// Operation
		//----------------------------------------------------------------------

		} else if ( !strcmp(argv[i], "--writef") || !strcmp(argv[i], "-p") ) {
			ds30_options->write_flash = true;

		} else if ( !strcmp(argv[i], "--writeaf") || !strcmp(argv[i], "--writebf") ) {
			ds30_options->write_aux_flash = true;

		} else if ( !strcmp(argv[i], "--writee") || !strcmp(argv[i], "-e") ) {
			ds30_options->write_eeprom = true;

		} else if ( !strcmp(argv[i], "--writec") || !strcmp(argv[i], "-c") ) {
			ds30_options->write_configs = true;

		} else if ( !strcmp(argv[i], "--readf") ) {
			ds30_options->read_flash = true;

		} else if ( !strcmp(argv[i], "--readaf") || !strcmp(argv[i], "--readbf") ) {
			ds30_options->read_aux_flash = true;

		} else if ( !strcmp(argv[i], "--reade") ) {
			ds30_options->read_eeprom = true;

		} else if ( !strcmp(argv[i], "--check-bl") ) {
			ds30_options->check_for_bl = true;


		//----------------------------------------------------------------------
		// CAN
		//----------------------------------------------------------------------
		} else if( !strcmp(argv[i], "--pic-id") ) {
			ds30_options->remote_id = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--ds30-id") ) {
			ds30_options->local_id = strtoul( (char *) argv[++i], NULL, 0 );

		} else if ( !strcmp(argv[i], "--ext") ) {
			ds30_options->can_extended = true;

		} else if( !strcmp(argv[i], "--dlc") ) {
			ds30_options->can_dlc = (uint8_t)strtoul( (char *) argv[++i], NULL, 0 );

		} else if ( !strcmp(argv[i], "--canfd") ) {
			ds30_options->can_fd = true;


		//----------------------------------------------------------------------
		// Advanced
		//----------------------------------------------------------------------

		} else if ( !strcmp(argv[i], "--disable-empty-pages") ) {
			ds30_options->dont_write_empty_pages = true;

		} else if( !strcmp(argv[i], "--blplp") ) {
			ds30_options->bl_placement_p = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--blsizep") ) {
			ds30_options->bl_size_p = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--no-app-vector") ) {
			ds30_options->no_app_vector = true;

		} else if( !strcmp(argv[i], "--no-bl-vector") ) {
			ds30_options->no_bl_vector = true;

		} else if (!strcmp(argv[i], "--echo-ver")) {
			ds30_options->echo_verification = true;

		} else if ( !strcmp(argv[i], "-s") || !strcmp(argv[i], "--allow-bl-overwrite") ) {
			ds30_options->allow_bl_overwrite = true;

		} else if ( !strcmp(argv[i], "-x") || !strcmp(argv[i], "--auto-baud") ) {
			ds30_options->auto_baud = true;

		} else if (!strcmp(argv[i], "--add-crc") ) {
			ds30_options->add_crc = true;


		//----------------------------------------------------------------------
		// Timing
		//----------------------------------------------------------------------

		} else if( !strcmp(argv[i], "--ht") ) {
			ds30_options->hello_timeout = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--polltime") || !strcmp(argv[i], "-a") ) {
			ds30_options->polltime = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--timeout") || !strcmp(argv[i], "-t") ) {
			ds30_options->timeout = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--port-open-delay") ) {
			ds30_options->port_open_delay = strtoul( (char *) argv[++i], NULL, 0 );


		//----------------------------------------------------------------------
		// Device reset
		//----------------------------------------------------------------------

		} else if( !strcmp(argv[i], "--reset-command") || !strcmp(argv[i], "-q") ) {
			ds30_options->reset_command = argv[++i];

		} else if( !strcmp(argv[i], "--reset-baud-rate") || !strcmp(argv[i], "-u") ) {
			ds30_options->reset_baud_rate = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--reset-time") || !strcmp(argv[i], "-b") ) {
			ds30_options->reset_time = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--reset-dlc") ) {
			ds30_options->reset_can_dlc = (uint8_t)strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--reset-ext") ) {
			ds30_options->reset_can_extended = true;

		} else if( !strcmp(argv[i], "--reset-pic-id") ) {
			ds30_options->reset_id = strtoul( (char *) argv[++i], NULL, 0 );

		} else if( !strcmp(argv[i], "--reset-dtr") ) {
			ds30_options->reset_dtr = true;

		} else if( !strcmp(argv[i], "--reset-rts") ) {
			ds30_options->reset_rts = true;


		//----------------------------------------------------------------------
		// Security
		//----------------------------------------------------------------------

		} else if( !strcmp(argv[i], "--pw") ) {
			ds30_options->password = argv[++i];


		//----------------------------------------------------------------------
		// Invalid
		//----------------------------------------------------------------------
		} else {
			fprintf( stderr, "Invalid argument: %s" NEWLINE, argv[i] );
				all_ok = false;
		}
	}

	return (all_ok ? 0 : -1);
}// parse_command_line()


//-----------------------------------------------------------------------------
// print_usage()
//-----------------------------------------------------------------------------
static void print_usage( const char* const exec_name, ds30_options_t* ds30_options )
{
	printf( 
		NEWLINE "%s options" 
		NEWLINE 
		NEWLINE
		NEWLINE "\t### Basic ###"
		NEWLINE "\t--device ..."
		NEWLINE "\t--file ..."
		NEWLINE
		NEWLINE "\t### Optional ###"
		NEWLINE "\t--debugmode"
		NEWLINE "\t--help"
		NEWLINE
		NEWLINE "\t### Communication ###"
		NEWLINE "\t--comm [uart/socketcan/socket/i2c/pcan/kvaser]"
		NEWLINE "\t--port ..."
		NEWLINE "\t--echo-ver"
		NEWLINE
		NEWLINE "\t### Communication - UART ###"
		NEWLINE "\t--baudrate ..."
		NEWLINE "\t--auto-baud"
		NEWLINE
		NEWLINE "\t### Communication - CAN ###"
		NEWLINE "\t--baudrate ..." 
	    NEWLINE "\t--pic-id ... (%u)"
		NEWLINE "\t--ds30-id ...(%u)"
		NEWLINE "\t--ext"
		NEWLINE "\t--dlc [1-8] (%u)"
		NEWLINE
		NEWLINE "\t### Communication - I2C ###"
		NEWLINE "\t--pic-id ... (%u)"
		NEWLINE "\t--ext"
		NEWLINE
		NEWLINE "\t### Operation ###"
		NEWLINE "\t--writef"
		NEWLINE "\t--writeaf/writebf"
		NEWLINE "\t--writee"
		NEWLINE "\t--readf"
		NEWLINE "\t--readaf/readbf"
		NEWLINE "\t--reade"
		NEWLINE "\t--check-bl"
		NEWLINE
		NEWLINE "\t### Advanced ###"
		NEWLINE "\t--disable-empty-pages"
		NEWLINE "\t--blplp ..."
		NEWLINE "\t--blsizep ..."
		NEWLINE "\t--no-app-vector"
		NEWLINE "\t--no-bl-vector"
		NEWLINE "\t--allow-bl-overwrite"
		NEWLINE "\t--add-crc"
		NEWLINE
		NEWLINE "\t### Timing ###"
		NEWLINE "\t--ht ... (%u)"
		NEWLINE "\t--polltime ... (%u)"
		NEWLINE "\t--timeout ... (%u)"
		NEWLINE "\t--port-open-delay ... (0)"
		NEWLINE
		NEWLINE "\t### Reset ###"
		NEWLINE "\t--reset-command ..."
		NEWLINE "\t--reset-baud-rate ... (same as baudrate)"
		NEWLINE "\t--reset-time ... (%u)"
		NEWLINE "\t--reset-dlc [1-8] (%u)"
		NEWLINE "\t--reset-ext"
		NEWLINE "\t--reset-pic-id ... (%u)"
		NEWLINE "\t--reset-dtr"
		NEWLINE "\t--reset-rts"
		NEWLINE
		NEWLINE "\t### Security ###"
		NEWLINE "\t--pw ..."
		NEWLINE
		NEWLINE,	
		exec_name,
		ds30_options->remote_id,
		ds30_options->local_id,
		ds30_options->can_dlc,
		ds30_options->remote_id,
		ds30_options->hello_timeout,
		ds30_options->polltime,
		ds30_options->timeout,
		ds30_options->reset_time,
		ds30_options->reset_can_dlc,
		ds30_options->reset_id
	);

	// License
	ds30_print_license();
}// print_usage()


//-----------------------------------------------------------------------------
// validate_arguments()
//-----------------------------------------------------------------------------
static int validate_arguments( const ds30_options_t* const ds30_options )
{
	bool all_ok = true;

	puts( "" );

	if( !ds30_options->file_name && !ds30_options->check_for_bl ) {
		fprintf( stderr, "Please specify a hex file: --file ..." NEWLINE );
		all_ok = false;
	}

	if( !ds30_options->port_name ) {
		fprintf( stderr, "Please specify a port: --port ..." NEWLINE );
		all_ok = false;
	}

	if( !ds30_options->baud_rate ) {
		switch ( ds30_options->comm_type ) {
			case DS30_SOCKET:
			case DS30_SOCKETCAN:
			case DS30_I2C:
				break;

			default:
				fprintf( stderr, "Please specify a baud rate: --baudrate ..." NEWLINE );
				all_ok = false;
				break;
		}
	}

	if ( all_ok ) {
		return 0;
	} else {
		puts( "" );
		return -1;
	}
}// validate_arguments()


//-----------------------------------------------------------------------------
// ds30_event_handler()
//-----------------------------------------------------------------------------
static void ds30_event_handler( const void* const ds30_options_, const ds30_event_t event, const bool success, const void* const data )
{
	//const ds30_options_t* const ds30_options = (const ds30_options_t* const)ds30_options_;

	// 
	(void)ds30_options_;
	(void)success;

	switch ( event ) {

		// Operations
		case EV_OP_START:
			break;

		case EV_OP_PARSE_HEX_FILE:
			break;

		case EV_OP_PORT_OPEN_RESET:
			break;

		case EV_OP_PORT_OPEN:
			break;

		case EV_OP_RESET_DEVICE:
			break;

		case EV_OP_HELLO_RSP_RCV:
		{
			// Customization, read product id
			/*uint8_t rx_buf[1];
			int32_t res = comm_read( ds30_options.port_fd, rx_buf, 1, ds30_options.hello_timeout );
			if ( res == 0 || res == -1 ) {
				puts( "Product id timed out" NEWLINE );
			} else {
				printf( "Product id: %d" NEWLINE, rx_buf[0] );
			}*/
			break;
		}

		case EV_OP_ESTABLISH_COMM:
			break;

		case EV_OP_WRITE_FLASH:
			break;

		case EV_OP_WRITE_AUX_FLASH:
			break;

		case EV_OP_WRITE_PAGE:
			break;

		case EV_OP_WRITE_EEPROM:
			break;

		case EV_OP_WRITE_CONFIGS:
			break;

		case EV_OP_READ_FLASH:
			break;

		case EV_OP_READ_PAGE:
			break;

		case EV_OP_READ_EEPROM:
			break;

		case EV_OP_ENDED:
			break;


		// Information
		case EV_INFO_LIB: 	//from the library
		case EV_INFO_FW:	//from the firmware
		{
			const char * const text = data;
			printf( "%s", text );
			break;
		}

		case EV_INFO_PROGRESS_START:
		{
			//uint32_t max_value = *( (uint32_t*)data );
			break;
		}

		case EV_INFO_PROGRESS:
		{
			//uint32_t val = *( (uint32_t*)data );
			break;
		}


		// Results
		case EV_COMM_TX_FAIL:
			break;

		case EV_COMM_TIMEOUT:
			break;

		case EV_WRONG_UNIT_CONNECTED:
			break;

		case EV_INVALID_SETTINGS:
			break;

		case EV_INCOMPATIBLE_HEX_FILE:
			break;


		// Firmware response
		case EV_RSP_WRITE_VER_FAIL:
			break;

		case EV_RSP_COMM_CHECKSUM_FAIL:
			break;

		case EV_RSP_OP_NOT_SUPPORTED:
			break;

		case EV_RSP_BAD_PASSWORD:
			break;

		case EV_RSP_BL_PROT_TRIP:
			break;

		case EV_RSP_BAD_DATA_LEN:
			break;

		case EV_RSP_BAD_MAC:
			break;

		case EV_RSP_UNKNOWN:
			break;

		default:
			printf( "Event %d" NEWLINE, (int)event );
			break;
	}
}//ds30_event_handler()


//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
int main( const int argc, const char** argv )
{
	int				r				= -1;
	ds30_options_t	ds30_options;

	// Disable buffering
	setvbuf( stdout, NULL, _IONBF, 0 );

	//
	puts( "ds30 Loader native console 2.2.0" );
    puts( "Copyright 2011-2021 DigSol Sweden AB" NEWLINE);

	// Parse command line arguments
	ds30_set_defaults( &ds30_options );
	if ( parse_command_line(argc, argv, &ds30_options) ) {
		print_usage( argv[0], &ds30_options );
		goto main_exit;
	}

	// Validate arguments
	if ( validate_arguments(&ds30_options) ) {
		print_usage( argv[0], &ds30_options );
		goto main_exit;
	}

	// 
	ds30_options.event_callback = &ds30_event_handler;
	ds30_init();
	r = ds30_write( &ds30_options );
	
main_exit:
	//puts( "Press ENTER to continue" );
	//(void)getchar();
	
	return r;
}// main()
