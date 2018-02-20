SETLOCAL
:: script global variables
SET myStartupExe=Launcher_Release_x64.exe
SET myUser=Robin
SET myFlags=
cd /d %~dp0Release\
%myStartupExe% -user %myUser%%myFlags%
