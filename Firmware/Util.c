/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>

#include "Util.h"


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void Delay(unsigned long count)
{
    for (unsigned long i = 0; i < count; i++)
        asm("nop");
}

void uDelay(unsigned long interval)
{
    for (unsigned long i = 0; i < interval; i++)
        __delay_us(1);
}

void mDelay(unsigned long interval)
{
    for (unsigned long i = 0; i < interval; i++)
        __delay_ms(1);
}

char SwapBitOrder(char src)
{
    char dest = 0;
    for (int i = 0; i < 8; i++)
    {
        dest |= (0b00000001 & src) << (7 - i);
        src >>= 1;
    }
    return dest;
}

static const char CvtVector[] = "0123456789ABCDEF";
char NibbleToHex(char nibble)
{
    return CvtVector[nibble & 0b00001111];
}
