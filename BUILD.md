# Build Instructions

This document explains how to build, test, and install the JoeyAES project on Linux and Windows.

## Prerequisites

### Linux
- GCC compiler (`gcc`)
- GNU Make (`make`)
- (Optional) MinGW cross-compiler for Windows builds: `x86_64-w64-mingw32-gcc`
- (Optional) Wine for testing Windows executables on Linux

### Windows
- GCC compiler (via MinGW-w64 or MSYS2)
- GNU Make
- Windows SDK (bcrypt.lib for cryptographic functions)

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

## Native Windows Build

### Build the application
```cmd
make
```
Creates `build\joeyaes.exe`.

### Run tests
```cmd
make test
```

### Install system-wide (requires Administrator)
```cmd
make install
```
Installs `joeyaes.exe` to `C:\Windows\System32\`.

### Uninstall
```cmd
make uninstall
```
Removes `joeyaes.exe` from `C:\Windows\System32\`.

### Clean build
```cmd
make clean
```

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

### Link errors with bcrypt
- **Windows**: Ensure Windows SDK is installed
- **Cross-compile**: MinGW should include bcrypt.lib by default
