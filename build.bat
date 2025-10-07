@echo off
REM Quick build script for Windows Command Prompt
REM Usage: build.bat [Debug|Release] [x64|x86]

setlocal

set CONFIG=%1
set ARCH=%2

if "%CONFIG%"=="" set CONFIG=Release
if "%ARCH%"=="" set ARCH=x64

set PRESET=%ARCH%-%CONFIG%

echo ======================================
echo Building TsPy Plugin
echo Configuration: %CONFIG%
echo Architecture: %ARCH%
echo Preset: %PRESET%
echo ======================================

echo.
echo Configuring CMake...
cmake --preset %PRESET%
if %ERRORLEVEL% neq 0 (
    echo Configuration failed!
    exit /b 1
)

echo.
echo Building project...
cmake --build --preset %PRESET%
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo ======================================
echo Build Complete!
echo Output directory: build\%PRESET%\bin
echo ======================================
echo.
echo To install the plugin, copy tspy_plugin.dll to:
echo   %%APPDATA%%\TS3Client\plugins\

endlocal
