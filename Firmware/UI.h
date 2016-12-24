#ifndef UI_H
#define	UI_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern char UI_LED_PWR;
    extern char UI_LED_CFG;
    extern char UI_LED_MEM;
    extern char UI_LED_NUMLK;
    extern char UI_LED_CAPSLK;
    extern char UI_LED_SCRLLK;
    extern char UI_DISP1_CHAR;
    extern char UI_DISP1_DOT;
    extern char UI_DISP2_CHAR;
    extern char UI_DISP2_DOT;
    extern char UI_DISP3_CHAR;
    extern char UI_DISP3_DOT;
    extern char UI_DISP4_CHAR;
    extern char UI_DISP4_DOT;

    void ProcessUI(void);
    void ProcessLED(void);
    void ProcessDisp(void);
    void SetDispChar(char letter);
    void SetUIMessage(const char *message);

#ifdef	__cplusplus
}
#endif

#endif	/* UI_H */

