@echo off
setlocal

set INSTALL_DIR=%USERPROFILE%\bin
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

echo Downloading joeyaes.exe...
curl -L -o "%INSTALL_DIR%\joeyaes.exe" https://github.com/JoeyBryson/joey_AES/releases/download/v1.0.0/joeyaes.exe

REM Check if the file downloaded
if not exist "%INSTALL_DIR%\joeyaes.exe" (
    echo ERROR: Download failed. Installation aborted.
    endlocal
    exit /b 1
)

REM Ensure the file ends with .exe
rename "%INSTALL_DIR%\joeyaes.exe" "joeyaes.exe" >nul 2>&1

REM Add to PATH if needed
setx PATH "%PATH%;%INSTALL_DIR%" >nul

echo Installation complete.
endlocal
exit /b 0
