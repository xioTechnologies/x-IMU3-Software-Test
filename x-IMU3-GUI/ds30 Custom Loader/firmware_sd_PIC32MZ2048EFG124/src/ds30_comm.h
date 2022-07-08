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
#ifndef DS30_COMM_H
#define DS30_COMM_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include "board.h"


//------------------------------------------------------------------------------
// Verify settings
//------------------------------------------------------------------------------
#if defined(USE_UART1) || defined(USE_UART2) || defined(USE_UART3) || defined(USE_UART4) || defined(USE_UART5) || defined(USE_UART6)
	#define USE_UART
#endif

#if defined(USE_CAN1) || defined(USE_CAN2)
	#define USE_CAN

    #if CAN_EXT == 0
        #if ID_PIC > 0x7ff
            #error "PIC ID is out of range. Max is 0x7ff"
        #endif
        #if ID_GUI > 0x7ff
            #error "GUI ID is out of range. Max is 0x7ff"
        #endif
    #else
        #if ID_PIC > 0x1fffffff
            #error "PIC ID is out of range. Max is 0x1fffffff"
        #endif
        #if ID_GUI > 0x1fffffff
            #error "GUI ID is out of range. Max is 0x1fffffff"
        #endif
    #endif
#endif

#if defined(USE_I2C1) || defined(USE_I2C2) || defined(USE_I2C3) || defined(USE_I2C4)
	#define USE_I2C

    #if defined(USE_I2C1)
        #if defined(USE_I2C2)
            #error "Multiple I2C are specified"
        #endif
        #if defined(USE_I2C3)
            #error "Multiple I2C are specified"
        #endif
    #endif

    #if defined(USE_I2C2)
        #if defined(USE_I2C3)
            #error "Multiple I2C are specified"
        #endif
    #endif

    #if I2C_ADDR >= 0
        #if I2C_ADDR <= 7
            #error "This is a reserved address"
        #endif
    #endif

    #if I2C_ADDR >= 124
        #error "This is a reserved address"
    #endif
#endif

#if defined(USE_SPI1) || defined(USE_SPI2) || defined(USE_SPI3) || defined(USE_SPI4) || defined(USE_SPI5) || defined(USE_SPI6)
	#define USE_SPI

    #if defined(USE_SPI1)
        #if defined(USE_SPI2)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI3)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI4)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI5)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI6)
            #error "Multiple SPI are specified"
        #endif
    #endif

    #if defined(USE_SPI2)
        #if defined(USE_SPI3)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI4)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI5)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI6)
            #error "Multiple SPI are specified"
        #endif
    #endif

    #if defined(USE_SPI3)
        #if defined(USE_SPI4)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI5)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI6)
            #error "Multiple SPI are specified"
        #endif
    #endif

    #if defined(USE_SPI4)
        #if defined(USE_SPI5)
            #error "Multiple SPI are specified"
        #endif
        #if defined(USE_SPI6)
            #error "Multiple SPI are specified"
        #endif
    #endif

    #if defined(USE_SPI5)
        #if defined(USE_SPI6)
            #error "Multiple SPI are specified"
        #endif
    #endif

    #if SPI_SS != 0 && SPI_SS != 1
        #error "Invalid value for SPI_SS define"
    #endif
#endif

#if defined(USE_UART) && ( defined(USE_SWUART) || defined(USE_CAN) || defined(USE_I2C) || ( (defined(USE_SPI) || defined(USE_SW_SPI)) && !defined(IS_DS30_SD_CARD_LOADER) ) || defined(USE_USB) )
	//#error "Multiple communication modules specified"

#elif defined(USE_SWUART) && ( defined(USE_CAN) || defined(USE_I2C) || defined(USE_SPI) || defined(USE_SW_SPI) ||  defined(USE_USB) )
	#error "Multiple communication modules specified"

#elif defined(USE_CAN) && ( defined(USE_I2C) || defined(USE_SPI) || defined(USE_SW_SPI) ||  defined(USE_USB) )
	#error "Multiple communication modules specified"

#elif defined(USE_I2C) && ( defined(USE_SPI) || defined(USE_SW_SPI) || defined(USE_USB) )
	#error "Multiple communication modules specified"

#elif defined(USE_SPI) && (  defined(USE_SW_SPI) || defined(USE_USB) )
	#error "Multiple communication modules specified"

#elif defined(USE_SW_SPI) && ( defined(USE_USB) )
	#error "Multiple communication modules specified"

#elif !defined(USE_UART) && !defined(USE_SWUART) && !defined(USE_CAN) && !defined(USE_I2C) && !defined(USE_SPI) && !defined(USE_SW_SPI) && !defined(USE_USB)
	#error "No communication specified"
#endif


//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

//#if defined(IS_DS30_SD_CARD_LOADER)
    uint8_t spi_exchange_byte( const uint8_t );
    void    spi_master_init( const uint8_t slow );
//#else
    void    uart_exit(void);
    void    uart_init(void);
    uint8_t uart_read_byte( uint8_t* const result );
    void    uart_send_byte( const uint8_t byte );
    void    uart_send_bytes( const uint8_t* data, const uint16_t count );
    void    uart_send_string( const char* str );
    uint8_t uart_data_available(void);
    
    void    usb_exit(void);
    void    usb_init(void);
    uint8_t usb_read_byte( uint8_t* const result );
    void    usb_send_byte( const uint8_t byte );
    void    usb_send_bytes( const uint8_t* data, const uint16_t count );
    void    usb_send_string( const char* str );
    uint8_t usb_data_available(void);
//#endif

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
#endif//DS30_COMM_H
