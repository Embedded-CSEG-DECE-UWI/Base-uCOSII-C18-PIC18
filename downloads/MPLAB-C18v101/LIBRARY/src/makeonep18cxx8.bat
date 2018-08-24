@echo off

if "%MCC_INCLUDE%"=="" goto ERRORINCLUDE 

cd ..
cd lib
IF EXIST p%1.lib del p%1.lib
mplib /c p%1.lib
cd ..
cd src

REM Using a "for" loop to delete files is cleaner than using a wildcard
REM because if there are no files to delete, no error message will be
REM displayed.

cd proc
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 p%1.asm
mplib /r ..\..\lib\p%1.lib p%1.o
cd ..

cd pmc

cd CCP\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd PWM\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd i2c\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd mwire\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd SPI\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd SW_SPI\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd SW_UART\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd SW_I2C\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd Timers\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd USART\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd ADC\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd reset\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd PORTB\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd CAN2510\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=%1 %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p%1 %%i
for %%i in (*.o) do mplib /r ..\..\..\..\lib\p%1.lib %%i
cd ..\..

cd ..

goto EOF

:ERRORINCLUDE
echo Please set the environment variable MCC_INCLUDE to point to the 
echo "h" directory of the mcc18 installation directory

:EOF