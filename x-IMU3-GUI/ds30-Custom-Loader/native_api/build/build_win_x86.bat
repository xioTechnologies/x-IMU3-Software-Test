@echo off

if "%VCINSTALLDIR%"=="" (
	echo Open the Visual Studio Command Prompt from the start menu, then run this script
	pause
	goto end
)

set SRC_DIR=..\src
set LIB_DIR=..\lib
set OUT_DIR=..\bin
set EXE_NAME=ds30_Loader_x86.exe

cl /O1 %SRC_DIR%\ds30_loader_native_console.c %LIB_DIR%\ds30_Loader_Win_x86.lib /Fe%OUT_DIR%\%EXE_NAME%

del *.obj

:end
