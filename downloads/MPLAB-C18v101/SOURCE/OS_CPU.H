/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                           (c) Copyright 2002, Nathan Brown, Longview, TX
*                                      http://www.sputnickonline.com/
*                                          All Rights Reserved
*
*                                       PIC18xxxx Specific Code
*                                       V1.00 - October 27, 2002
*
* File : OS_CPU.H
* By   : Nathan Brown
*********************************************************************************************************
*/

// Interrupt handling - define in user code.
void CPUlowInterruptHook(void);
void CPUhighInterruptHook(void);


#define  OS_STK_GROWTH        0                  /* Define stack growth: 1 = Down, 0 = Up              */

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

/* Microchip PIC18xxx specific */
typedef signed short long    INT24S;             /* Signed   24 bit quantity                           */
typedef unsigned short long  INT24U;             /* Unsigned 24 bit quantity                           */
/* End Microchip PIC18xxx specific */

typedef INT8U          OS_STK;                   /* Each stack entry is 8-bit wide                    */
typedef INT8U          OS_CPU_SR;                /* Size of Processor Status Word (PSW)                */

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16S                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U


/*
**********************************************************************************************************
*                                       MACROS FOR BYTE AND WORD SELECTION
*
* Needed for some of the OS specific funcions as the stack width and the address widths are different.
**********************************************************************************************************
*/

#define LOWORD(l)           (*( (INT16U*)(&l)))
#define HIWORD(l)           (*( ( (INT16U*) (&l) ) + 1 ))
#define LOBYTE(w)           (*( (INT8U*) (&w)))
#define HIBYTE(w)           (*( ( (INT8U*)  (&w) ) + 1 ))
#define UPBYTE(w)           (*( ( (INT8U*)  (&w) ) + 2 ))



/*
*********************************************************************************************************
*                                        CRITICAL SECTION CONTROL
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*             Not supported on a PIC18xxx
*
* Method #3:  Disable/Enable interrupts by saving the value of the PSW into a local variable.  After
*             the critical section the variable can then be used to restore the PSW.
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD    3

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  INTCON &= 0b00111111
#define  OS_EXIT_CRITICAL()   INTCON |= 0b11000000
#endif

#if      OS_CRITICAL_METHOD == 2
#error  "OS_CFG.H, OS_CRITICAL_METHOD=2 is not supported in this PIC UCOS port."
#define  OS_ENTER_CRITICAL()
#define  OS_EXIT_CRITICAL()
#endif

#if      OS_CRITICAL_METHOD == 3
// OS_ENTER_CRITICAL disables all interrupts in the old one level mode, and 
// disables low priority interrupts in two priority level mode.
#define  OS_ENTER_CRITICAL()            \
         cpu_sr = INTCON & 0b11000000;  \
         INTCON &= (0b00111111 | (RCON & 0b10000000))

// OS_ENTER_CRITICAL_HIGH disables both high and low level priority interrupts.
#define  OS_ENTER_CRITICAL_HIGH()		\
		 cpu_sr = INTCON & 0b11000000;	\
		 INTCON &= 0b00111111

// OS_EXIT_CRITICAL restores the interrupt register with the previous configuration
#define  OS_EXIT_CRITICAL()            \
         INTCON |= cpu_sr;
#endif


/*
*********************************************************************************************************
*                                        TASK CONTEXT SWITCH
*
* Switch the context by calling the function directly, since there are no software interrupts in
* a PIC18 processor.
*********************************************************************************************************
*/
void OSCtxSw(void);

#define  OS_TASK_SW()         OSCtxSw()

/* end */
