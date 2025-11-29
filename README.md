# JOEYAES
JOEYAES is is a command line based file encryption application written in C. It uses a naive personal implimentation of the (Advanced Encryption Standard) AES algorithm running in Cipher-Block-Chaining(CBC) mode to encrypt/decrypt individual files along with their file names. It has not been sufficiently vetted for security issues to be used for any sensitive application. It has been created for personal educational purposes only.

### **The AES algorythm** - ./src/core
 AES is the most widly used block cipher worldwide. It encrypts and decrypts individual 16 byte blocks, using a specific private key. It has three versions using keys of length 128, 192 and 256 bits, with longer keys offering more theoretical security although all are regarded as secure unless your adversaries have a quantum computer. The source of the specifications for building this algorithm was the official FIPS 197 document published in 2001 (and updated 2023) by the National Institute of Standards and Technology. This is an excelent resource for a project like this, as it has both detailed explanation and specifications, and examples that were used for testing. Terminology and names (eg. words, state) have mostly been pulled directly from this document.

https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

the files containing the algorythm functions described are in the ./core directory.

### **byte_t, word_t and state_t** - ./include/aes.h
This AES implementation mirrors the structure described in FIPS-197. Each byte is stored as a uint8_t (byte_t), four of which are grouped into a word_t, and four words form the state_t struct(where words form the *collumns* of the state matrix). This layout keeps the code close to the mathematical notation and makes the transformations easy to follow. The trade-off is performance: acting on individual bytes leads to more instructions and less parallelism than word-oriented operations. Implementations like OpenSSL use 32-bit words and optimized bitwise and table-driven operations to achieve much higher throughput at the cost of the code looking like an enchanting table.

### **Sbox** - ./src/core/Sbox_gen.c
The aes Substitution Box is an easily copyable 32 x 32 arry of bytes. This program goes through the unnecarsary effort of generating the Sbox from it's construction instructions defined in FIPS197 each time it runs. This takes minimal computation and is performed for the satisfaction of the designer.

### **Mode of operation** - ./src/file-utilities
In order to create full confidentiality for a string of blocks (like a file), but still using the same key, the program must encrypt each block, even identical ones to different cipher blocks. To achieve this, it's paired with a Mode Of Operation, JOEYAES uses CBC decribed here

https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38a.pdf

Each block is XOR'ed with the previous cipher block this is a way we can 'inject entropy' into the algorythm. The first block is XOR'd with an initialization vector block which must be included with the cipher blocks for decryption.

### **.jwy and .jky files** - ./src/file-utilities
.jwy and .jky files are custom file extentions created by this project.
.jwy files are the encrypted cipher files and contain:
- 7 magic Bytes JOEY128/ JOEY192/ JOEY256 to veryify the file type and algorithm used to encrypt
- the initialization vector
- the cipherblocks which when decrypted contain:
    - the file name length(1 byte)
    - the file name (including extention)
    - the file bytes
    - PK#7 padding bytes

.jky files are key files and contain
- 7 magic bytes JKEY128/ JKEY192/ JKEY256 to veryify the file type and key length
- the key bytes

Code implimenting the mode of operation 

### **CLI** - ./src/cli

The CLI is the user interface of the program. It is run by executing the joeyaes (or joeyaes.exe) binary it has four commands: encrypt, decrypt, genkey and help. The format of these commands and their arguments are shown by the help command.
```bash
   joeyaes help
```
```
Joey AES - AES Encryption Tool
================================

Usage: joeyaes <command> [arguments]

Commands:
  genkey <directory> <name> <algorithm>
      Generate a new AES key
      - directory:  Directory to save the key file
      - name:       Name for the key file (without extension)
      - algorithm:  Key size (128, 192, or 256)
      Example: joeyaes genkey ./keys mykey 256

  encrypt <plainfile> <cipher_dir> <cipher_name> <keyfile>
      Encrypt a file using AES
      - plainfile:   Path to the file to encrypt
      - cipher_dir:  Directory to save the encrypted file
      - cipher_name: Name for the encrypted file (without extension)
      - keyfile:     Path to the key file (.jky)
      Example: joeyaes encrypt file.txt ./encrypted secret ./keys/mykey.jky

  decrypt <cipherfile> <output_dir> <keyfile>
      Decrypt an encrypted file
      - cipherfile:  Path to the encrypted file (.jwy)
      - output_dir:  Directory to save the decrypted file
      - keyfile:     Path to the key file (.jky)
      Example: joeyaes decrypt ./encrypted/secret.jwy ./output ./keys/mykey.jky

  help
      Display this help message
   ```

## Build system

This project uses a manually coded makefile system. The core directory files are compiled into a library libcore.a which is then a dependency for the file-utilities directory which is compiled into the libutil.a library. All tests were performed using these libraies as source files. The CLI executables also depends on these libraies. Testing of the CLI was performmed manually.


### Build Instructions

This guide explains how to compile the project for both Linux and Windows binaries.

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

###  Personal AI Policy
No code in the core directory has been written by AI and AI autocomplete was dissabled, this was to make sure that everything was intentional. AI was used to create test code, by giving it specific test vectors to format into tests. The logic of this was that since this is code that since this is code that only runs once to verify everything is working, it is less critical that it is meticulously thought through or well structured. However, upon reflection it became obvious that the AI code was not up to sufficient standard, and time was spent rewriting it, finding sources for the data, and debugging it when it made false assumptions about how the program worked. 


