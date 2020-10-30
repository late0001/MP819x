set WINRAR_EXE="C:\Program Files\WinRAR\winrar.exe"

::----------------------------------------------------------------------------------------------------

@echo off
chgcolor 0B   :: Light Aqua
chgcolor 0C   :: red
chgcolor E0   :: highlighted
chgcolor 07   :: reset

if exist %RTLBT_E% ( del "%RTLBT_E%")
if exist %RTLBT_U% ( del "%RTLBT_U%")
if exist %RTLBT_S% ( del "%RTLBT_S%")

:Archive 
chgcolor E0
echo Archiving the sample code...
chgcolor 07
%WINRAR_EXE% a -afzrar -ag_yyyyMMdd -m3 -ed  RTL819xVC.zip Doc res autotest.txt Color.h ColorEdit.cpp ColorEdit.h ColorStatic.cpp ColorStatic.h config.ini ConfigChip.ini CustomOid.h Dbg.h DbgCmd.cpp DbgField.cpp DbgField.h IniIO.cpp IniIO.h mp819xbd.dll mp819xbd.exp mp819xbd.lib mp819xbd.pdb MP819xVC.aps MP819xVC.clw MP819xVC.cpp MP819xVC.dsp MP819xVC.dsw MP819xVC.exe MP819xVC.h MP819xVC.ncb MP819xVC.opt MP819xVC.plg MP819xVC.rc MP819xVC.sln MP819xVC.vcxproj MP819xVC.vcxproj.filters MP819xVC.vcxproj.user MP819xVCDlg.cpp MP819xVCDlg.h mp8723xBT.h mp8723xBT_DllGet.cpp mp8723xBT_DllHeader.h NTDDNDIS.H NTDDPACK.H packet32.H ReadMe.txt resource.h rtkbt.dll RtlFunc.cpp RtlFunc.h StdAfx.cpp StdAfx.h TX_PWR_MAP.cpp TX_PWR_MAP.h 


:Exit
pause
