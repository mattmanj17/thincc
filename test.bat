@echo off
tcc\tcc thincc\shell.c
if %errorlevel% NEQ 0 exit /B 1
REM echo C:/Windows/System32/where.exe where > in.txt
echo where where > in.txt
shell < in.txt