//-----------------------------------------------------------------------------
// Copyright 2011-2019 DigSol Sweden AB
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
#include <stdint.h>


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ds30_fifo.h"
#include "ds30_misc.h"


//-----------------------------------------------------------------------------
// ds30_fifo_init()
//-----------------------------------------------------------------------------
void ds30_fifo_init( ds30_fifo_t* fifo )
{
	fifo->count		= 0;
	fifo->read_pos	= 0;
	fifo->size		= FIFO_SIZE;
	fifo->write_pos	= 0;
}


//-----------------------------------------------------------------------------
// ds30_fifo_pop()
//-----------------------------------------------------------------------------
uint8_t ds30_fifo_pop( ds30_fifo_t* fifo )
{
	if ( fifo->count > 0 ) {
		uint8_t data = fifo->buffer[ fifo->read_pos++ ];
		if ( fifo->read_pos >= fifo->size ) {
			fifo->read_pos = 0;
		}
		fifo->count--;
		return data;
	}
	return 0;
}


//-----------------------------------------------------------------------------
// ds30_fifo_push()
//-----------------------------------------------------------------------------
void ds30_fifo_push( ds30_fifo_t* fifo, uint8_t data )
{
	if ( fifo->count < fifo->size ) {
		fifo->buffer[ fifo->write_pos++ ] = data;
		if ( fifo->write_pos >= fifo->size ) {
			fifo->write_pos = 0;
		}
		fifo->count++;
	}
}


//-----------------------------------------------------------------------------
// ds30_fifo_peek()
//-----------------------------------------------------------------------------
uint8_t ds30_fifo_peek( ds30_fifo_t* fifo )
{
	if ( fifo->count > 0 ) {
		return fifo->buffer[ fifo->read_pos ];
	}
	return 0;
}
