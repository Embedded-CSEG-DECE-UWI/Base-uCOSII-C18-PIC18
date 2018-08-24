@echo off

if "%MCC_INCLUDE%"=="" goto ERRORINCLUDE 

call makeonep.bat 18c242
call makeonep.bat 18c252
call makeonep.bat 18c442
call makeonep.bat 18c452
call makeonep18cxx8.bat 18c658
call makeonep18cxx8.bat 18c858
call makeonep.bat 18c601
call makeonep.bat 18c801
call makeonep.bat 18f242
call makeonep.bat 18f252
call makeonep.bat 18f442
call makeonep.bat 18f452
call makeonep18fxx8.bat 18f248
call makeonep18fxx8.bat 18f258
call makeonep18fxx8.bat 18f448
call makeonep18fxx8.bat 18f458
call makeonep18f1x20.bat 18f1220
call makeonep18f1x20.bat 18f1320
call makeonep18f4x20.bat 18f2220
call makeonep18f4x20.bat 18f2320
call makeonep18f4x20.bat 18f4220
call makeonep18f4x20.bat 18f4320
call makeonep18fxx20.bat 18f6620
call makeonep18fxx20.bat 18f6720
call makeonep18fxx20.bat 18f8620
call makeonep18fxx20.bat 18f8720

goto EOF

:ERRORINCLUDE
echo Please set the environment variable MCC_INCLUDE to point to the
echo "h" directory of the mcc18 installation directory.

:EOF
