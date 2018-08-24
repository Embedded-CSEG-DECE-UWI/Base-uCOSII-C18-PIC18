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
* File : OS_CPU_C.C
* By   : Nathan Brown
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.  For the TASKING compiler, 'pdata' is passed
*                            in R1:R0.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*
*              2) Save the current context onto the stack
*
*              3) The return addresses are in a different stack, so save all of those into the stack
*
*              4) The last stacking operation (before the return) places the number of return stack
*                 items in the software stack.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;

    stk     = ptos;                     // local pointer to our new proccess stack
    *stk++  = (OS_STK)*(((INT8U*)&pdata));  // data being passed
    *stk++  = (OS_STK)*(((INT8U*)&pdata)+1);
    *stk++  = 0xFF;                     // <-- FSR1 Pointer position on function call.
    *stk++  = 0x1A;                     // STATUS register
    *stk++  = 0x1B;                     // BSR register
    *stk++  = 0x1C;                     // W register
    *stk++  = 0x1D;                     // FSR0L register
    *stk++  = 0x1E;                     // FSR0H register
    *stk++  = 0x1F;                     // FSR2L register
    *stk++  = 0x2A;                     // FSR2H register
    *stk++  = 0x2B;                     // TBLPTRL Prog Mem Table Pointer Upper
    *stk++  = 0x2C;                     // TBLPTRH Prog Mem Table Pointer High
    *stk++  = 0x2D;                     // TBLPTRU Prog Mem Table Pointer Low
    *stk++  = 0x2E;                     // PRODL Product Reg high
    *stk++  = 0x2F;                     // PRODH Product Reg low
    *stk++  = 0x3A;						// AARGB3
    *stk++  = 0x3B;						// AARGB2
    *stk++  = 0x3C;						// AARGB1
    *stk++  = 0x3D;						// AARGB0

    // first return address, the task address, goes on the hardware return stack in a context switch
    *stk++  = (OS_STK)*(((INT8U*)&task));   // TOSL - Top of stack - Lower bits
    *stk++  = (OS_STK)*(((INT8U*)&task)+1); // TOSH - Top of stack - High bits
    *stk++  = (OS_STK)*(((INT8U*)&task)+2); // TOSU - Top of stack - Uppre bits
    // repeat...

    *stk++  = 0x01;                     // size of return stack

    return ((OS_STK *)stk);             // Return pointer to task's top-of-stack
}

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook(OS_TCB *ptcb)
{
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
	// Clear watch dog timer on context switches.  If enabled, the processor
	// will reset if this does not get called (application frozen).
_asm
	CLRWDT
_endasm
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}



/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook(struct os_tcb *ptcb)
{
}
#endif


/*
*********************************************************************************************************
*                                           IDLE TASK HOOK
*
* Description: This function is called by OS_TaskIdle() which is executed whenever no other task is
*              ready to run.  This function can put the CPU to sleep, ready to be woken by an interupt
*              or it can blink a LED to indicate how busy the processor is.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskIdleHook (void)
{
}


void OSTaskReturnHook (OS_TCB *p)
{
}

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                       START HIGHEST PRIORITY TASK
*
* Description : This function is called by OSStart() to start the highest priority task that was created
*               by your application before calling OSStart().
*
* Arguments   : none
*
* Note(s)     : 1)  Interupts are disabled when calling.
*
*               2) The stack frame (8-bit wide stack) is assumed to look as follows:
*
*                                                                                          LOW MEMORY
*                                           -   23      pdata (lower 8 bits)
*                                           -   22      pdata (upper 8 bits)
*                                           -   21      XX (pointed to by FSR1, frame pointer)
*                                           -   20      STATUS  register
*                                           -   19      BSR     register
*                                           -   18      W       register
*                                           -   17      FSR0L   register
*                                           -   16      FSR0H   register
*                                           -   15      FSR2L   register (frame pointer, low)
*                                           -   14      FSR2H   register (frame pointer, high)
*                                           -   13      TBLPTRL register
*                                           -   12      TBLPTRH register
*                                           -   11      TBLPTRU register
*                                           -   10      PRODL   register
*                                           -   9       PRODH   register
*                                           -   8       AARGB3  memory location
*                                           -   7       AARGB2  memory location
*                                           -   6       AARGB1  memory location
*                                           -   5       AARGB0  memory location
*
*                                       (end repeating, based on number of items)
*                                           -   4       TOSL    (top of return stack, low)
*                                           -   3       TOSH    (top of return stack, high)
*                                           -   2       TOSU    (top of return stack, upper)
*                                       (start repeating, based on number of items)
*
*                                           -   1       (number of return stack items)
*
*               OSTCBHighRdy->OSTCBStkPtr   -   0 ----> (free item)
*                                                                                          HIGH MEMORY
*
*               2) OSStartHighRdy() MUST:
*                      a) Call OSTaskSwHook()
*                      b) Set OSRunning to TRUE
*                      c) Switch to the highest priority task.
*********************************************************************************************************
*/

void OSStartHighRdy (void)
{
    // Call user definable OSTaskSwHook();
    OSTaskSwHook();

    // Set OSRunning to TRUE
    OSRunning = OS_TRUE;

    // Get the stack pointer of the task to resume.
    //  Stack Pointer = OSTCBHighRdy->OSTCBStkPtr;
_asm
    MOVFF   OSTCBHighRdy,   FSR0L   // load the STCBHighRdy->OSTCBStkPtr into the free indirect register
    MOVFF   OSTCBHighRdy+1, FSR0H
    MOVFF   POSTINC0,   FSR1L       // copy the variable into the stack pointer
    MOVFF   POSTDEC0,   FSR1H
_endasm

    // delete the contents of the hardware return stack - In OSStartHighRdy() ONLY
    while(STKPTR & 0x1F)
    {
    _asm
        POP     // discard the return address pointer
    _endasm
    }
    if(STKPTR)
        STKPTR = 0;

    // restore the hardware return stack
_asm
    MOVF    POSTDEC1,   1,  0       // decrement one to set it on the first item
    MOVF    POSTDEC1,   0,  0       // number of function pointers in hardware return stack
    MOVWF   FSR0L,  0               // user FSR0 as a temperary counter
_endasm
    do
    {
    _asm
        PUSH                            // push current address onto hardware stack
        MOVF    POSTDEC1,   0,  0       // then change to the function pointer in stack
        MOVWF   TOSU,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSH,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSL,   0
    _endasm
    } while(--FSR0L);               // decrement counter and loop if not finished

    // restore all processor registers from the new task's stack:
_asm
    MOVFF   POSTDEC1,   __AARGB0
    MOVFF   POSTDEC1,   __AARGB1
    MOVFF   POSTDEC1,   __AARGB2
    MOVFF   POSTDEC1,   __AARGB3
    MOVFF   POSTDEC1,   PRODH
    MOVFF   POSTDEC1,   PRODL
    MOVFF   POSTDEC1,   TBLPTRU
    MOVFF   POSTDEC1,   TBLPTRH
    MOVFF   POSTDEC1,   TBLPTRL
    MOVFF   POSTDEC1,   FSR2H
    MOVFF   INDF1,      FSR2L
    MOVF    POSTDEC1,   1,  0
    MOVFF   POSTDEC1,   FSR0H
    MOVFF   POSTDEC1,   FSR0L
    MOVF    POSTDEC1,   0,  0
    MOVFF   POSTDEC1,   BSR
    MOVFF   POSTDEC1,   STATUS
_endasm

    // execute a return from interrupt instruction
_asm
    RETFIE 0
_endasm
}

/*
*********************************************************************************************************
*                                       TASK LEVEL CONTEXT SWITCH
*
* Description : This function is called when a task makes a higher priority task ready-to-run.
*
* Arguments   : none
*
* Note(s)     : 1) Upon entry,
*                  OSTCBCur     points to the OS_TCB of the task to suspend
*                  OSTCBHighRdy points to the OS_TCB of the task to resume
*
*               2) The stack frame of the task to resume looks as follows:
*
*                                                                                          LOW MEMORY
*                                           -   23      pdata (lower 8 bits)
*                                           -   22      pdata (upper 8 bits)
*                                           -   21      XX (pointed to by FSR1, frame pointer)
*                                           -   20      STATUS  register
*                                           -   19      BSR     register
*                                           -   18      W       register
*                                           -   17      FSR0L   register
*                                           -   16      FSR0H   register
*                                           -   15      FSR2L   register (frame pointer, low)
*                                           -   14      FSR2H   register (frame pointer, high)
*                                           -   13      TBLPTRL register
*                                           -   12      TBLPTRH register
*                                           -   11      TBLPTRU register
*                                           -   10      PRODL   register
*                                           -   9       PRODH   register
*                                           -   8       AARGB3  memory location
*                                           -   7       AARGB2  memory location
*                                           -   6       AARGB1  memory location
*                                           -   5       AARGB0  memory location
*
*                                       (end repeating, based on number of items)
*                                           -   4       TOSL    (top of return stack, low)
*                                           -   3       TOSH    (top of return stack, high)
*                                           -   2       TOSU    (top of return stack, upper)
*                                       (start repeating, based on number of items)
*
*                                           -   1       (number of return stack items)
*
*               OSTCBHighRdy->OSTCBStkPtr   -   0 ----> (free item)
*                                                                                          HIGH MEMORY
*********************************************************************************************************
*/

//#pragma interrupt OSCtxSw
void  OSCtxSw (void)
{
    // disable interrupts
    INTCON &= (0b00111111 | (RCON & 0b10000000));
    
    // Save processor registers
_asm
    MOVFF   STATUS, PREINC1
    MOVFF   BSR,    PREINC1
    MOVWF   PREINC1,    0
    MOVFF   FSR0L,  PREINC1
    MOVFF   FSR0H,  PREINC1
    MOVF    POSTINC1,   1,  0
    MOVFF   FSR2L,  POSTINC1
    MOVFF   FSR2H,  POSTINC1
    MOVFF   TBLPTRL,POSTINC1
    MOVFF   TBLPTRH,POSTINC1
    MOVFF   TBLPTRU,POSTINC1
    MOVFF   PRODL,  POSTINC1
    MOVFF   PRODH,  POSTINC1
    MOVFF   __AARGB3, POSTINC1
    MOVFF   __AARGB2, POSTINC1
    MOVFF   __AARGB1, POSTINC1
    MOVFF   __AARGB0, POSTINC1
_endasm

    // Save the current task's return address hardware stack
    FSR0L = 0;
    while(STKPTR & 0x1F)
    {
    _asm
        MOVFF   TOSL,   POSTINC1        // copy the return address pointer
        MOVFF   TOSH,   POSTINC1        //   to the end of the software stack.
        MOVFF   TOSU,   POSTINC1
        POP                             // discard the top stack item that we just copied
    _endasm
        FSR0L++;
    }
_asm
    MOVFF   FSR0L,  POSTINC1            // copy the count to the last item in the software stack
_endasm
    if(STKPTR)
        STKPTR = 0;


    // Save the current task's stack pointer into the current task's OS_TCB:
    //   OSTCBCur->OSTCBStkPtr = Stack Pointer;
_asm
    MOVFF   OSTCBCur,   FSR0L       // load the OSTCBCur->OSTCBStkPtr into the free indirect register
    MOVFF   OSTCBCur+1, FSR0H
    MOVFF   FSR1L,      POSTINC0    // copy the stack pointer into the variable
    MOVFF   FSR1H,      POSTDEC0
_endasm

    // Call OSTaskSwHook()
    OSTaskSwHook();

    // re-arrange the pointers
    OSTCBCur = OSTCBHighRdy;
    OSPrioCur = OSPrioHighRdy;

    // Get the stack pointer of the task to resume.
    //  Stack Pointer = OSTCBHighRdy->OSTCBStkPtr;
_asm
    MOVFF   OSTCBHighRdy,   FSR0L   // load the STCBHighRdy->OSTCBStkPtr into the free indirect register
    MOVFF   OSTCBHighRdy+1, FSR0H
    MOVFF   POSTINC0,   FSR1L       // copy the variable into the stack pointer
    MOVFF   POSTDEC0,   FSR1H
_endasm

    // restore the hardware return stack
_asm
    MOVF    POSTDEC1,   1,  0       // decrement stack pointer one to set it on the first item
    MOVF    POSTDEC1,   0,  0       // number of function pointers in hardware return stack
    MOVWF   FSR0L,  0               // user FSR0 as a temperary counter
_endasm
    do
    {
    _asm
        PUSH                            // push current address onto hardware stack
        MOVF    POSTDEC1,   0,  0       // then change to the function pointer in stack
        MOVWF   TOSU,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSH,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSL,   0
    _endasm
    } while(--FSR0L);               // decrement counter and loop if not finished

    // restore all processor registers from the new task's stack:
_asm
    MOVFF   POSTDEC1,   __AARGB0
    MOVFF   POSTDEC1,   __AARGB1
    MOVFF   POSTDEC1,   __AARGB2
    MOVFF   POSTDEC1,   __AARGB3
    MOVFF   POSTDEC1,   PRODH
    MOVFF   POSTDEC1,   PRODL
    MOVFF   POSTDEC1,   TBLPTRU
    MOVFF   POSTDEC1,   TBLPTRH
    MOVFF   POSTDEC1,   TBLPTRL
    MOVFF   POSTDEC1,   FSR2H
    MOVFF   INDF1,      FSR2L
    MOVF    POSTDEC1,   1,  0
    MOVFF   POSTDEC1,   FSR0H
    MOVFF   POSTDEC1,   FSR0L
    MOVF    POSTDEC1,   0,  0
    MOVFF   POSTDEC1,   BSR
    MOVFF   POSTDEC1,   STATUS
_endasm

    // execute a return from interrupt instruction
_asm
    RETFIE 0
_endasm
}

/*
*********************************************************************************************************
*                                             TICK ISR
*
* Notes: 1) Your ISR MUST be added into the CPUInterruptHook()
*
*       setup the timer with the following instructions.
*       T1CON = 0x0F;
*         or
*       movlw   0x0F
*       movwf   T1CON
*********************************************************************************************************
*/
void CPUhighVector(void);
void CPUlowVector(void);
void CPUlowInterrupt(void);

#pragma code highVector=0x008
void CPUhighVector(void)
{
_asm 
	BTFSC RCON, 7, 0
	goto CPUhighInterruptHook
	goto CPUlowInterrupt
_endasm
}

#pragma code lowVector=0x018
void CPUlowVector(void)
{
    _asm goto CPUlowInterrupt _endasm
}
#pragma code

void CPUlowInterrupt(void)
{
    // Save processor registers
_asm
    MOVFF   STATUS, PREINC1
    MOVFF   BSR,    PREINC1
    MOVWF   PREINC1,    0
    MOVFF   FSR0L,  PREINC1
    MOVFF   FSR0H,  PREINC1
    MOVF    POSTINC1,   1,  0
    MOVFF   FSR2L,  POSTINC1
    MOVFF   FSR2H,  POSTINC1
    MOVFF   TBLPTRL,POSTINC1
    MOVFF   TBLPTRH,POSTINC1
    MOVFF   TBLPTRU,POSTINC1
    MOVFF   PRODL,  POSTINC1
    MOVFF   PRODH,  POSTINC1
    MOVFF   __AARGB3, POSTINC1
    MOVFF   __AARGB2, POSTINC1
    MOVFF   __AARGB1, POSTINC1
    MOVFF   __AARGB0, POSTINC1
_endasm

    // Notify kernel about interrupt by incrementing OSIntNesting or by calling OSIntEnter()
    OSIntNesting++;
    //OSIntEnter();

    // Call the user defined interrupt hook.
    CPUlowInterruptHook();

    // Call OSIntExit() to determine what task to switch to.
    OSIntExit();
_asm
    MOVF    POSTDEC1,   1,  0       // decrement stack pointer one to set it on the first item
_endasm

    // restore all processor registers from the new task's stack:
_asm
    MOVFF   POSTDEC1,   __AARGB0
    MOVFF   POSTDEC1,   __AARGB1
    MOVFF   POSTDEC1,   __AARGB2
    MOVFF   POSTDEC1,   __AARGB3
    MOVFF   POSTDEC1,   PRODH
    MOVFF   POSTDEC1,   PRODL
    MOVFF   POSTDEC1,   TBLPTRU
    MOVFF   POSTDEC1,   TBLPTRH
    MOVFF   POSTDEC1,   TBLPTRL
    MOVFF   POSTDEC1,   FSR2H
    MOVFF   INDF1,      FSR2L
    MOVF    POSTDEC1,   1,  0
    MOVFF   POSTDEC1,   FSR0H
    MOVFF   POSTDEC1,   FSR0L
    MOVF    POSTDEC1,   0,  0
    MOVFF   POSTDEC1,   BSR
    MOVFF   POSTDEC1,   STATUS
_endasm

    // execute a return from interrupt instruction
_asm
    RETFIE 0
_endasm
}

/*
*********************************************************************************************************
*                               PERFORM A CONTEXT SWITCH (From an ISR)
*
* Description : This function is called when an ISR makes a higher priority task ready-to-run.
*
* Arguments   : none
*
* Note(s)     : 1) Upon entry,
*                  OSTCBCur     points to the OS_TCB of the task to suspend
*                  OSTCBHighRdy points to the OS_TCB of the task to resume
*
*               2) The stack frame of the task to suspend looks as follows.
*
*                                                                                          LOW MEMORY
*                                           -   19      pdata (lower 8 bits)
*                                           -   18      pdata (upper 8 bits)
*                                           -   17      XX (pointed to by FSR1, frame pointer)
*                                           -   16      STATUS  register
*                                           -   15      BSR     register
*                                           -   14      W       register
*                                           -   13      FSR0L   register
*                                           -   12      FSR0H   register
*                                           -   11      FSR2L   register (frame pointer, low)
*                                           -   10      FSR2H   register (frame pointer, high)
*                                           -   9       TBLPTRL register
*                                           -   8       TBLPTRH register
*                                           -   7       TBLPTRU register
*                                           -   6       PRODL   register
*                                           -   5       PRODH   register
*                                           -   4       AARGB3  memory location
*                                           -   3       AARGB2  memory location
*                                           -   2       AARGB1  memory location
*                                           -   1       AARGB0  memory location
*
*				OSTCBCur->OSTCBStkPtr		-   0 ----> (free item
*
*				3) The following itms need to be placed on to the stack.
*				   The function return stack holds two extra items that need
*				   to be removed before saving, OSIntExit() and OSIntCtxSw().
*
*                                       (end repeating, based on number of items)
*                                           +   0       TOSL    (top of return stack, low)
*                                           +   1       TOSH    (top of return stack, high)
*                                           +   2       TOSU    (top of return stack, upper)
*                                       (start repeating, based on number of items)
*
*                                           +   3       (number of return stack items)
*
*               OSTCBCur->OSTCBStkPtr		+   4 ----> (free item)
*                                                                                          HIGH MEMORY
*
*               3) The stack frame of the task to resume looks as follows:
*
*                                                                                          LOW MEMORY
*                                           -   23      pdata (lower 8 bits)
*                                           -   22      pdata (upper 8 bits)
*                                           -   21      XX (pointed to by FSR1, frame pointer)
*                                           -   20      STATUS  register
*                                           -   19      BSR     register
*                                           -   18      W       register
*                                           -   17      FSR0L   register
*                                           -   16      FSR0H   register
*                                           -   15      FSR2L   register (frame pointer, low)
*                                           -   14      FSR2H   register (frame pointer, high)
*                                           -   13      TBLPTRL register
*                                           -   12      TBLPTRH register
*                                           -   11      TBLPTRU register
*                                           -   10      PRODL   register
*                                           -   9       PRODH   register
*                                           -   8       AARGB3  memory location
*                                           -   7       AARGB2  memory location
*                                           -   6       AARGB1  memory location
*                                           -   5       AARGB0  memory location
*
*                                       (end repeating, based on number of items)
*                                           -   4       TOSL    (top of return stack, low)
*                                           -   3       TOSH    (top of return stack, high)
*                                           -   2       TOSU    (top of return stack, upper)
*                                       (start repeating, based on number of items)
*
*                                           -   1       (number of return stack items)
*
*               OSTCBHighRdy->OSTCBStkPtr   -   0 ----> (free item)
*                                                                                          HIGH MEMORY
*********************************************************************************************************
*/

void OSIntCtxSw (void)
{
	// Remove the two extra return items, OSIntExit() and OSIntCtxSw().
_asm
    POP
    POP
_endasm

#if OS_CRITICAL_METHOD == 3                      /* De-Allocate storage for CPU status register        */
_asm
	MOVF POSTDEC1, 1, 0
	MOVF POSTDEC1, 1, 0
	MOVF POSTDEC1, 1, 0
_endasm
#endif

	// Save the current task's return address hardware stack
    FSR0L = 0;
    while(STKPTR & 0x1F)
    {
    _asm
        MOVFF   TOSL,   POSTINC1        // copy the return address pointer
        MOVFF   TOSH,   POSTINC1        //   to the end of the software stack.
        MOVFF   TOSU,   POSTINC1
        POP                             // discard the top stack item that we just copied
    _endasm
        FSR0L++;
    }
_asm
    MOVFF   FSR0L,  POSTINC1            // copy the count to the last item in the software stack
_endasm
    if(STKPTR)
        STKPTR = 0;

    // Save the current task's stack pointer into the current task's OS_TCB:
    //   OSTCBCur->OSTCBStkPtr = Stack Pointer;
_asm
    MOVFF   OSTCBCur,   FSR0L       // load the OSTCBCur->OSTCBStkPtr into the free indirect register
    MOVFF   OSTCBCur+1, FSR0H
    MOVFF   FSR1L,      POSTINC0    // copy the stack pointer into the variable
    MOVFF   FSR1H,      POSTDEC0
_endasm


    // Call OSTaskSwHook()
    OSTaskSwHook();

    // re-arrange the pointers
    OSTCBCur = OSTCBHighRdy;
    OSPrioCur = OSPrioHighRdy;

    // Get the stack pointer of the task to resume.
    //  Stack Pointer = OSTCBHighRdy->OSTCBStkPtr;
_asm
    MOVFF   OSTCBHighRdy,   FSR0L   // load the STCBHighRdy->OSTCBStkPtr into the free indirect register
    MOVFF   OSTCBHighRdy+1, FSR0H
    MOVFF   POSTINC0,   FSR1L       // copy the variable into the stack pointer
    MOVFF   POSTDEC0,   FSR1H
_endasm

    // delete the contents of the hardware return stack - In OSStartHighRdy() ONLY
    while(STKPTR & 0x1F)
    {
    _asm
        POP     // discard the return address pointer
    _endasm
    }
    if(STKPTR)
        STKPTR = 0;

    // restore the hardware return stack
_asm
    MOVF    POSTDEC1,   1,  0       // decrement stack pointer one to set it on the first item
    MOVF    POSTDEC1,   0,  0       // number of function pointers in hardware return stack
    MOVWF   FSR0L,  0               // user FSR0 as a temperary counter
_endasm
    do
    {
    _asm
        PUSH                            // push current address onto hardware stack
        MOVF    POSTDEC1,   0,  0       // then change to the function pointer in stack
        MOVWF   TOSU,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSH,   0
        MOVF    POSTDEC1,   0,  0
        MOVWF   TOSL,   0
    _endasm
    } while(--FSR0L);               // decrement counter and loop if not finished

    // restore all processor registers from the new task's stack:
_asm
    MOVFF   POSTDEC1,   __AARGB0
    MOVFF   POSTDEC1,   __AARGB1
    MOVFF   POSTDEC1,   __AARGB2
    MOVFF   POSTDEC1,   __AARGB3
    MOVFF   POSTDEC1,   PRODH
    MOVFF   POSTDEC1,   PRODL
    MOVFF   POSTDEC1,   TBLPTRU
    MOVFF   POSTDEC1,   TBLPTRH
    MOVFF   POSTDEC1,   TBLPTRL
    MOVFF   POSTDEC1,   FSR2H
    MOVFF   INDF1,      FSR2L
    MOVF    POSTDEC1,   1,  0
    MOVFF   POSTDEC1,   FSR0H
    MOVFF   POSTDEC1,   FSR0L
    MOVF    POSTDEC1,   0,  0
    MOVFF   POSTDEC1,   BSR
    MOVFF   POSTDEC1,   STATUS
_endasm

    // execute a return from interrupt instruction
_asm
    RETFIE 0
_endasm
}
