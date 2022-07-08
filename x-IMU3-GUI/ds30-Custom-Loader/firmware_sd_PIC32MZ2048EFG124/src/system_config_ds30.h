
#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H


#include "driver/usb/usbhs/drv_usbhs_ds30.h"




typedef struct
{
    SYS_MODULE_OBJ  drvUSBObject;    
    SYS_MODULE_OBJ  usbDevObject0;
} SYSTEM_OBJECTS;

extern SYSTEM_OBJECTS sysObj;

#define DRV_USBHS_DEVICE_SUPPORT                true    /* Enables Device Support */
#define DRV_USBHS_HOST_SUPPORT                  false   /* Disable Host Support */
#define DRV_USBHS_INSTANCES_NUMBER              1       /* Maximum USB driver instances */
#define DRV_USBHS_INTERRUPT_MODE                false   /* Interrupt mode Disabled */
#define DRV_USBHS_ENDPOINTS_NUMBER              3       /* Number of Endpoints used */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT           /* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_INSTANCES_NUMBER             1       /* Maximum device layer instances */
#define USB_DEVICE_EP0_BUFFER_SIZE              64      /* EP0 size in bytes */
#define USB_DEVICE_SOF_EVENT_ENABLE                     /* Enable SOF Events */ 
#define USB_DEVICE_CDC_INSTANCES_NUMBER         1       /* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED     3       /* CDC Transfer Queue Size for both read and write. Applicable to all instances of the function driver */
#define APP_READ_BUFFER_SIZE                    512     /* Application USB Device CDC Read Buffer Size. This should be a multiple of * the CDC Bulk Endpoint size */
#define APP_WRITE_BUFFER_SIZE                   512     

#define APP_MAKE_BUFFER_DMA_READY  __attribute__((coherent)) __attribute__((aligned(16)))   /* Macro defines USB internal DMA Buffer criteria*/



void APP_Initialize ( void );
void APP_Tasks ( void );

#endif // _SYSTEM_CONFIG_H
