/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>

#include <usb/usb.h>

#include "UI.h"
#include "Util.h"
#include "PS2.h"
#include "Keyboard.h"
#include "USB.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void interrupt ISR()
{
    // Timer 0 Interrupt
    if (INTCONbits.TMR0IF == 1)
    {
        UI_LED_PWR = !UI_LED_PWR;
        INTCONbits.TMR0IF = 0;
    }
    // Timer 1 Interrupt
    else if (PIR1bits.TMR1IF == 1)
    {
        ProcessPS2(PS2_TIMEOUT);
        PIR1bits.TMR1IF = 0;
    }
    // Timer 2 Interrupt
    else if (PIR1bits.TMR2IF == 1)
    {
        ProcessKbd();
        USBHIDProcess();
        PIR1bits.TMR2IF = 0;
    }
    // Capture Module 1 Interrupt
    else if (PIR1bits.CCP1IF == 1)
    {
        ProcessPS2(PS2_RECV);
        PIR1bits.CCP1IF = 0;
    }
    // USB Interrupt
    else if (PIR2bits.USBIF == 1)
    {
        USBDeviceTasks();
        PIR2bits.USBIF = 0;
    }
}
