//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "system_config_ds30.h"
#include "usb/usb_device_cdc_ds30.h"


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
#include "ds30_comm.h"

//#if defined(USE_USB)
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "ds30_common.h"
#include "ds30_timer.h"
#include "ds30_fifo.h"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define RX_BUF_SIZE		600
#define MAX_TX			240


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
static uint8_t      first_read              = 1;

ds30_fifo_t usb_rx_fifo;





//------------------------------------------------------------------------------
// Data types
//------------------------------------------------------------------------------
typedef enum
{    
    APP_STATE_INIT=0,                       // Application's state machine's initial state.     
    APP_STATE_WAIT_FOR_CONFIGURATION,       // Application waits for device configuration        
    APP_STATE_SCHEDULE_READ,                // Wait for a character receive     
    APP_STATE_WAIT_FOR_READ_COMPLETE,       // A character is received from host     
    APP_STATE_IDLE,
    APP_STATE_WAIT_FOR_WRITE_COMPLETE,      // Wait for the write to complete     
    APP_STATE_ERROR                         // Application Error state

} APP_STATES;

typedef struct
{   
    USB_DEVICE_HANDLE               deviceHandle;            // Device layer handle returned by device layer open function     
    APP_STATES                      state;                   // Application's current state    
    USB_CDC_LINE_CODING             setLineCodingData;       // Set Line Coding Data     
    bool                            isConfigured;            // Device configured state     
    USB_CDC_LINE_CODING             getLineCodingData;       // Get Line Coding Data     
    USB_CDC_CONTROL_LINE_STATE      controlLineStateData;    // Control Line State     
    USB_DEVICE_CDC_TRANSFER_HANDLE  readTransferHandle;      // Read transfer handle     
    USB_DEVICE_CDC_TRANSFER_HANDLE  writeTransferHandle;     // Write transfer handle     
    bool                            isReadComplete;          // True if a character was read     
    bool                            isWriteComplete;         // True if a character was written  
    bool                            sofEventHasOccurred;     // Flag determines SOF event occurrence     
    uint16_t                        breakData;               // Break data  
    uint8_t *                       readBuffer;              // Application CDC read buffer     
    uint32_t                        numBytesRead;            // Number of bytes read from Host  
} APP_DATA;

uint8_t APP_MAKE_BUFFER_DMA_READY read_buffer[APP_READ_BUFFER_SIZE];
uint8_t APP_MAKE_BUFFER_DMA_READY write_buffer[APP_WRITE_BUFFER_SIZE];


//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
APP_DATA appData;
SYSTEM_OBJECTS sysObj;  // Structure to hold the object handles for the modules in the system. 


//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

// USB Driver Initialization
const DRV_USBHS_INIT drvUSBHSInit =
{    
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},  /* System module initialization */
    .operationMode = DRV_USBHS_OPMODE_DEVICE,   /* Operation Mode */
    .operationSpeed = USB_SPEED_HIGH,   /* Operation Speed */ 
    .stopInIdle = false,    /* Stop in idle */    
    .suspendInSleep = false,    /* Suspend in sleep */    
    .usbID = USBHS_ID_0,    /* Identifies peripheral (PLIB-level) ID */
    
};

// USB Device Function Driver Init Data
const USB_DEVICE_CDC_INIT cdcInit0 =
{
    .queueSizeRead = 1,
    .queueSizeWrite = 1,
    .queueSizeSerialStateNotification = 1
};

// USB Device Layer Function Driver Registration  Table
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{
    // Function 1
    { 
        .configurationValue     = 1,                                        /* Configuration value */ 
        .interfaceNumber        = 0,                                        /* First interfaceNumber of this function */ 
        .speed                  = USB_SPEED_HIGH|USB_SPEED_FULL,            /* Function Speed */ 
        .numberOfInterfaces     = 2,                                        /* Number of interfaces */
        .funcDriverIndex        = 0,                                        /* Index of CDC Function Driver */
        .driver                 = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit         = (void*)&cdcInit0                          /* Function driver init data */
    },
};

// USB Device Descriptor 
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
	USB_CDC_CLASS_CODE,             // Class Code
    USB_CDC_SUBCLASS_CODE,          // Subclass code
    0x00,                           // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,     // Max packet size for EP0, see system_config.h
    0x04D8,                         // Vendor ID
    0x000A,                         // Product ID
    0x0100,                         // Device release number in BCD format
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
    0x00,                           // Device serial number string index
    0x01                            // Number of possible configurations
};

//  USB Device Qualifier Descriptor for this demo.
const USB_DEVICE_QUALIFIER deviceQualifierDescriptor1 =
{
    0x0A,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE_QUALIFIER,    // Device Qualifier Type
    0x0200,                             // USB Specification Release number
	USB_CDC_CLASS_CODE,                 // Class Code
    USB_CDC_SUBCLASS_CODE,              // Subclass code
    0x00,                               // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,         // Maximum packet size for endpoint 0
    0x01,                               // Number of possible configurations
    0x00                                // Reserved for future use.
};

// USB High Speed Configuration Descriptor 
const uint8_t highSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */
    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    67,0,                                               //(67 Bytes)Size of the Config descriptor.e
    2,                                                  // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
    
    /* Descriptor for Function 1 - CDC     */     
    /* Interface Descriptor */
    0x09,                                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                       // Descriptor Type
    0,                                              // Interface Number
    0x00,                                           // Alternate Setting Number
    0x01,                                           // Number of endpoints in this interface
    USB_CDC_COMMUNICATIONS_INTERFACE_CLASS_CODE,    // Class code
    USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,        // Subclass code
    USB_CDC_PROTOCOL_AT_V250,                       // Protocol code
    0x00,                                           // Interface string index

    /* CDC Class-Specific Descriptors */
    sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR),               // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_HEADER,                                  // Type of functional descriptor
    0x20,0x01,                                                  // CDC spec version

    sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR),                  // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_ABSTRACT_CONTROL_MANAGEMENT,             // Type of functional descriptor
    USB_CDC_ACM_SUPPORT_LINE_CODING_LINE_STATE_AND_NOTIFICATION,// bmCapabilities of ACM

    sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_HEADER) + 1,     // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_UNION,                                   // Type of functional descriptor
    0,                                                          // com interface number
    1,

    sizeof(USB_CDC_CALL_MANAGEMENT_DESCRIPTOR),                 // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_CALL_MANAGEMENT,                         // Type of functional descriptor
    0x00,                                                       // bmCapabilities of CallManagement
    1,                                                          // Data interface number

    /* Interrupt Endpoint (IN) Descriptor */
    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,        // EndpointAddress ( EP1 IN INTERRUPT)
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes type of EP (INTERRUPT)
    0x10,0x00,                      // Max packet size of this EP
    0x02,                           // Interval (in ms)

    /* Interface Descriptor */
    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    1,                                  // Interface Number
    0x00,                               // Alternate Setting Number
    0x02,                               // Number of endpoints in this interface
    USB_CDC_DATA_INTERFACE_CLASS_CODE,  // Class code
    0x00,                               // Subclass code
    USB_CDC_PROTOCOL_NO_CLASS_SPECIFIC, // Protocol code
    0x00,                               // Interface string index

    /* Bulk Endpoint (OUT) Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP2 OUT )
	USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    0x00, 0x02,                 // Max packet size of this EP
    0x00,                       // Interval (in ms)

     /* Bulk Endpoint (IN)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP2 IN )
    0x02,                       // Attributes type of EP (BULK)
    0x00, 0x02,                 // Max packet size of this EP
    0x00,                       // Interval (in ms)

};

// Array of High speed config descriptors
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE highSpeedConfigDescSet[1] =
{
    highSpeedConfigurationDescriptor
};

// USB Full Speed Configuration Descriptor
const uint8_t fullSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    67,0,                //(67 Bytes)Size of the Config descriptor.e
    2,                                                  // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
    /* Descriptor for Function 1 - CDC     */ 
    
    /* Interface Descriptor */

    0x09,                                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                       // Descriptor Type
    0,                                              // Interface Number
    0x00,                                           // Alternate Setting Number
    0x01,                                           // Number of endpoints in this interface
    USB_CDC_COMMUNICATIONS_INTERFACE_CLASS_CODE,    // Class code
    USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,        // Subclass code
    USB_CDC_PROTOCOL_AT_V250,                       // Protocol code
    0x00,                                           // Interface string index

    /* CDC Class-Specific Descriptors */

    sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR),               // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_HEADER,                                  // Type of functional descriptor
    0x20,0x01,                                                  // CDC spec version

    sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR),                  // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_ABSTRACT_CONTROL_MANAGEMENT,             // Type of functional descriptor
    USB_CDC_ACM_SUPPORT_LINE_CODING_LINE_STATE_AND_NOTIFICATION,// bmCapabilities of ACM

    sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_HEADER) + 1,     // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_UNION,                                   // Type of functional descriptor
    0,                                                          // com interface number
    1,

    sizeof(USB_CDC_CALL_MANAGEMENT_DESCRIPTOR),                 // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_CALL_MANAGEMENT,                         // Type of functional descriptor
    0x00,                                                       // bmCapabilities of CallManagement
    1,                                                          // Data interface number

    /* Interrupt Endpoint (IN) Descriptor */

    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,        // EndpointAddress ( EP1 IN INTERRUPT)
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes type of EP (INTERRUPT)
    0x10,0x00,                      // Max packet size of this EP
    0x02,                           // Interval (in ms)

    /* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    1,      // Interface Number
    0x00,                               // Alternate Setting Number
    0x02,                               // Number of endpoints in this interface
    USB_CDC_DATA_INTERFACE_CLASS_CODE,  // Class code
    0x00,                               // Subclass code
    USB_CDC_PROTOCOL_NO_CLASS_SPECIFIC, // Protocol code
    0x00,                               // Interface string index

    /* Bulk Endpoint (OUT) Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP2 OUT )
	USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    0x40,0x00,                  // Max packet size of this EP
    0x00,                       // Interval (in ms)

     /* Bulk Endpoint (IN)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP2 IN )
    0x02,                       // Attributes type of EP (BULK)
    0x40,0x00,                  // Max packet size of this EP
    0x00,                       // Interval (in ms)


};

// Array of Full speed config descriptors
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};

// Language code string descriptor
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[1];
}
sd000 =
{
    sizeof(sd000),          // Size of this descriptor in bytes
    USB_DESCRIPTOR_STRING,  // STRING descriptor type
    {0x0409}                // Language ID
};

// Manufacturer string descriptor
const struct
{
    uint8_t bLength;        // Size of this descriptor in bytes
    uint8_t bDscType;       // STRING descriptor type
    uint16_t string[25];    // String
}
sd001 =
{
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}

};

// Product string descriptor
const struct
{
    uint8_t bLength;        // Size of this descriptor in bytes
    uint8_t bDscType;       // STRING descriptor type
    uint16_t string[22];    // String
}
sd002 =
{
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {'S','i','m','p','l','e',' ','C','D','C',' ','D','e','v','i','c','e',' ','D','e','m','o'}
}; 

// Array of string descriptors
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};

// USB Device Layer Master Descriptor Table 
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
    &deviceDescriptor,          /* High speed device descriptor*/
    1,                          /* Total number of high speed configurations available */
    highSpeedConfigDescSet,     /* Pointer to array of high speed configurations descriptors. */
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
    &deviceQualifierDescriptor1,// Pointer to full speed dev qualifier.
    &deviceQualifierDescriptor1 // Pointer to high speed dev qualifier.
};

// USB Device Layer Initialization Data
const USB_DEVICE_INIT usbDevInitData =
{    
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},          /* System module initialization */ 
    .registeredFuncCount = 1,                           /* Number of function drivers registered to this instance of the       USB device layer */   
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,  /* Function driver table registered to this instance of the USB device layer*/    
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor, /* Pointer to USB Descriptor structure */    
    .deviceSpeed = USB_SPEED_HIGH,                      /* USB Device Speed */   
    .driverIndex = DRV_USBHS_INDEX_0,                   /* Index of the USB Driver to be used by this Device Layer Instance */    
    .usbDriverInterface = DRV_USBHS_DEVICE_INTERFACE,   /* Pointer to the USB Driver Functions. */
    
};


//------------------------------------------------------------------------------
// APP_USBDeviceCDCEventHandler())
// USB CDC Device Events - Application Event Handler
//------------------------------------------------------------------------------
static USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index,
    USB_DEVICE_CDC_EVENT event,
    void * pData,
    uintptr_t userData
)
{
    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead; 

    switch ( event ) {
        
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:
            // This means the host wants to know the current line coding. This is a control transfer request. Use the
            // USB_DEVICE_ControlSend() function to send the data to host.
            USB_DEVICE_ControlSend( appDataObject->deviceHandle, &appDataObject->getLineCodingData, sizeof(USB_CDC_LINE_CODING) );
            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:
            // This means the host wants to set the line coding. This is a control transfer request. Use the
            // USB_DEVICE_ControlReceive() function to receive the data from the host
            USB_DEVICE_ControlReceive( appDataObject->deviceHandle, &appDataObject->setLineCodingData, sizeof(USB_CDC_LINE_CODING) );
            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:
            // This means the host is setting the control line state. Read the control line state. We will accept this request for now.
            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier = controlLineStateData->carrier;
            
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:
            // This means that the host is requesting that a break of the specified duration be sent. Read the break duration
            appDataObject->breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *)pData)->breakDuration;
            
            // Complete the control transfer by sending a ZLP 
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);            
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:
            // This means that the host has sent some data
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            appDataObject->isReadComplete = true;
            appDataObject->numBytesRead = eventDataRead->length; 
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            // The data stage of the last control transfer is complete. For now we accept all the data
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:
            // This means the GET LINE CODING function data is valid. We dont do much with this data in this demo.
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:
            // This means that the data write got completed. We can schedule the next read.
            appDataObject->isWriteComplete = true;
            break;

        default:
            break;
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}


//------------------------------------------------------------------------------
// APP_USBDeviceEventHandler()
// Application USB Device Layer Event Handler.
//------------------------------------------------------------------------------
static void APP_USBDeviceEventHandler ( USB_DEVICE_EVENT event, void * eventData, uintptr_t context )
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;

    switch ( event ) {
        
        case USB_DEVICE_EVENT_SOF:
            // This event is used for switch debounce. This flag is reset by the switch process routine.
            appData.sofEventHasOccurred = true;
            break;

        case USB_DEVICE_EVENT_RESET:
            appData.isConfigured = false;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            // Check the configuration. We only support configuration 1 
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            if ( configuredEventData->configurationValue == 1 ) {
                // Register the CDC Device application event handler here. Note how the appData object pointer is passed as the user data
                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                // Mark that the device is now configured
                appData.isConfigured = true;
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:
            // VBUS was detected. We can attach the device
            USB_DEVICE_Attach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            // VBUS is not available any more. Detach the device.
            USB_DEVICE_Detach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}


//------------------------------------------------------------------------------
// APP_USBDeviceEventHandler()
// This function is called in every step of the application state machine.
// This function returns true if the device was reset 
//------------------------------------------------------------------------------
static bool APP_StateReset(void)
{   

    bool retVal;

    if( appData.isConfigured == false ) {
        appData.state               = APP_STATE_WAIT_FOR_CONFIGURATION;
        appData.readTransferHandle  = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.isReadComplete      = true;
        appData.isWriteComplete     = true;
        retVal = true;
    } else {
        retVal = false;
    }

    return retVal;
}


//------------------------------------------------------------------------------
// APP_Initialize()
//------------------------------------------------------------------------------
void APP_Initialize ( void )
{    
    sysObj.drvUSBObject                     = DRV_USBHS_Initialize(DRV_USBHS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBHSInit);   /* Initialize USB Driver */ 
    sysObj.usbDevObject0                    = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);  /* Initialize the USB device layer */ 
        
    appData.state                           = APP_STATE_INIT;                                         /* Place the App state machine in its initial state. */        
    appData.deviceHandle                    = USB_DEVICE_HANDLE_INVALID;                       /* Device Layer Handle  */    
    appData.isConfigured                    = false;                                           /* Device configured status */    
    appData.getLineCodingData.dwDTERate     = 9600;                                     // Initial get line coding state
    appData.getLineCodingData.bParityType   =  0;
    appData.getLineCodingData.bParityType   = 0;
    appData.getLineCodingData.bDataBits     = 8;    
    appData.readTransferHandle              = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;    // Read Transfer Handle 
    appData.writeTransferHandle             = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;   /* Write Transfer Handle */
    appData.isReadComplete                  = true;                                          /* Intialize the read complete flag */
    appData.isWriteComplete                 = true;                                         /*Initialize the write complete flag*/
    appData.sofEventHasOccurred             = false;                                    /* Reset other flags */
    appData.readBuffer                      = &read_buffer[0];                                    /* Set up the read buffer */       
}


//------------------------------------------------------------------------------
// APP_Tasks()
//------------------------------------------------------------------------------
void APP_Tasks(void)
{    
    DRV_USBHS_Tasks( sysObj.drvUSBObject );   //USB HS Driver Task Routine
    USB_DEVICE_Tasks( sysObj.usbDevObject0 ); //USB Device layer tasks routine
    
    // Update the application state machine based on the current state 
    int i; 
    switch( appData.state ) {
        
        case APP_STATE_INIT:
            // Open the device layer
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

            if ( appData.deviceHandle != USB_DEVICE_HANDLE_INVALID ) {
                // Register a callback with device layer to get event notification (for end point 0)
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            } else {
                // The Device Layer is not ready to be opened. We should try again later.
            }
            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            // Check if the device was configured
            if ( appData.isConfigured ) {
                // If the device is configured then lets start reading
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            break;

        case APP_STATE_SCHEDULE_READ:

            if( APP_StateReset() ) {
                break;
            }

            // If a read is complete, then schedule a read else wait for the current read to complete

            appData.state = APP_STATE_WAIT_FOR_READ_COMPLETE;
            if ( appData.isReadComplete == true ) {
                appData.isReadComplete = false;
                appData.readTransferHandle =  USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

                USB_DEVICE_CDC_Read (USB_DEVICE_CDC_INDEX_0, &appData.readTransferHandle, appData.readBuffer, APP_READ_BUFFER_SIZE);
                
                if ( appData.readTransferHandle == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID ){
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:

            if ( APP_StateReset() ) {
                break;
            }

            if ( appData.isReadComplete ) {
                for ( i=0; i<appData.numBytesRead; i++ ) {
                    ds30_fifo_push( &usb_rx_fifo, appData.readBuffer[i] );
                    //#warning "test code"
                    //uart_send_byte( appData.readBuffer[i] );
                }
                
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            break;
            
        case APP_STATE_IDLE:
            break;


        /*case APP_STATE_SCHEDULE_WRITE:

            if ( APP_StateReset() ) {
                break;
            }

            // Setup the write
            appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            appData.isWriteComplete = false;
            appData.state = APP_STATE_WAIT_FOR_WRITE_COMPLETE; 
            
            USB_DEVICE_CDC_Write( USB_DEVICE_CDC_INDEX_0, &appData.writeTransferHandle, appData.readBuffer, appData.numBytesRead, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE );
            break;*/

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            if ( APP_StateReset() ) {
                break;
            }

            // Check if a character was sent. The isWriteComplete flag gets updated in the CDC event handler
            if ( appData.isWriteComplete == true ) {
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            break;

        case APP_STATE_ERROR:
            break;
            
        default:
            break;
    }
}


//------------------------------------------------------------------------------
// usb_read_byte()
//------------------------------------------------------------------------------
uint8_t usb_data_available(void)
{
    APP_Tasks();
    return ( usb_rx_fifo.count > 0 ? 1 : 0 );
}// usb_read_byte()


//------------------------------------------------------------------------------
// usb_exit()
//------------------------------------------------------------------------------
void usb_exit(void)
{
    USB_DEVICE_Detach( appData.deviceHandle );
}// usb_exit()


//------------------------------------------------------------------------------
// usb_init()
//------------------------------------------------------------------------------
void usb_init(void)
{
    ds30_fifo_init( &usb_rx_fifo );
    APP_Initialize();
    void APP_Tasks();
    void APP_Tasks();
    void APP_Tasks();
}// usb_init()


//------------------------------------------------------------------------------
// usb_read_byte()
//------------------------------------------------------------------------------
uint8_t usb_read_byte( uint8_t* const result )
{		
	uint8_t byte;
	
	*result = 1;
	
	if ( first_read ) {
		first_read = 0;
		timer_start_ms( HELLO_TIMEOUT );
	} else {
		timer_start_ms( COMM_TIMEOUT );	
	}
    
	do {	
        APP_Tasks();
		
		CLEAR_WDT();

		if ( timer_timeout() ) {
			return 0;
		}
	} while ( usb_rx_fifo.count == 0 );
	
	*result = 0;
	return ds30_fifo_pop( &usb_rx_fifo );
}// usb_read_byte()
		
			
//------------------------------------------------------------------------------
// wait_tx_ready()
//------------------------------------------------------------------------------
static uint8_t wait_tx_ready(void)
{
	timer_start_ms( COMM_TIMEOUT );
    while ( !appData.isWriteComplete ) {   
        APP_Tasks();           

        CLEAR_WDT();
        if ( timer_timeout() ) {
            return 1;
        }
    }
    return 0;
}// wait_tx_ready()
		
			
//------------------------------------------------------------------------------
// usb_send_bytes()
//------------------------------------------------------------------------------
void usb_send_bytes( const uint8_t* data, const uint16_t count ) 
{
	uint16_t bytes_left = count;
    uint16_t i;
	do {
		uint16_t bytes_to_send = MIN( bytes_left, MAX_TX );
		
        if ( wait_tx_ready() ) {
            return;
        }
        
		appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.isWriteComplete     = false;
        appData.state               = APP_STATE_WAIT_FOR_WRITE_COMPLETE; 
        
        for ( i=0; i<count; i++ ) {
            write_buffer[i] = *data++;
        }
        USB_DEVICE_CDC_Write( USB_DEVICE_CDC_INDEX_0, &appData.writeTransferHandle, write_buffer, count, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE );            
        
        if ( wait_tx_ready() ) {
            return;
        }
		
		bytes_left -= bytes_to_send;
		data += bytes_to_send;
	} while ( bytes_left > 0 );
}// usb_send_bytes()


//------------------------------------------------------------------------------
// usb_send_byte()
//------------------------------------------------------------------------------
void usb_send_byte( const uint8_t byte )
{
	usb_send_bytes( &byte, 1 );
}// usb_send_byte()
		
			
//------------------------------------------------------------------------------
// usb_send_string()
//------------------------------------------------------------------------------
void usb_send_string( const char* str )
{
	usb_send_bytes( (uint8_t*)str, strlen(str) );
}// usb_send_string()