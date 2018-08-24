@echo off

if "%MCC_INCLUDE%"=="" goto ERRORINCLUDE 

cd ..\lib
IF EXIST clib.lib del clib.lib
mplib /c clib.lib
cd ..\src

REM Using a "for" loop to delete files is cleaner than using a wildcard
REM because if there are no files to delete, no error message will be
REM displayed.

cd math\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

cd stdlib\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

cd string\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

cd ctype\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

cd delays\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

cd setjmp\18Cxx
for %%f in (*.o) do del %%f
for %%f in (*.err) do del %%f
for %%f in (*.lst) do del %%f

for %%i in (*.c) do mcc18 -ls -ml -p=18cxx %%i
for %%i in (*.asm) do mpasm /rDEC /l- /o /q /d__LARGE__ /p18cxx %%i
for %%i in (*.o) do mplib /r ..\..\..\lib\clib.lib %%i
cd ..\..

goto EOF

:ERRORINCLUDE
echo Please set the environment variable MCC_INCLUDE to point to the 
echo "h" directory of the mcc18 installation directory

:EOF