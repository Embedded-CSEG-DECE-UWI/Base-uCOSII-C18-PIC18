                         uC/OS-II, The Real-Time Kernel

                                   PIC18xxx
                                 (Large Model)

                                 READ ME FILE
                           V1.01 - December 20, 2002

------------------------------ MPLAB-C18 C COMPILER ---------------------------
1)  The  current  version  of  the Microchip PIC18 port is based on the
    Microchip MPLAB C18 compiler version 2.09.24 or higher.  v2.00 has bugs.

    Microchip Technology Inc.
    2355 West Chandler Blvd.
    Chandler, AZ 85224-6199
    USA

    480-792-7200
    480-792-7277 (FAX)

    www.microchip.com

2)  This port was tested using  MPLAB IDE v6.00.20 Simulator and was tested on
    the PICDEM 2 Plus Demo Board.  The tested processor was a PIC18F452.

3)  The following settings were set for the C18 C compiler:
    -mL -Ls -O-


------------------------------ PIC18 PORT NOTES -------------------------------

1) The port installation batch file (INSTALL.BAT) places the PIC18 port in the
   following directory of the selected drive:

   \SOFTWARE\uCOS-II\PIC18\MPLAB-C18

   This directory contains the following sub-directories:

   \SOFTWARE\uCOS-II\PIC18\MPLAB-C18\SOURCE
       This directory contains the port source files.

   \SOFTWARE\uCOS-II\PIC18\MPLAB-C18\WORK
       This directory contains the port source files as well as the sample test
       code (i.e. TEST.C).

   \SOFTWARE\uCOS-II\PIC18\MPLAB-C18\LIBRARY
       This directory contains MPLAB C18 Library files that were modified to
       to use the large memory space and a large stack.
       The XLCD library was also modified to work on the PICDEM 2 PLUS Demo Board.

2)  The port makes use of the PICs timer #0 for the tick ISR (OSTickISR()).

3)  You must use a large code model, large data model, and a large stack.
    This includes the libraries that you are using.
    Modified .bat files to build the libraries are included.

4)  Integer promotion must be disabled.
    The default storage class must be Auto.
    Optimizations must be disabled for now.


5)  *.lkr scripts are included both with the debugger and without.
    These scripts set the memory space to be one contigious space to make
    use of the large memory and large stack option ( -ls ml).
    Large memory is needed because uC/OS-II uses more than 0xFF bytes of ram.

6)  Some modifications can be made to uCOS_II.H and OS_CORE.C to save RAM.
    These modifications make the code read out of the program memory/ROM.

    The following line is modified in uCOS_II.H:

/* Microchip PIC18xxx specific - lookup in program memory because of limited RAM space */
extern rom INT8U  const   OSMapTbl[];               /* Priority->Bit Mask lookup table                 */
extern rom INT8U  const   OSUnMapTbl[];             /* Priority->Index    lookup table                 */
/* End Microchip PIC18xxx specific */

    The following lines are modified in OS_CORE.C:

/* Microchip PIC18xxx specific - lookup in program memory because of limited RAM space */
rom INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/* End Microchip PIC18xxx specific */

/* Microchip PIC18xxx specific - lookup in program memory because of limited RAM space */
rom INT8U  const  OSUnMapTbl[] = {
/* End Microchip PIC18xxx specific */


------------------------------ REPORTING PROBLEMS -----------------------------

If  you find a problem (i.e. bug) in this port,  do not hesitate to
report the problem to me:

 1) Through the INTERNET:

        My INTERNET address is:   nathan@sputnickonline.com

 2) By writing to me:

        Nathan Brown
        CPO 397
        PO Box 7001
        Longview, TX 75607
        U.S.A.

 3) By calling me (after 7PM Central time, US):

        (903) 446-6123
        (630) 563-1171 (FAX)

Make sure that the problem you are  reporting is in or this port and
not uC/OS-II or your application.

------------------------------ PIC18 RELEASE NOTES ---------------------------

December 20, 2002: v1.01
------------------------
 1) Added the ablity work with interrupt priority levels.  A new 
    OS_ENTER_CRITICAL_HIGH() function was added to allow for disabling high
    priority interrupts. OS_ENTER_CRITICAL() only disables low priority
    interrupts when priority levels are enabled, otherwise it disables all.

 2) Interrupt handling does not save the program return stack unless the OS
    is switching tasks.  Provides a speedup.

 3) Saved in context return 24 and 32 bit values. 8 bit numbers use WREG,
    which is saved.  16 return values use PRODH:PRODL which is also saved.
    Others up to 32 bits use AARGB0:AARGB1:AARGB2:AARGB3.

October 30, 2002: V1.00
------------------------

 1) First release of the PIC18 port.
