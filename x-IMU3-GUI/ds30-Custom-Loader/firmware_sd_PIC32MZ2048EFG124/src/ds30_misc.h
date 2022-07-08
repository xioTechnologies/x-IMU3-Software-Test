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
#ifndef DS30_MISC_H
#define DS30_MISC_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdarg.h>


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
extern char         str_buf[64];


//------------------------------------------------------------------------------
// Function declarations
//------------------------------------------------------------------------------
void ds30_sprintf( char* str, const char* format, ... );


//------------------------------------------------------------------------------
// End of file intel_hex.h
//------------------------------------------------------------------------------
#endif//DS30_MISC_H
