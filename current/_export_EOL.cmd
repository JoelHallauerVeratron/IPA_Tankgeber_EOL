@echo off
@set TARGET=k:\LinkUp_EOL
@set PROGRAMM=*
@echo on

copy "%PROGRAMM%.exe" %TARGET%
rem copy *.dll %TARGET%
pause