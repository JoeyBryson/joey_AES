# Step-by-Step Guide

This file gives copy‑pasteable steps to build, test, and install JoeyAES on:
- Linux (native)
- Linux → Windows (cross-compile)
- Windows (native CMD)

Binary output:
- Linux: `build/joeyaes`
- Windows: `build\joeyaes.exe`

---

## 1) Linux (native)

### 1.1 Install prerequisites
```bash
# Debian/Ubuntu
sudo apt update
sudo apt install -y build-essential

# Fedora
sudo dnf groupinstall -y "Development Tools"

# Arch
sudo pacman -S --needed base-devel
```

### 1.2 Get the source
```bash
git clone https://github.com/JoeyBryson/joey_AES.git
cd joey_AES
```
(Or download a ZIP from GitHub and extract.)

### 1.3 Build
```bash
make clean && make
```
Output: `build/joeyaes`

### 1.4 Run tests
```bash
make test
```

### 1.5 Install system-wide (optional)
```bash
sudo make install
```
Installs to `/usr/local/bin/joeyaes`.

### 1.6 Uninstall
```bash
sudo make uninstall
```

---

## 2) Linux → Windows (cross-compile)

### 2.1 Install prerequisites
```bash
sudo apt update
sudo apt install -y mingw-w64 wine
```

### 2.2 Build Windows binaries
```bash
CROSS_COMPILE_WIN=1 make clean && CROSS_COMPILE_WIN=1 make
```
Output: `build/joeyaes.exe`

### 2.3 Run tests under Wine
```bash
CROSS_COMPILE_WIN=1 make test
```
Wine may print some display-related warnings — safe to ignore if tests pass.

### 2.4 Common pitfall
Do NOT run:
```bash
make --CROSS_COMPILE_WIN   # ❌ incorrect
```
Use the environment variable style shown above.

---

## 3) Windows (native, Command Prompt)

You’ll install a GCC + Make toolchain and then build.

Pick A (winget + MSYS2) or B (manual WinLibs). For install/uninstall, run CMD as Administrator.

### 3A) Using winget + MSYS2 (recommended)

1) Install tools (Command Prompt)
```cmd
winget install -e --id Git.Git
winget install -e --id MSYS2.MSYS2
```

2) Open the "MSYS2 MinGW64" shell (Start Menu) and install toolchain
```bash
pacman -Sy --noconfirm
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

3) Back in Command Prompt, add MinGW to PATH (adjust if MSYS2 installed elsewhere)
```cmd
setx PATH "%PATH%;C:\msys64\mingw64\bin"
```

4) Optional (if only `mingw32-make.exe` exists)
```cmd
copy C:\msys64\mingw64\bin\mingw32-make.exe C:\msys64\mingw64\bin\make.exe
```

5) Get the source
```cmd
git clone https://github.com/JoeyBryson/joey_AES.git
cd joey_AES
```
No Git? Download ZIP:
```cmd
curl -L -o joey_AES.zip https://github.com/JoeyBryson/joey_AES/archive/refs/heads/main.zip
tar -xf joey_AES.zip
cd joey_AES-main
```

6) Build
```cmd
make clean
make
```
Output: `build\joeyaes.exe`

7) Run tests
```cmd
make test
```

8) Install (Administrator)
```cmd
make install
```
Installs to `C:\Windows\System32\joeyaes.exe`.

9) Uninstall (Administrator)
```cmd
make uninstall
```

### 3B) Manual toolchain (WinLibs, no winget)

1) Download & extract WinLibs (Command Prompt)
```cmd
mkdir C:\Tools
curl -L -o C:\Tools\winlibs.zip https://github.com/brechtsanders/winlibs_mingw/releases/latest/download/winlibs-x86_64-posix-seh-gcc.zip
tar -xf C:\Tools\winlibs.zip -C C:\Tools
```
Assuming `C:\Tools\winlibs\mingw64\bin` exists, add to PATH:
```cmd
setx PATH "%PATH%;C:\Tools\winlibs\mingw64\bin"
```
If only `mingw32-make.exe` exists:
```cmd
copy C:\Tools\winlibs\mingw64\bin\mingw32-make.exe C:\Tools\winlibs\mingw64\bin\make.exe
```

2) Get the source
```cmd
git clone https://github.com/JoeyBryson/joey_AES.git
cd joey_AES
```
Or ZIP route:
```cmd
curl -L -o joey_AES.zip https://github.com/JoeyBryson/joey_AES/archive/refs/heads/main.zip
tar -xf joey_AES.zip
cd joey_AES-main
```

3) Build / Test / Install
```cmd
make clean
make
make test
```
Install/uninstall (Administrator):
```cmd
make install
make uninstall
```

---

## Quick CLI check
After building (or installing), try:
```bash
# Linux
./build/joeyaes help

# Windows
build\joeyaes.exe help
# or if installed system-wide
joeyaes help
```

---

## Troubleshooting

### `make: command not found`
Ensure your PATH includes the bin folder with `make.exe` and `gcc.exe`. If only `mingw32-make.exe` exists, copy it to `make.exe` as shown above.

### Cross-compile CROSS_COMPILE_WIN
Set `CROSS_COMPILE_WIN=1` as an environment variable before `make` (don't use `make --CROSS_COMPILE_WIN`).

### Windows: Cannot find bcrypt / undefined reference to BCryptGenRandom
Some MinGW distributions (especially older ones) don't include `bcrypt.lib` or have linking issues. Solutions:

**Option 1: Use a modern MinGW-w64**
```cmd
# Uninstall old MinGW and use MSYS2 MinGW64 (recommended)
winget install -e --id MSYS2.MSYS2
# Then follow steps 2-4 from section 3A above
```

**Option 2: Verify bcrypt is available**
```cmd
gcc -print-search-dirs
# Check if bcrypt.lib or libbcrypt.a exists in the library paths
```

**Option 3: Use WinLibs (usually includes bcrypt)**
Follow section 3B above - WinLibs typically bundles bcrypt support.

**Option 4: Link directly to system bcrypt.dll**
If you have Windows SDK installed:
```cmd
# Edit root makefile line 4 to:
LDFLAGS = -lbcrypt -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"
```
(Adjust the path to match your Windows SDK version.)

### Install/uninstall requires elevated privileges
- **Windows**: Run Command Prompt as Administrator
- **Linux**: Use `sudo make install` / `sudo make uninstall`
