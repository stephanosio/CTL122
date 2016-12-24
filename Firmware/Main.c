/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>            /* XC8 General Include File */
#include <plib/usart.h>

#include <stdio.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "Init.h"
#include "Port.h"
#include "UI.h"
#include "Serial.h"
#include "Util.h"

/******************************************************************************/
/* Configuration                                                              */
/******************************************************************************/

// Generic
#pragma config PWRT = OFF
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config VREGEN = ON

// Clock Subsystem
#pragma config FOSC = HSPLL_HS
#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LPT1OSC = OFF



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    // Initialise I/O
    InitIO();
    // Initialise capture
    InitCapture();
    // Initialise timer
    InitTimer();
    // Initialise EUSART
    InitEUSART();
    // Initialise USB
    InitUSB();

    // Print init. message
    printf("CTL122 Initialised\r\n");
    
    // Set standby UI state
    SetUIMessage("I N I T ");
    
    // Processing Loop
    while (TRUE)
    {
        ProcessUI();
    }
}
