#ifndef UTIL_H
#define	UTIL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FALSE       0
#define TRUE        1

#define _XTAL_FREQ      48000000

    void Delay(unsigned long count);
    void uDelay(unsigned long interval);
    void mDelay(unsigned long interval);
    char SwapBitOrder(char src);
    char NibbleToHex(char nibble);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

