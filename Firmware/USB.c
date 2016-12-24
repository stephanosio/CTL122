/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>
#include <stdint.h>

#include <usb/usb.h>
#include <usb/usb_device_hid.h>

#include "USB.h"
#include "PS2.h"
#include "Keyboard.h"
#include "UI.h"
#include "Util.h"


/******************************************************************************/
/* Static Variables                                                           */
/******************************************************************************/
USBHID_CONTEXT USBHIDContext;

#if !defined(KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
KEYBOARD_INPUT_REPORT inputReport KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;

#if !defined(KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
volatile KEYBOARD_OUTPUT_REPORT outputReport KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;

// Transmit Buffer
KEYBOARD_INPUT_REPORT USBHID_TxBuf[_USBHID_TXBUFLEN];
unsigned int USBHID_TxBufIdx = 0;
unsigned int USBHID_TxBufCnt = 0;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void USBHIDInit(void)
{
    // Initialise the variable holding the handle for the last transmission
    USBHIDContext.lastINTransmission = 0;

    // Enable the HID endpoint
    USBEnableEndpoint(HID_EP,
            USB_IN_ENABLED |
            USB_OUT_ENABLED |
            USB_HANDSHAKE_ENABLED |
            USB_DISALLOW_SETUP);

    // Arm OUT endpoint for LED states info from the host
    USBHIDContext.lastOUTTransmission = HIDRxPacket(HID_EP,
            (uint8_t*)&outputReport,
            sizeof(outputReport));
}

void USBHIDProcess(void)
{
    /* Check if the IN endpoint is busy, and if it isn't check if we want to send
     * keystroke data to the host. */
    if (HIDTxHandleBusy(USBHIDContext.lastINTransmission) == false)
    {
        // If the transmit queue is not empty
        if (GetTxCountUSBHID() > 0)
        {
            // Dequeue
            DequeueTxUSBHID(&inputReport);
            // Transmit over USB
            USBHIDContext.lastINTransmission = HIDTxPacket(HID_EP,
                (uint8_t *)&inputReport,
                sizeof(inputReport));
        }
    }

    /* Check if any data was sent from the PC to the keyboard device.  Report
     * descriptor allows host to send 1 byte of data.  Bits 0-4 are LED states,
     * bits 5-7 are unused pad bits.  The host can potentially send this OUT
     * report data through the HID OUT endpoint (EP1 OUT), or, alternatively,
     * the host may try to send LED state information by sending a SET_REPORT
     * control transfer on EP0.  See the USBHIDCBSetReportHandler() function. */
    if (HIDRxHandleBusy(USBHIDContext.lastOUTTransmission) == false)
    {
        USBHIDContext.lastOUTTransmission = HIDRxPacket(HID_EP,
                (uint8_t *)&outputReport, sizeof(outputReport));

        // Update keyboard LEDs
        UI_LED_NUMLK = outputReport.leds.numLock;
        UI_LED_CAPSLK = outputReport.leds.capsLock;
        UI_LED_SCRLLK = outputReport.leds.scrollLock;
    }
}

void USBHIDCBSetReportHandler(void)
{
    USBEP0Receive((uint8_t *)&CtrlTrfData,
            USB_EP0_BUFF_SIZE,
            USBHIDCBSetReportComplete);
}

void USBHIDCBSetReportComplete(void)
{
    /* 1 byte of LED state data should now be in the CtrlTrfData buffer.  Copy
     * it to the OUTPUT report buffer for processing */
    outputReport.value = CtrlTrfData[0];
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch((int)event)
    {
        case EVENT_CONFIGURED:
            USBHIDInit();
            break;
        case EVENT_EP0_REQUEST:
            USBCheckHIDRequest();
            break;
        case EVENT_BUS_ERROR:
            SetUIMessage("F.A.I.L.");
            break;
        default:
            break;
    }
    return true;
}

void EnqueueTxUSBHID(const KEYBOARD_INPUT_REPORT *elem)
{
    unsigned int end = (USBHID_TxBufIdx + USBHID_TxBufCnt) % _USBHID_TXBUFLEN;

    memcpy(&USBHID_TxBuf[end], elem, sizeof(KEYBOARD_INPUT_REPORT));

    if (USBHID_TxBufCnt == _USBHID_TXBUFLEN)
        USBHID_TxBufIdx = (USBHID_TxBufIdx + 1) % _USBHID_TXBUFLEN;
    else
        USBHID_TxBufCnt++;
}

void DequeueTxUSBHID(KEYBOARD_INPUT_REPORT *elem)
{
    memcpy(elem, &USBHID_TxBuf[USBHID_TxBufIdx], sizeof(KEYBOARD_INPUT_REPORT));

    USBHID_TxBufIdx = (USBHID_TxBufIdx + 1) % _USBHID_TXBUFLEN;
    USBHID_TxBufCnt--;
}

unsigned int GetTxCountUSBHID(void)
{
    return USBHID_TxBufCnt;
}

int IsTxFullUSBHID(void)
{
    return USBHID_TxBufCnt == _USBHID_TXBUFLEN;
}
