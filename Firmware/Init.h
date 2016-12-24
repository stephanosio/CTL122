#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

    void InitIO(void);
    void InitTimer(void);
    void InitCapture(void);
    void InitEUSART(void);
    void InitUSB(void);

#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

