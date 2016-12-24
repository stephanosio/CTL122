/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>

#include "UI.h"
#include "Port.h"
#include "Util.h"

/******************************************************************************/
/* Static Variables                                                           */
/******************************************************************************/
char UI_LED_PWR = TRUE;
char UI_LED_CFG = FALSE;
char UI_LED_MEM = FALSE;
char UI_LED_NUMLK = FALSE;
char UI_LED_CAPSLK = FALSE;
char UI_LED_SCRLLK = FALSE;
char UI_DISP1_CHAR = '\0';
char UI_DISP1_DOT = FALSE;
char UI_DISP2_CHAR = '\0';
char UI_DISP2_DOT = FALSE;
char UI_DISP3_CHAR = '\0';
char UI_DISP3_DOT = FALSE;
char UI_DISP4_CHAR = '\0';
char UI_DISP4_DOT = FALSE;


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void ProcessUI(void)
{
    ProcessLED();
    ProcessDisp();
}

void ProcessLED(void)
{
    // Turn on LEDs
    LED_PWR = UI_LED_PWR;
    LED_CFG = UI_LED_CFG;
    LED_MEM = UI_LED_MEM;
    LED_NUMLK = UI_LED_NUMLK;
    LED_CAPSLK = UI_LED_CAPSLK;
    LED_SCRLLK = UI_LED_SCRLLK;

    mDelay(3);

    // Turn off
    LED_PWR = 0;
    LED_CFG = 0;
    LED_MEM = 0;
    LED_NUMLK = 0;
    LED_CAPSLK = 0;
    LED_SCRLLK = 0;
}

void ProcessDisp(void)
{
    // Display segment 1
    SetDispChar(UI_DISP1_CHAR);
    DISP_P = UI_DISP1_DOT;
    DISP_EN1 = 1;
    mDelay(5);
    DISP_EN1 = 0;

    // Display segment 2
    SetDispChar(UI_DISP2_CHAR);
    DISP_P = UI_DISP2_DOT;
    DISP_EN2 = 1;
    mDelay(5);
    DISP_EN2 = 0;

    // Display segment 3
    SetDispChar(UI_DISP3_CHAR);
    DISP_P = UI_DISP3_DOT;
    DISP_EN3 = 1;
    mDelay(5);
    DISP_EN3 = 0;

    // Display segment 4
    SetDispChar(UI_DISP4_CHAR);
    DISP_P = UI_DISP4_DOT;
    DISP_EN4 = 1;
    mDelay(5);
    DISP_EN4 = 0;
}

void SetDispChar(char letter)
{
    switch (letter)
    {
        case '0':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 0;
            break;
        case '1':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 0; DISP_F = 0; DISP_G = 0;
            break;
        case '2':
            DISP_A = 1; DISP_B = 1; DISP_C = 0; DISP_D = 1;
            DISP_E = 1; DISP_F = 0; DISP_G = 1;
            break;
        case '3':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 0; DISP_F = 0; DISP_G = 1;
            break;
        case '4':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 0; DISP_F = 1; DISP_G = 1;
            break;
        case '5':
            DISP_A = 1; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 0; DISP_F = 1; DISP_G = 1;
            break;
        case '6':
            DISP_A = 1; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case '7':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 0; DISP_F = 0; DISP_G = 0;
            break;
        case '8':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case '9':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 0; DISP_F = 1; DISP_G = 1;
            break;
        case 'A':
            DISP_A = 1; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'B':
            DISP_A = 0; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'C':
            DISP_A = 1; DISP_B = 0; DISP_C = 0; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 0;
            break;
        case 'D':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 0; DISP_G = 1;
            break;
        case 'E':
            DISP_A = 1; DISP_B = 0; DISP_C = 0; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'F':
            DISP_A = 1; DISP_B = 0; DISP_C = 0; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'G':
            DISP_A = 1; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 0;
            break;
        case 'H':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'I':
            DISP_A = 0; DISP_B = 0; DISP_C = 0; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 0;
            break;
        case 'M':
            DISP_A = 1; DISP_B = 0; DISP_C = 1; DISP_D = 0;
            DISP_E = 1; DISP_F = 0; DISP_G = 0;
            break;
        case 'N':
            DISP_A = 0; DISP_B = 0; DISP_C = 1; DISP_D = 0;
            DISP_E = 1; DISP_F = 0; DISP_G = 1;
            break;
        case 'O':
            DISP_A = 0; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 1; DISP_F = 0; DISP_G = 1;
            break;
        case 'P':
            DISP_A = 1; DISP_B = 1; DISP_C = 0; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'R':
            DISP_A = 0; DISP_B = 0; DISP_C = 0; DISP_D = 0;
            DISP_E = 1; DISP_F = 0; DISP_G = 1;
            break;
        case 'S':
            DISP_A = 1; DISP_B = 0; DISP_C = 1; DISP_D = 1;
            DISP_E = 0; DISP_F = 1; DISP_G = 1;
            break;
        case 'T':
            DISP_A = 0; DISP_B = 0; DISP_C = 0; DISP_D = 1;
            DISP_E = 1; DISP_F = 1; DISP_G = 1;
            break;
        case 'W':
            DISP_A = 0; DISP_B = 1; DISP_C = 0; DISP_D = 1;
            DISP_E = 0; DISP_F = 1; DISP_G = 0;
            break;
        case 'X':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 0;
            DISP_E = 1; DISP_F = 1; DISP_G = 0;
            break;
        case 'Y':
            DISP_A = 0; DISP_B = 1; DISP_C = 1; DISP_D = 1;
            DISP_E = 0; DISP_F = 1; DISP_G = 1;
            break;
        case '\0':
        default:
            DISP_A = 0; DISP_B = 0; DISP_C = 0; DISP_D = 0;
            DISP_E = 0; DISP_F = 0; DISP_G = 0;
            break;
    }
}

void SetUIMessage(const char *message)
{
    // Display segment 1
    UI_DISP1_CHAR = message[0];
    UI_DISP1_DOT = (message[1] == '.');

    // Display segment 2
    UI_DISP2_CHAR = message[2];
    UI_DISP2_DOT = (message[3] == '.');

    // Display segment 3
    UI_DISP3_CHAR = message[4];
    UI_DISP3_DOT = (message[5] == '.');

    // Display segment 4
    UI_DISP4_CHAR = message[6];
    UI_DISP4_DOT = (message[7] == '.');
}