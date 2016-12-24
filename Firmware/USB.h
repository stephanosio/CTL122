#ifndef USB_H
#define	USB_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG   @ 0x500
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG  @ 0x508

    #define _USBHID_TXBUFLEN                                100

    /* This typedef defines the only INPUT report found in the HID report
     * descriptor and gives an easy way to create the OUTPUT report. */
    typedef struct __attribute__((packed))
    {
        /* The union below represents the first byte of the INPUT report.  It is
         * formed by the following HID report items:
         *
         *  0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
         *  0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
         *  0x15, 0x00, //   LOGICAL_MINIMUM (0)
         *  0x25, 0x01, //   LOGICAL_MAXIMUM (1)
         *  0x75, 0x01, //   REPORT_SIZE (1)
         *  0x95, 0x08, //   REPORT_COUNT (8)
         *  0x81, 0x02, //   INPUT (Data,Var,Abs)
         *
         * The report size is 1 specifying 1 bit per entry.
         * The report count is 8 specifying there are 8 entries.
         * These entries represent the Usage items between Left Control (the usage
         * minimum) and Right GUI (the usage maximum).
         */
        union __attribute__((packed))
        {
            uint8_t value;
            struct __attribute__((packed))
            {
                unsigned leftControl    :1;
                unsigned leftShift      :1;
                unsigned leftAlt        :1;
                unsigned leftGUI        :1;
                unsigned rightControl   :1;
                unsigned rightShift     :1;
                unsigned rightAlt       :1;
                unsigned rightGUI       :1;
            } bits;
        } modifiers;

        /* There is one byte of constant data/padding that is specified in the
         * input report:
         *
         *  0x95, 0x01,                    //   REPORT_COUNT (1)
         *  0x75, 0x08,                    //   REPORT_SIZE (8)
         *  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
         */
        unsigned :8;

        /* The last INPUT item in the INPUT report is an array type.  This array
         * contains an entry for each of the keys that are currently pressed until
         * the array limit, in this case 6 concurent key presses.
         *
         *  0x95, 0x06,                    //   REPORT_COUNT (6)
         *  0x75, 0x08,                    //   REPORT_SIZE (8)
         *  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
         *  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
         *  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
         *  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
         *  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
         *
         * Report count is 6 indicating that the array has 6 total entries.
         * Report size is 8 indicating each entry in the array is one byte.
         * The usage minimum indicates the lowest key value (Reserved/no event)
         * The usage maximum indicates the highest key value (Application button)
         * The logical minimum indicates the remapped value for the usage minimum:
         *   No Event has a logical value of 0.
         * The logical maximum indicates the remapped value for the usage maximum:
         *   Application button has a logical value of 101.
         *
         * In this case the logical min/max match the usage min/max so the logical
         * remapping doesn't actually change the values.
         *
         * To send a report with the 'a' key pressed (usage value of 0x04, logical
         * value in this example of 0x04 as well), then the array input would be the
         * following:
         *
         * LSB [0x04][0x00][0x00][0x00][0x00][0x00] MSB
         *
         * If the 'b' button was then pressed with the 'a' button still held down,
         * the report would then look like this:
         *
         * LSB [0x04][0x05][0x00][0x00][0x00][0x00] MSB
         *
         * If the 'a' button was then released with the 'b' button still held down,
         * the resulting array would be the following:
         *
         * LSB [0x05][0x00][0x00][0x00][0x00][0x00] MSB
         *
         * The 'a' key was removed from the array and all other items in the array
         * were shifted down. */
        uint8_t keys[6];
    } KEYBOARD_INPUT_REPORT;

    /* This typedef defines the only OUTPUT report found in the HID report
     * descriptor and gives an easy way to parse the OUTPUT report. */
    typedef union __attribute__((packed))
    {
        /* The OUTPUT report is comprised of only one byte of data. */
        uint8_t value;
        struct
        {
            /* There are two report items that form the one byte of OUTPUT report
             * data.  The first report item defines 5 LED indicators:
             *
             *  0x95, 0x05,                    //   REPORT_COUNT (5)
             *  0x75, 0x01,                    //   REPORT_SIZE (1)
             *  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
             *  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
             *  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
             *  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
             *
             * The report count indicates there are 5 entries.
             * The report size is 1 indicating each entry is just one bit.
             * These items are located on the LED usage page
             * These items are all of the usages between Num Lock (the usage
             * minimum) and Kana (the usage maximum).
             */
            unsigned numLock        :1;
            unsigned capsLock       :1;
            unsigned scrollLock     :1;
            unsigned compose        :1;
            unsigned kana           :1;

            /* The second OUTPUT report item defines 3 bits of constant data
             * (padding) used to make a complete byte:
             *
             *  0x95, 0x01,                    //   REPORT_COUNT (1)
             *  0x75, 0x03,                    //   REPORT_SIZE (3)
             *  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
             *
             * Report count of 1 indicates that there is one entry
             * Report size of 3 indicates the entry is 3 bits long. */
            unsigned                :3;
        } leds;
    } KEYBOARD_OUTPUT_REPORT;

    /* This creates a storage type for all of the information required to track the
     * current state of the keyboard. */
    typedef struct
    {
        USB_HANDLE lastINTransmission;
        USB_HANDLE lastOUTTransmission;
    } USBHID_CONTEXT;


    void USBHIDInit(void);
    void USBHIDProcess(void);
    void USBHIDCBSetReportHandler(void);
    void USBHIDCBSetReportComplete(void);
    void EnqueueTxUSBHID(const KEYBOARD_INPUT_REPORT *elem);
    void DequeueTxUSBHID(KEYBOARD_INPUT_REPORT *elem);
    unsigned int GetTxCountUSBHID(void);
    int IsTxFullUSBHID(void);

    extern USBHID_CONTEXT USBHIDContext;
    extern KEYBOARD_INPUT_REPORT inputReport;
    extern volatile KEYBOARD_OUTPUT_REPORT outputReport;


#ifdef	__cplusplus
}
#endif

#endif	/* USB_H */
