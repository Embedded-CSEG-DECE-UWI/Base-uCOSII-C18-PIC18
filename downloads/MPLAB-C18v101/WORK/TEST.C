#include    "includes.h"
#include    <timers.h>
#include    <xlcd.h>
#include    <delays.h>
#include    <string.h>
#include    <stdlib.h>

static char rstr_welcome[17L] = "Welcome uC/OS-II";

OS_STK  TestTaskStk[100L];
OS_STK  LCDTaskStk[200L];

// functions required for XLCD
// min of 18 Tcy
void DelayFor18TCY(void)
{
    Delay10TCYx(2L);
}

// min of 15ms
void DelayPORXLCD(void)
{
    Delay1KTCYx(15L);
}

// min of 5ms
void DelayXLCD(void)
{
    Delay1KTCYx(5L);
}

void LCDTask(void *pdata)
{
    char    str_time[17];
    char    str_proc[4];
    INT32U  ui32_Time;
    INT8U   ui8_Proc;

    // Initialize the LCD
    OpenXLCD(FOUR_BIT & LINES_5X7 & LINES_FLIP);
    while(BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF); // display on
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
    WriteCmdXLCD(ENTRY_CURSOR_INC & ENTRY_DISPLAY_NO_SHIFT); // entrymode

    while(BusyXLCD());              // Wait if LCD busy
    SetDDRamAddr(0x0);                // Set Display data ram address to 0
    putsXLCD(rstr_welcome);

    for(;;)
    {
        ui32_Time = OSTimeGet();
        ultoa(ui32_Time, str_time);
        btoa(OSCPUUsage, str_proc);

        while(BusyXLCD());              // Wait if LCD busy
        SetDDRamAddr(0x40);             // Set Display data ram address to 0
        putsXLCD(str_time);

        while(BusyXLCD());              // Wait if LCD busy
        SetDDRamAddr(0x4E);             // Set Display data ram address to 0
        putsXLCD(str_proc);

        if(strlen(str_proc) < 2)        // If two characters wouldn't have been
        {                               // written, then write the second one.
            while(BusyXLCD());
            putcXLCD(' ');
        }

        OSTimeDly(10);
    }
}

void TestTask(void *pdata)
{
    int     i_data;

    // configure the input/output pins
    ADCON1 = 0b00001110;        // set the A/D register
    INTCON2 = 0b01111111;
    INTCON3 = 0b00000000;

    // enable interrupts
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
    WriteTimer0(-10000);

    // Initialize statistics task
    OSStatInit();

    // Start up other tasks
    OSTaskCreate(LCDTask, (void *)0, &LCDTaskStk[0], 1);

    PORTB = 0x00;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB2 = 0;

    i_data = *((int *)pdata);

    // task loop
    for(;;)
    {
        PORTBbits.RB3 = !PORTBbits.RB3;
        PORTBbits.RB2 = !PORTBbits.RB3;
        OSTimeDly(100);
    }
}

void main (void)
{
    int i_test = 0xABCD;
    INTCONbits.GIEH = 0;
    OSInit();
    OSTaskCreate(TestTask, (void *)&i_test, &TestTaskStk[0], 0);
    OSStart();
}

