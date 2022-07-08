//------------------------------------------------------------------------------
//
// Title:				ds30 Loader
//
// Copyright: 			Copyright 2011-2012 DigSol Sweden AB
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Copyright 2011-2016 DigSol Sweden AB
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
#ifndef DS30_FIFO_H
#define DS30_FIFO_H


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define FIFO_SIZE	4096


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct {
	uint16_t	count;
	uint16_t	read_pos;
	uint16_t	write_pos;
	uint16_t	size;
	uint8_t		buffer[ FIFO_SIZE ];
} ds30_fifo_t;


//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void ds30_fifo_init( ds30_fifo_t* fifo );
uint8_t ds30_fifo_pop( ds30_fifo_t* fifo );
void ds30_fifo_push( ds30_fifo_t* fifo, uint8_t data );
uint8_t ds30_fifo_peek( ds30_fifo_t* fifo );


//-----------------------------------------------------------------------------
// End of file
//-----------------------------------------------------------------------------
#endif //DS30_FIFO_H
