@echo off
@if "%MSDevDir%" == "" call c:\VCVARS32.BAT
nmake /f Makefile.mk
rem Pause