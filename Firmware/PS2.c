/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>
#include <plib/timers.h>

#include <stdio.h>
#include <stdint.h>

#include "PS2.h"
#include "Port.h"
#include "UI.h"
#include "Serial.h"
#include "Util.h"

/******************************************************************************/
/* Static Variables                                                           */
/******************************************************************************/
// Receive Sequence
unsigned int PS2_CurRx;
char PS2_CurRxIdx = 0;

// Receive Buffer
char PS2_RxBuf[_PS2_RXBUFLEN];
unsigned int PS2_RxBufIdx = 0;
unsigned int PS2_RxBufCnt = 0;


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void ProcessPS2(int mode)
{
    char temp;

    switch (mode)
    {
        // Receive Event
        case PS2_RECV:
            if (PS2_CurRxIdx < 10) // Append bits
            {
                PS2_CurRx <<= 1;
                PS2_CurRx |= PS2_DATA;
                PS2_CurRxIdx++;
            }
            else if (PS2_CurRxIdx == 10) // Stop bit
            {
                // Generate reordered temp. received byte
                temp = SwapBitOrder((char)(PS2_CurRx >> 1));
                // Verify the receive buffer state
                if (IsRxFullPS2() == FALSE)
                {
                    // Enqueue to the receive buffer
                    EnqueueRxPS2(temp);
                }
                else
                {
                    // Print error on 7 segment displays
                    SetUIMessage("P B O F ");
                }
                // Reset the receive sequence index
                PS2_CurRxIdx = 0;
            }
            WriteTimer1(0x0000);
            break;
        // Receive Timeout
        case PS2_TIMEOUT:
            PS2_CurRxIdx = 0;
            break;
    }
}

void EnqueueRxPS2(char elem)
{
    int end = (PS2_RxBufIdx + PS2_RxBufCnt) % _PS2_RXBUFLEN;

    PS2_RxBuf[end] = elem;

    if (PS2_RxBufCnt == _PS2_RXBUFLEN)
        PS2_RxBufIdx = (PS2_RxBufIdx + 1) % _PS2_RXBUFLEN;
    else
        PS2_RxBufCnt++;
}

char DequeueRxPS2(void)
{
    char elem;

    elem = PS2_RxBuf[PS2_RxBufIdx];

    PS2_RxBufIdx = (PS2_RxBufIdx + 1) % _PS2_RXBUFLEN;
    PS2_RxBufCnt--;

    return elem;
}

unsigned int GetRxCountPS2(void)
{
    return PS2_RxBufCnt;
}

int IsRxFullPS2(void)
{
    return PS2_RxBufCnt == _PS2_RXBUFLEN;
}
