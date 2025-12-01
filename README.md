# JOEYAES
JOEYAES is is a command line based file encryption application written in C. It uses a naive personal implimentation of the (Advanced Encryption Standard) AES algorithm running in Cipher-Block-Chaining(CBC) mode to encrypt/decrypt individual files along with their file names. It has not been sufficiently vetted for security issues to be used for any sensitive application. It has been created for personal educational purposes only.
# Installation
Download the latest release for your operating system(joeyaes for Linux and joeyaes.exe for windows) and put the executable on your `PATH`:

## Linux

```bash
wget https://github.com/JoeyBryson/joey_AES/releases/download/v1.0.0/joeyeas
```
```bash
chmod +x joeyeas
```
```bash
sudo mv joeyeas /usr/local/bin/
```

## Windows


Download the executable
```cmd
curl -L -o joeyaes.exe https://github.com/JoeyBryson/joey_AES/releases/download/v1.0.0/joeyaes.exe
```
Create a permanent location (if it doesn’t exist) and move the file

```cmd
mkdir C:\Tools
move joeyaes.exe C:\Tools\
```
:: 3. Rename it to remove the .exe
```cmd
ren C:\Tools\joeyaes.exe joeyaes
```
:: 4. Add C:\Tools to the system PATH
```cmd
setx PATH "%PATH%;C:\Tools"
```

:: 5. Open a new terminal after this step, then verify:
```cmd
joeyaes --help
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



 


