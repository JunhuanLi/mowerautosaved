@echo off

set MATLAB=D:\Program Files\MATLAB\R2016b

cd .

if "%1"=="" (D:\PROGRA~1\MATLAB\R2016b\bin\win64\gmake  -f fit_ellipse_rtw.mk all) else (D:\PROGRA~1\MATLAB\R2016b\bin\win64\gmake  -f fit_ellipse_rtw.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
