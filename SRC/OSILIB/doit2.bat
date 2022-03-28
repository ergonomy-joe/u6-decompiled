@echo off
echo "START" > log.txt
set OLDPATH=%PATH%

REM set TC=\RETRO\TOOLS\TURBOC20\
set TC=\TOOLS\TURBOC20\
set INCLUDE=%TC%\INCLUDE
set LIB=%TC%\LIB
set PATH=%TC%

tcc -I%INCLUDE% -I.. -d -mm -p -c -Z -O -G seg_33e3.c >> log.txt
REM tcc -I%INCLUDE% -d- -mm -c -O -G -B seg_33e3.c >> log.txt


REM tlink %LIB%\c0m seg_0919,u.exe,,%LIB%\CM; >> log.txt

set TC=
set INCLUDE=
set LIB=
set PATH=%OLDPATH%
set OLDPATH=

type log.txt