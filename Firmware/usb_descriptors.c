// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>

#include <usb/usb.h>
#include <usb/usb_device_hid.h>


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR device_dsc =
{
    0x12,                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    MY_VID,                 // Vendor ID
    MY_PID,                 // Product ID: Keyboard fw demo
    0x0001,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const uint8_t configDescriptor1[] = {
    /* Configuration Descriptor */
    0x09, //sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,   // CONFIGURATION descriptor type
    DESC_CONFIG_WORD(0x0029),       // Total length of data for this cfg
    1,                              // Number of interfaces in this cfg
    1,                              // Index value of this configuration
    0,                              // Configuration string index
    _DEFAULT | _SELF | _RWU,        // Attributes, see usb_device.h
    50,                             // Max power consumption (2X mA)

    /* Interface Descriptor */
    0x09, //sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type
    0,                              // Interface Number
    0,                              // Alternate Setting Number
    2,                              // Number of endpoints in this intf
    HID_INTF,                       // Class code
    BOOT_INTF_SUBCLASS,             // Subclass code
    HID_PROTOCOL_KEYBOARD,          // Protocol code
    0,                              // Interface string index

    /* HID Class-Specific Descriptor */
    0x09, //sizeof(USB_HID_DSC)+3,  // Size of this descriptor in bytes RRoj hack
    DSC_HID,                        // HID descriptor type
    DESC_CONFIG_WORD(0x0111),       // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,                 // Number of class descriptors, see usbcfg.h
    DSC_RPT,                        // Report descriptor type
    DESC_CONFIG_WORD(63), //sizeof(hid_rpt01),
                                    // Size of the report descriptor

    /* Endpoint Descriptor */
    0x07, /*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    HID_EP | _EP_IN,                // EndpointAddress
    _INTERRUPT,                     // Attributes
    DESC_CONFIG_WORD(8),            // size
    0x01,                           // Interval

    /* Endpoint Descriptor */
    0x07, /*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    HID_EP | _EP_OUT,               // EndpointAddress
    _INTERRUPT,                     // Attributes
    DESC_CONFIG_WORD(8),            // size
    0x01                            // Interval

};

// Language code string descriptor
const struct {
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[1];
} sd000 = {
    sizeof(sd000),
    USB_DESCRIPTOR_STRING,
    { 0x0409 }
};

// Manufacturer string descriptor
const struct {
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[12];
} sd001 = {
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {
        'S', 't', 'e', 'p', 'h', 'a', 'n', 'o', 's', ' ', 'I', 'o'
    }
};

// Product string descriptor
const struct {
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[41];
} sd002 = {
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {
        'C', 'T', 'L', '1', '2', '2', ' ',
        'P', 'r', 'o', 'g', 'r', 'a', 'm', 'm', 'a', 'b', 'l', 'e', ' ',
        'P', 'S', '/', '2', '-', 't', 'o', '-', 'U', 'S', 'B', ' ',
        'C', 'o', 'n', 'v', 'e', 'r', 't', 'e', 'r'
    }
};

// Array of configuration descriptors
const uint8_t *const USB_CD_Ptr[] =
{
    (const uint8_t *const)&configDescriptor1
};

// Array of string descriptors
const uint8_t *const USB_SD_Ptr[] =
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};

// Class specific descriptor - HID Keyboard
const struct {
    uint8_t report[HID_RPT01_SIZE];
} hid_rpt01 = { {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // End Collection
} };

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
 End of File
*/
