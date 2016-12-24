/******************************************************************************/
/* Header Include                                                             */
/******************************************************************************/

#include <xc.h>            /* XC8 General Include File */

#include <usb/usb.h>

#include "Keyboard.h"
#include "PS2.h"
#include "USB.h"
#include "UI.h"
#include "Util.h"


/******************************************************************************/
/* Static Variables                                                           */
/******************************************************************************/
const char Kbd_TransTbl[256] = {
//  0     1     2     3     4     5     6     7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x3A,   // 0
    0x68, 0x00, 0x00, 0x00, 0x00, 0x2B, 0x35, 0x3B,   // 1
    0x69, 0x00, 0x00, 0x64, 0x00, 0x14, 0x1E, 0x3C,   // 2
    0x6A, 0x00, 0x1D, 0x16, 0x04, 0x1A, 0x1F, 0x3D,   // 3
    0x6B, 0x06, 0x1B, 0x07, 0x08, 0x21, 0x20, 0x3E,   // 4
    0x6C, 0x2C, 0x19, 0x09, 0x17, 0x15, 0x22, 0x3F,   // 5
    0x6D, 0x11, 0x05, 0x0B, 0x0A, 0x1C, 0x23, 0x40,   // 6
    0x6E, 0x00, 0x10, 0x0D, 0x18, 0x24, 0x25, 0x41,   // 7
    0x6F, 0x36, 0x0E, 0x0C, 0x12, 0x27, 0x26, 0x42,   // 8
    0x70, 0x37, 0x38, 0x0F, 0x33, 0x13, 0x2D, 0x43,   // 9
    0x71, 0x00, 0x34, 0x31, 0x2F, 0x2E, 0x44, 0x72,   // 10
    0x00, 0x00, 0x28, 0x30, 0x00, 0x00, 0x45, 0x73,   // 11
    0x51, 0x50, 0x00, 0x52, 0x4C, 0x4D, 0x2A, 0x49,   // 12
    0x00, 0x59, 0x4F, 0x5C, 0x5F, 0x4E, 0x4A, 0x4B,   // 13
    0x62, 0x63, 0x5A, 0x5D, 0x5E, 0x60, 0x53, 0x54,   // 14
    0x00, 0x58, 0x5B, 0x00, 0x57, 0x61, 0x55, 0x00,   // 15
    0x00, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00,   // 16
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 17
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 18
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 19
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 20
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 21
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 22
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 24
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 25
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 26
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 27
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 28
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 29
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 30
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    // 31
};

int Kbd_Mode = KBD_MODE_THRU;
int Kbd_SubMode;
int Kbd_LCtrlDown = FALSE;
int Kbd_LAltDown = FALSE;
int Kbd_LShiftDown = FALSE;
int Kbd_LWinDown = FALSE;
int Kbd_RCtrlDown = FALSE;
int Kbd_RAltDown = FALSE;
int Kbd_RShiftDown = FALSE;

int Kbd_Config_F1324WinProgCombo = TRUE;


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void ProcessKbd(void)
{
    /* NOTE: ProcessKbd function processes and translates the PS/2 inputs
     *       into USB outputs. Note that the purpose of ProcessKbd is not just
     *       simple translation, but an advanced macro and function processing
     *       as well.
    */
    char ps2ScanCode;
    
    // If PS/2 receive queue is not empty
    if (GetRxCountPS2() > 0)
    {
        // Dequeue PS/2 scancode
        ps2ScanCode = DequeueRxPS2();

        // Process scancode
        // * Through Mode *
        if (Kbd_Mode == KBD_MODE_THRU)
        {
            switch (ps2ScanCode)
            {
                /******************************************************************/
                /* Special Keys                                                   */
                /******************************************************************/
                case 0x11: // LCtrl
                    Kbd_LCtrlDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x19: // LAlt
                    Kbd_LAltDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x12: // LShift
                    Kbd_LShiftDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x58: // RCtrl
                    Kbd_RCtrlDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x39: // RAlt
                    Kbd_RAltDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x59: // RShift
                    Kbd_RShiftDown = TRUE; EnqueueSpecialKeyUSBHID(); break;
                case 0x14: // CapsLk
                    EnqueueSingleKeyUSBHID(0x39); break;
                case 0x06: // WinKey
                    if (Kbd_LCtrlDown == TRUE)
                    {
                        Kbd_LCtrlDown = FALSE;
                        SetUIMessage("W I N   ");
                        Kbd_Mode = KBD_MODE_WIN;
                        Kbd_SubMode = KBD_SUBMODE_WIN_WIN;
                    }
                    else
                    {
                        Kbd_LWinDown = TRUE;
                        EnqueueSpecialKeyUSBHID();
                        Kbd_LWinDown = FALSE;
                        EnqueueSpecialKeyUSBHID();
                    }
                    break;
                case 0x83: // Explorer
                    Kbd_LWinDown = TRUE;
                    EnqueueSingleKeyUSBHID(0x08);
                    Kbd_LWinDown = FALSE;
                    EnqueueSpecialKeyUSBHID();
                    break;
                case 0x0A: // Exec
                    Kbd_LWinDown = TRUE;
                    EnqueueSingleKeyUSBHID(0x15);
                    Kbd_LWinDown = FALSE;
                    EnqueueSpecialKeyUSBHID();
                    break;
                case 0x01: // ConfigKey
                    SetUIMessage("C F G   ");
                    Kbd_Mode = KBD_MODE_CFG;
                    break;
                case 0x09: // MemKey
                    SetUIMessage("M E M   ");
                    Kbd_Mode = KBD_MODE_MEM;
                    break;
                case 0xF0: // (Key Break)
                    Kbd_Mode = KBD_MODE_WAITBREAK; break;
                default:
                    if (Kbd_Config_F1324WinProgCombo == TRUE)
                    {
            /******************************************************************/
            /* F13-F24 Windows Program Combo Processing                       */
            /******************************************************************/
                        Kbd_LWinDown = TRUE;
                        switch (ps2ScanCode)
                        {
                            case 0x08: // F13 (WinKey+1)
                                EnqueueSingleKeyUSBHID(0x1E); break;
                            case 0x10: // F14 (WinKey+2)
                                EnqueueSingleKeyUSBHID(0x1F); break;
                            case 0x18: // F15 (WinKey+3)
                                EnqueueSingleKeyUSBHID(0x20); break;
                            case 0x20: // F16 (WinKey+4)
                                EnqueueSingleKeyUSBHID(0x21); break;
                            case 0x28: // F17 (WinKey+5)
                                EnqueueSingleKeyUSBHID(0x22); break;
                            case 0x30: // F18 (WinKey+6)
                                EnqueueSingleKeyUSBHID(0x23); break;
                            case 0x38: // F19 (WinKey+7)
                                EnqueueSingleKeyUSBHID(0x24); break;
                            case 0x40: // F20 (WinKey+8)
                                EnqueueSingleKeyUSBHID(0x25); break;
                            case 0x48: // F21 (WinKey+9)
                                EnqueueSingleKeyUSBHID(0x26); break;
                            case 0x50: // F22 (WinKey+0)
                                EnqueueSingleKeyUSBHID(0x27); break;
                            default:
                                Kbd_LWinDown = FALSE;
                                goto ProcessStandardTranslation;
                        }
                        Kbd_LWinDown = FALSE;
                        EnqueueSpecialKeyUSBHID();
                    }
                    else
                    {
            /******************************************************************/
            /* Standard Translation                                           */
            /******************************************************************/
                    ProcessStandardTranslation:
                        EnqueueSingleKeyUSBHID(
                                Kbd_TransTbl[ps2ScanCode]
                                );
                    }
                    break;
            }
        }
        // * Wait Break Mode *
        else if (Kbd_Mode == KBD_MODE_WAITBREAK)
        {
            switch (ps2ScanCode)
            {
                case 0x11: // LCtrl
                    Kbd_LCtrlDown = FALSE; break;
                case 0x19: // LAlt
                    Kbd_LAltDown = FALSE; break;
                case 0x12: // LShift
                    Kbd_LShiftDown = FALSE; break;
                case 0x58: // RCtrl
                    Kbd_RCtrlDown = FALSE; break;
                case 0x39: // RAlt
                    Kbd_RAltDown = FALSE; break;
                case 0x59: // RShift
                    Kbd_RShiftDown = FALSE; break;
                case 0x14: // CapsLk
                    break;
            }
            Kbd_Mode = KBD_MODE_THRU;
            EnqueueSpecialKeyUSBHID();
        }
        // * Windows Key Mode *
        else if (Kbd_Mode == KBD_MODE_WIN)
        {
            if (ps2ScanCode == 0x05) // Escape
                goto ExitWinMode;
            else if (ps2ScanCode == 0x06) // WinKey Repeat
            {
                switch (Kbd_SubMode)
                {
                    case KBD_SUBMODE_WIN_WIN:
                        Kbd_LShiftDown = TRUE;
                        SetUIMessage("W I S   ");
                        Kbd_SubMode = KBD_SUBMODE_WIN_WIS;
                        break;
                    case KBD_SUBMODE_WIN_WIS:
                        Kbd_LShiftDown = FALSE;
                        goto ExitWinMode;
                }
            }
            else
            {
                Kbd_LWinDown = TRUE;
                EnqueueSpecialKeyUSBHID();
                switch (ps2ScanCode)
                {
                    case 0x2D: // R (Run)
                        EnqueueSingleKeyUSBHID(0x15); goto ExitWinMode;
                    case 0x24: // E (Explorer)
                        EnqueueSingleKeyUSBHID(0x08); goto ExitWinMode;
                    case 0x23: // D (Desktop)
                        EnqueueSingleKeyUSBHID(0x07); goto ExitWinMode;
                    case 0x07: // F1 (WinKey+1)
                        EnqueueSingleKeyUSBHID(0x1E); break;
                    case 0x0F: // F2 (WinKey+2)
                        EnqueueSingleKeyUSBHID(0x1F); break;
                    case 0x17: // F3 (WinKey+3)
                        EnqueueSingleKeyUSBHID(0x20); break;
                    case 0x1F: // F4 (WinKey+4)
                        EnqueueSingleKeyUSBHID(0x21); break;
                    case 0x27: // F5 (WinKey+5)
                        EnqueueSingleKeyUSBHID(0x22); break;
                    case 0x2F: // F6 (WinKey+6)
                        EnqueueSingleKeyUSBHID(0x23); break;
                    case 0x37: // F7 (WinKey+7)
                        EnqueueSingleKeyUSBHID(0x24); break;
                    case 0x3F: // F8 (WinKey+8)
                        EnqueueSingleKeyUSBHID(0x25); break;
                    case 0x47: // F9 (WinKey+9)
                        EnqueueSingleKeyUSBHID(0x26); break;
                    case 0x4F: // F10 (WinKey+0)
                        EnqueueSingleKeyUSBHID(0x27); break;
                    case 0x5A: // Enter
                        EnqueueSingleKeyUSBHID(0x28); break;
                }
            }
            return;
ExitWinMode:
            Kbd_LWinDown = FALSE;
            Kbd_LShiftDown = FALSE;
            EnqueueSpecialKeyUSBHID();
            Kbd_Mode = KBD_MODE_THRU;
            SetUIMessage("T H R U ");
        }
        else if (Kbd_Mode == KBD_MODE_CFG)
        {
            switch (ps2ScanCode)
            {
                case 0x1D: // W (F13-F24 Windows Program Combo)
                    Kbd_Config_F1324WinProgCombo = !Kbd_Config_F1324WinProgCombo;
                    break;
            }
            Kbd_Mode = KBD_MODE_THRU;
            SetUIMessage("T H R U ");
        }
        else if (Kbd_Mode == KBD_MODE_MEM)
        {
            switch (ps2ScanCode)
            {
                case 0x07: // F1
                    EnqueueSingleKeyUSBHID(0x04 + 'm' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'a' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'c' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'r' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'o' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 't' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'e' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 's' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 't' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'm' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'e' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 's' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 's' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'a' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'g' - 'a');
                    EnqueueSingleKeyUSBHID(0x04 + 'e' - 'a');
                    break;
                case 0x0F: // F2
                    for (int i = 0; i < 26; i++)
                    {
                        EnqueueSingleKeyUSBHID(0x04 + i);
                        EnqueueSingleKeyUSBHID(0x28);
                    }
                    break;
            }
            Kbd_Mode = KBD_MODE_THRU;
            SetUIMessage("T H R U ");
        }
    }
}

void EnqueueSingleKeyUSBHID(char scanCode)
{
    KEYBOARD_INPUT_REPORT usbInReport;

    // Enqueue the input report for key down
    memset(&usbInReport, 0, sizeof(usbInReport));
    usbInReport.modifiers.bits.leftControl = Kbd_LCtrlDown;
    usbInReport.modifiers.bits.leftAlt = Kbd_LAltDown;
    usbInReport.modifiers.bits.leftShift = Kbd_LShiftDown;
    usbInReport.modifiers.bits.leftGUI = Kbd_LWinDown;
    usbInReport.modifiers.bits.rightControl = Kbd_RCtrlDown;
    usbInReport.modifiers.bits.rightAlt = Kbd_RAltDown;
    usbInReport.modifiers.bits.rightShift = Kbd_RShiftDown;
    usbInReport.keys[0] = scanCode;
    EnqueueTxUSBHID(&usbInReport);
    
    // Enqueue the input report for key up
    usbInReport.keys[0] = 0;
    EnqueueTxUSBHID(&usbInReport);
}

void EnqueueSpecialKeyUSBHID(void)
{
    KEYBOARD_INPUT_REPORT usbInReport;

    // Enqueue the input report for key down
    memset(&usbInReport, 0, sizeof(usbInReport));
    usbInReport.modifiers.bits.leftControl = Kbd_LCtrlDown;
    usbInReport.modifiers.bits.leftAlt = Kbd_LAltDown;
    usbInReport.modifiers.bits.leftShift = Kbd_LShiftDown;
    usbInReport.modifiers.bits.leftGUI = Kbd_LWinDown;
    usbInReport.modifiers.bits.rightControl = Kbd_RCtrlDown;
    usbInReport.modifiers.bits.rightAlt = Kbd_RAltDown;
    usbInReport.modifiers.bits.rightShift = Kbd_RShiftDown;
    EnqueueTxUSBHID(&usbInReport);
}
