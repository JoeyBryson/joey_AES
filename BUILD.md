# Build Instructions

This project supports building natively on both Linux and Windows, as well as cross-compiling from Linux to Windows.

## Building on Linux

### Prerequisites
- GCC compiler
- GNU Make

### Build
```bash
make
```

The binary will be created at `build/joeyaes`

### Clean
```bash
make clean
```

## Cross-Compiling for Windows (from Linux)

### Prerequisites
- MinGW-w64 cross-compiler
- GNU Make

Install on Ubuntu/Debian:
```bash
sudo apt install mingw-w64
```

### Build
```bash
CROSS_COMPILE_WIN=1 make
```

The Windows executable will be created at `build/joeyaes.exe`

### Clean
```bash
make clean
```

## Building on Windows

### Prerequisites
- MinGW-w64 (via MSYS2 or MinGW-w64 installer)
- GNU Make

### Build
Open a MinGW terminal and run:
```bash
make
```

The binary will be created at `build/joeyaes.exe`

## Platform-Specific Notes

### Windows
- The build system automatically links against `bcrypt.lib` for cryptographic random number generation
- Address Sanitizer (ASAN) is disabled for Windows builds as it's not supported by MinGW

### Linux
- Uses `/dev/urandom` for cryptographic random number generation
- Address Sanitizer is enabled by default for debugging

## Testing Cross-Compiled Windows Executable on Linux

You can test the Windows executable using Wine:
```bash
wine build/joeyaes.exe
```

Note: Wine may not fully support all Windows APIs, so testing on a real Windows system is recommended.
