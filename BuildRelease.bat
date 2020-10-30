@ECHO off

IF [%1] EQU [] (
	SET TASK=build
) ELSE (
	SET TASK=%1
)

MSBUILD /p:Configuration=Release /p:Platform="Win32" /t:%TASK% MP819xVC.sln
IF %ERRORLEVEL% NEQ 0 (
	ECHO Build failed. Press any key to continue ...
)