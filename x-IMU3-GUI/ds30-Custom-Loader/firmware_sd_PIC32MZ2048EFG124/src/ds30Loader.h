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
#ifndef DS30LOADER_H
#define DS30LOADER_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
typedef struct {
	uint8_t     hello_tries;
} ds30_settings_t;



//------------------------------------------------------------------------------
// Public function prototypes
//------------------------------------------------------------------------------
void ds30_main(void);
    

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//DS30LOADER_H
