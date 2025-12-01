### Prerequisites
- GCC (Linux) or MinGW-w64 (for compiling Windows version)
- `make` utility

### Building on Linux

1. Open a terminal in the project root directory.
2. Run:
   ```bash
   make
   ```
3. The compiled binary will be located in `build/joeyaes`.
4. Install/unistall using
   ```bash
   make install
   make uninstall
   ```

### Cross-Compiling for Windows (from Linux/Debian)

1. Install MinGW-w64 cross-compiler:
   ```bash
   sudo apt-get update
   sudo apt-get install mingw-w64
   ```
2. Run the build command with the `w=1` flag:
   ```bash
   make w=1
   ```
3. The Windows executable will be located in `build/joeyaes.exe`.
4. Install manually from windows machine by adding joeyaes.exe to PATH

### Tests

Run all tests with the command 
```bash
make tests
```

Run invividual test files with 
```bash
make <test_name>
```
test names correspond to the file names in test-code without the extentions.
Use w=1 for testing the windows version(from linux OS).

### Notes
- Clean previous builds with:
    ```bash
    make clean
    ```
- Compile just the core libraries and not the CLI(prerequisit used used for test targets) 
    ```bash
    make libs
    ```