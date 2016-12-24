/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>            /* XC8 General Include File */
#include <plib/capture.h>
#include <plib/timers.h>
#include <plib/usart.h>

#include <usb/usb.h>
#include <usb/usb_device_hid.h>

#include "Init.h"
#include "Port.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void InitIO(void)
{
    // Enable PS/2 inputs
    TRISCbits.TRISC2 = 1; // PS2_CLK
    TRISCbits.TRISC1 = 1; // PS2_DATA
    // Enable indicator LED outputs
    TRISAbits.TRISA0 = 0;
    LED_PWR = 0;
    TRISAbits.TRISA1 = 0;
    LED_CFG = 0;
    TRISAbits.TRISA2 = 0;
    LED_MEM = 0;
    TRISAbits.TRISA3 = 0;
    LED_NUMLK = 0;
    TRISAbits.TRISA4 = 0;
    LED_CAPSLK = 0;
    TRISAbits.TRISA5 = 0;
    LED_SCRLLK = 0;
    // Enable 7-segment display outputs
    TRISDbits.TRISD0 = 0; // A
    DISP_A = 0;
    TRISDbits.TRISD1 = 0; // B
    DISP_B = 0;
    TRISDbits.TRISD2 = 0; // C
    DISP_C = 0;
    TRISDbits.TRISD3 = 0; // D
    DISP_D = 0;
    TRISDbits.TRISD4 = 0; // E
    DISP_E = 0;
    TRISDbits.TRISD5 = 0; // F
    DISP_F = 0;
    TRISDbits.TRISD6 = 0; // G
    DISP_G = 0;
    TRISDbits.TRISD7 = 0; // P
    DISP_P = 0;
    TRISBbits.TRISB0 = 0; // EN1
    DISP_EN1 = 0;
    TRISBbits.TRISB1 = 0; // EN2
    DISP_EN2 = 0;
    TRISBbits.TRISB2 = 0; // EN3
    DISP_EN3 = 0;
    TRISBbits.TRISB3 = 0; // EN4
    DISP_EN4 = 0;
}

void InitTimer(void)
{
    // Initialise Timer 0
    // Set clock source to internal instruction clock
    T0CONbits.T0CS = 0;
    // Set prescaler bypass (NO)
    T0CONbits.T0PS = 0b100;
    T0CONbits.PSA = 0;
    // Set 8-bit counter
    T0CONbits.T08BIT = 0;
    // Enable interrupts
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    // Enable timer
    T0CONbits.TMR0ON = 1;

    // Initialise Timer 2
    OpenTimer2(TIMER_INT_ON & T2_PS_1_1);
}

void InitCapture(void)
{
    // Initialise Capture 1
    OpenCapture1(C1_EVERY_FALL_EDGE & CAPTURE_INT_ON);
    // Initialise the Timer 1
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_EXT & T1_PS_1_1 & T1_16BIT_RW);
    WriteTimer1(0x0000);
}

void InitEUSART(void)
{
    // Initialise I/O ports for EUSART
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;
    // Open EUSART
    OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_BRGH_HIGH, 25);
}

void InitUSB(void)
{
    USBDeviceInit();
    USBDeviceAttach();
}
