//------------------------------------------------------------------------------
// Copyright 2020 DigSol Sweden AB
//------------------------------------------------------------------------------
//   The information in this document is the property of DigSol Sweden AB.
//   Except as specifically authorized in writing by DigSol Sweden AB
//   the receiver of this document shall keep the information contained
//   herein confidential and shall protect the same in whole or in part
//   from disclosure and dissemination to third parties.
//   Disclosure and dissemination to the receiver's employees shall
//   only be made on a strict need to know basis.
//------------------------------------------------------------------------------
#ifndef BOARD_H
#define BOARD_H


//------------------------------------------------------------------------------
// Board include
//------------------------------------------------------------------------------
#if defined(__32MZ2048EFG124__)
	#include "board_32MZ2048EFG124.h"

#elif defined(__32MZ2048ECH100__)
	#include "board_32MZ2048ECH100.h"	
#else 
	#error "Unlicensed board"
#endif


//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
void board_init(void);
void APP_Initialize (void);
void APP_Tasks(void);


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//BOARD_H
