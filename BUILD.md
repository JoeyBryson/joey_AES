# Build system

This project uses a manually coded makefile system. The core directory files are compiled into a library libcore.a which is then a dependency for the file-utilities directory which is compiled into the libutil.a library. All tests are performed using these libraries as source files. The CLI executables also depends on these libraries. This guide explains how to compile the project for both Linux and Windows binaries.

## Prerequisites

- GCC (Linux) or MinGW-w64 (for compiling Windows version)
- `make` utility
- git

first clone the repository

```bash
git clone https://github.com/JoeyBryson/joey_AES 
```

enter project directory

```bash
cd joey_aes
```

### Building on Linux

1. Run:

   ```bash
   make
   ```

2. The compiled binary `joeyaes` will be located in `./build`

3. Install/uninstall using

   ```bash
   make install
   make uninstall
   ```

### Cross-Compiling for Windows from Linux

1. Install MinGW-w64 cross-compiler:

   ```bash
   sudo apt-get update
   sudo apt-get install mingw-w64
   ```

2. Run the build command with the `w=1` flag:

   ```bash
   make w=1
   ```

3. The Windows executable`joeyaes.exe` will be located in `.\build`.
4. Install manually from windows machine by adding joeyaes.exe to PATH (follow command in windows_install.cmd)

### Tests

Run all tests with the command

```bash
make tests
```

Run individual test files with

```bash
make <test_name>
```

test names correspond to the file names in test-code without the extensions.
Use w=1 for testing the windows version(from linux OS).

## Notes

- Clean previous builds with:

    ```bash
    make clean
    ```

- the builds should be cleaned between compiling for different OS's

- Compile just the core libraries and not the CLI(prerequisite used for test targets)

    ```bash
    make libs
    ```
