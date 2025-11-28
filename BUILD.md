# Build Instructions

This document explains how to build, test, and install the JoeyAES project on Linux, how to cross-compile for Windows from Linux, and how to build natively on Windows from a plain Command Prompt.

## Prerequisites

### Linux
- GCC compiler (`gcc`)
- GNU Make (`make`)
- (Optional) MinGW cross-compiler for Windows builds: `x86_64-w64-mingw32-gcc`
- (Optional) Wine for testing Windows executables on Linux

### Windows
- GCC + Make via MinGW-w64 (MSYS2 or WinLibs)
- Git (optional; you can also download a ZIP)
- Administrator privileges for install/uninstall

---

## Linux Build

### Basic Commands

#### Build the CLI application
```bash
make
# or
make cli
```
Creates the `joeyaes` executable in `build/joeyaes`.

#### Build libraries only
```bash
make libs
```
Builds the static libraries without creating the CLI binary:
- `build/libcore.a` - Core AES encryption functions
- `build/libfileutils.a` - File utilities and key generation

#### Run all tests
```bash
make test
```
Runs the complete test suite (26 tests total).

#### Run individual test suites
```bash
make test_Sbox_gen          # Test S-box generation (5 tests)
make test_key_expansion     # Test AES key expansion (5 tests)
make test_aes_functions     # Test AES primitives (4 tests)
make test_cipher            # Test encryption (3 tests)
make test_inv_cipher        # Test decryption (3 tests)
make test_file_utils        # Test file operations (6 tests)
make test_key_gen           # Test key generation
```

#### Clean build artifacts
```bash
make clean
```
Removes all object files, libraries, executables, and test binaries.

#### Install system-wide (requires sudo)
```bash
sudo make install
```
Installs `joeyaes` to `/usr/local/bin/joeyaes`.

#### Uninstall
```bash
sudo make uninstall
```
Removes `joeyaes` from `/usr/local/bin/`.

---

## Windows Cross-Compilation (from Linux)

Build Windows executables on Linux using the MinGW cross-compiler.

### Prerequisites
```bash
sudo apt install mingw-w64 wine
```

### Build for Windows
```bash
CROSS_COMPILE_WIN=1 make clean
CROSS_COMPILE_WIN=1 make
```
Creates `build/joeyaes.exe`.

### Test Windows build on Linux (using Wine)
```bash
CROSS_COMPILE_WIN=1 make test
```
Tests run through Wine automatically. Wine display errors can be ignored.

### Clean Windows build
```bash
CROSS_COMPILE_WIN=1 make clean
```

---

## Windows Quickstart (native CMD, no prerequisites)

These steps start from a plain Windows Command Prompt (CMD). Choose A (winget + MSYS2) or B (manual WinLibs). If running install/uninstall, open an elevated CMD (Run as Administrator).

### A) Using winget + MSYS2 (recommended)

1) Install tools
```cmd
winget install -e --id Git.Git
winget install -e --id MSYS2.MSYS2
```

2) In the MSYS2 MinGW64 shell (launch from Start Menu → MSYS2 MinGW64), install the MinGW toolchain:
```bash
pacman -Sy --noconfirm
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

3) Back in CMD, add MinGW to PATH (adjust if MSYS2 installed elsewhere):
```cmd
setx PATH "%PATH%;C:\msys64\mingw64\bin"
```

4) Optional: alias mingw32-make as make (some setups only ship mingw32-make.exe)
```cmd
copy C:\msys64\mingw64\bin\mingw32-make.exe C:\msys64\mingw64\bin\make.exe
```

5) Get the source
```cmd
git clone https://github.com/JoeyBryson/joey_AES.git
cd joey_AES
```
Alternatively (without Git):
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

### B) Manual toolchain (WinLibs, no winget)

1) Download and extract MinGW-w64 (WinLibs)
```cmd
mkdir C:\Tools
curl -L -o C:\Tools\winlibs.zip https://github.com/brechtsanders/winlibs_mingw/releases/latest/download/winlibs-x86_64-posix-seh-gcc.zip
tar -xf C:\Tools\winlibs.zip -C C:\Tools
```
Assuming it extracted to `C:\Tools\winlibs\mingw64` (adjust if different), add to PATH:
```cmd
setx PATH "%PATH%;C:\Tools\winlibs\mingw64\bin"
```

If only `mingw32-make.exe` exists, alias it to `make.exe`:
```cmd
copy C:\Tools\winlibs\mingw64\bin\mingw32-make.exe C:\Tools\winlibs\mingw64\bin\make.exe
```

2) Get the source
```cmd
git clone https://github.com/JoeyBryson/joey_AES.git
cd joey_AES
```
Or download ZIP if Git isn’t available:
```cmd
curl -L -o joey_AES.zip https://github.com/JoeyBryson/joey_AES/archive/refs/heads/main.zip
tar -xf joey_AES.zip
cd joey_AES-main
```

3) Build, test, install (same as above)
```cmd
make clean
make
make test
```
For install/uninstall, run CMD as Administrator:
```cmd
make install
make uninstall
```

Tip: If `make` isn’t found, confirm your PATH and the `make.exe` copy step above.

---

## Common Build Workflows

### Development workflow (Linux)
```bash
# Edit source files...
make clean && make        # Rebuild
make test                 # Run tests
./build/joeyaes help      # Test CLI locally
```

### Full test workflow (Linux → Windows cross-compile)
```bash
# Test on native Linux
make clean && make test

# Cross-compile and test for Windows
CROSS_COMPILE_WIN=1 make clean && CROSS_COMPILE_WIN=1 make test
```

### Install for system-wide use (Linux)
```bash
make clean
make
sudo make install
joeyaes help              # Now available system-wide
```

---

## Build Targets Summary

| Target | Description |
|--------|-------------|
| `make` or `make all` | Build the CLI application (default) |
| `make cli` | Build the CLI application |
| `make libs` | Build static libraries only |
| `make test` | Run all test suites (26 tests) |
| `make test_<name>` | Run specific test suite |
| `make clean` | Remove all build artifacts |
| `make install` | Install system-wide (requires elevated privileges) |
| `make uninstall` | Remove system-wide installation |

---

## Environment Variables

| Variable | Description |
|----------|-------------|
| `CROSS_COMPILE_WIN=1` | Enable Windows cross-compilation from Linux |
| `OS=Windows_NT` | Automatically set on native Windows builds |

---

## Platform-Specific Notes

### Windows
- The build system automatically links against `bcrypt.lib` for cryptographic random number generation
- Address Sanitizer (ASAN) is disabled for Windows builds as it's not supported by MinGW

### Linux
- Uses `/dev/urandom` for cryptographic random number generation
- Address Sanitizer is enabled by default for debugging

---

## Troubleshooting

### "command not found" errors
- **Linux**: Ensure `gcc` and `make` are installed
- **Windows cross-compile**: Install `mingw-w64` package
- **Wine testing**: Install `wine` package

### Permission denied on install
- **Linux**: Use `sudo make install`
- **Windows**: Run as Administrator

### Tests fail on Windows cross-compile
- Ensure Wine is installed and configured
- Wine display errors can be ignored (tests will still pass)

### Link errors with bcrypt (Windows native builds)
**Symptoms**: `undefined reference to 'BCryptGenRandom'` or similar linker errors.

**Common causes**:
- Older MinGW versions don't include bcrypt.lib
- Library search path doesn't include bcrypt

**Solutions** (try in order):
1. **Use modern MinGW-w64 via MSYS2** (recommended):
   ```cmd
   winget install -e --id MSYS2.MSYS2
   ```
   Then install toolchain from MSYS2 MinGW64 shell:
   ```bash
   pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-make
   ```

2. **Use WinLibs** (usually includes bcrypt): Follow manual WinLibs instructions above

3. **Add Windows SDK library path** (if Windows SDK is installed):
   Edit root `makefile` line 4 to add `-L` flag:
   ```makefile
   LDFLAGS = -lbcrypt -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"
   ```
   (Adjust path for your SDK version)

4. **Verify bcrypt availability**:
   ```cmd
   gcc -print-search-dirs
   # Check library paths for bcrypt.lib or libbcrypt.a
   ```

### Cross-compile: MinGW should include bcrypt.lib by default
Cross-compiling from Linux with `x86_64-w64-mingw32-gcc` typically works without issues.

### CROSS_COMPILE_WIN usage (common mistake)
To cross-compile from Linux, set the environment variable before `make`, e.g.:
```bash
CROSS_COMPILE_WIN=1 make clean && CROSS_COMPILE_WIN=1 make
```
Do not pass it as an option after make (e.g. `make --CROSS_COMPILE_WIN` will fail).
