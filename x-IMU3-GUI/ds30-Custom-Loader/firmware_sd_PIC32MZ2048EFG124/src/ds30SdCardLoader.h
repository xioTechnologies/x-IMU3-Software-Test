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
#ifndef DS30SDCARDLOADER_H
#define DS30SDCARDLOADER_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define NEWLINE	"\r\n"


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
typedef enum { 
	SUCCESS				= 0, 
	FAIL				= 1,
	FAIL_FS				= 2,    //file system init failed
    FAIL_FILE_READ      = 3, 	//f_read() failed
	FAIL_HEX			= 4,    //hex file parse failed
    FAIL_HEX_INCOMPLETE = 5,    //the end-of-file record was not detected
    FAIL_LINE_LEN       = 6,	//the line is to long
	FAIL_VERIFICATION	= 7,    //write verification failed	
	FAIL_PROT			= 8,    //boot loader protection tripped
	FAIL_MAC			= 9,    //MAC verification failed
	FAIL_EVAL 			= 10,   //evaluation limitation in effect
    FINISHED            = 11   	//the end-of-file record was successfully parsed
} DS30RESULT;

typedef struct {
    uint32_t    xtea_key[4];
    uint32_t    mac_key[4];
} ds30_secure_settings_t;


//------------------------------------------------------------------------------
// Public function prototypes
//------------------------------------------------------------------------------
void        ds30_early_init(void);
void        goto_user_app(void);
DS30RESULT  ds30_hex_main(void);

void        comm_init(void);
void        comm_exit(void);
void        comm_send_string( const char* str );


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//DS30SDCARDLOADER_H
