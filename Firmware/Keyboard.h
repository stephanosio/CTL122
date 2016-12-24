#ifndef KEYBOARD_H
#define	KEYBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define KBD_MODE_THRU           0
#define KBD_MODE_WAITBREAK      1
#define KBD_MODE_WIN            2
#define KBD_MODE_CFG            3
#define KBD_MODE_MEM            4

#define KBD_SUBMODE_WIN_WIN     0
#define KBD_SUBMODE_WIN_WIS     1


    void ProcessKbd(void);
    void EnqueueSingleKeyUSBHID(char scanCode);
    void EnqueueSpecialKeyUSBHID(void);


#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */
