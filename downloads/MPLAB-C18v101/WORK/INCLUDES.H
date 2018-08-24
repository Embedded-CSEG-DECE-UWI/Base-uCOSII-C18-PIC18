/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                           (c) Copyright 2002, Nathan Brown, Longview, TX
*                                      http://www.sputnickonline.com/
*                                          All Rights Reserved
*
*                                     PIC18xxxx Master Include File
*                                       V1.00 - October 27, 2002
*
* File : INCLUDES.H
* By   : Nathan Brown
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         PROCESSOR SPECIFICS
*********************************************************************************************************
*/

#define  CPU_FRT_FREQ        1000000L  /* Free Running Timer rate (Hz)                                 */

#define  OS_TICK_OC                 1  /* Output compare # used to generate a tick int.                */

                                       /* Number of FRT counts to produce an interrupt @tick rate      */
#define  OS_TICK_OC_CNTS  (CPU_FRT_FREQ / OS_TICKS_PER_SEC)

/*
*********************************************************************************************************
*                                         Standard Header Files
*********************************************************************************************************
*/

#include <p18cxxx.h>        // Register definitions
#include  <STRING.H>

/*
*********************************************************************************************************
*                                           FILES TO INCLUDE
*********************************************************************************************************
*/

#define   OS_ISR_PROTO_EXT                   /* Function prototypes for OSCtxSw() and OSTickISR() ...  */
                                             /* ... are found in OS_CPU.H                              */
#include  "OS_CPU.H"                         /* Processor specific header file                         */
#include  "OS_CFG.H"                         /* The OS configuration header                            */

#include  "uCOS_II.H"                        /* uC/OS-II header file                                   */
