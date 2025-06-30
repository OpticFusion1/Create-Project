@echo off
setlocal

:: Initialize Visual Studio environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"

:: Move to your project folder automatically
cd /d D:\repository\projects\archive\programming\archive\c++\personal\standalone\create_project

:: Now you are ready to build!
mkdir build 2>nul
cd build
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
nmake

endlocal
pause