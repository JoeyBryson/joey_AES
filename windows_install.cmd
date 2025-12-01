@echo off
setlocal

set INSTALL_DIR=%USERPROFILE%\bin
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

curl -L -o "%INSTALL_DIR%\joeyaes.exe" https://github.com/JoeyBryson/joey_AES/releases/download/v1.0.0/joeyaes.exe
ren "%INSTALL_DIR%\joeyaes.exe" joeyaes

setx PATH "%PATH%;%INSTALL_DIR%" >nul

echo Installation complete
endlocal
