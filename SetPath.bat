@echo on

set FolderToAdd=D:\lib\opencv\build\x64\vc14\bin

set Key=HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment

For /f "tokens=2*" %%a In ('Reg query "%key%" /v Path^|Find "Path"') do set CurPath=%%~b
reg add "%Key%" /v Path /t REG_EXPAND_SZ /d "%CurPath%;%FolderToAdd%" /f

pause
