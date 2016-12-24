#ifndef PS2_H
#define	PS2_H

#ifdef	__cplusplus
extern "C" {
#endif

#define _PS2_RXBUFLEN   32

#define PS2_RECV        0
#define PS2_TIMEOUT     1

    void ProcessPS2(int mode);
    void EnqueueRxPS2(char elem);
    char DequeueRxPS2(void);
    unsigned int GetRxCountPS2(void);
    int IsRxFullPS2(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PS2_H */
