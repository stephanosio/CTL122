#ifndef PORT_H
#define	PORT_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Port Definitions                                                           */
/******************************************************************************/
#define PS2_CLK     PORTCbits.RC2
#define PS2_DATA    PORTCbits.RC1

#define LED_PWR     LATAbits.LATA0
#define LED_CFG     LATAbits.LATA1
#define LED_MEM     LATAbits.LATA2
#define LED_NUMLK   LATAbits.LATA3
#define LED_CAPSLK  LATAbits.LATA4
#define LED_SCRLLK  LATAbits.LATA5

#define DISP_EN1    LATBbits.LATB0
#define DISP_EN2    LATBbits.LATB1
#define DISP_EN3    LATBbits.LATB2
#define DISP_EN4    LATBbits.LATB3
#define DISP_A      LATDbits.LATD0
#define DISP_B      LATDbits.LATD1
#define DISP_C      LATDbits.LATD2
#define DISP_D      LATDbits.LATD3
#define DISP_E      LATDbits.LATD4
#define DISP_F      LATDbits.LATD5
#define DISP_G      LATDbits.LATD6
#define DISP_P      LATDbits.LATD7


#ifdef	__cplusplus
}
#endif

#endif	/* PORT_H */

