# JOEYAES  

Command-line file encryption tool for Linux and Windows with no third-party dependencies, built as a learning exercise

[![Release](https://img.shields.io/github/v/release/JoeyBryson/joey_AES?include_prereleases&label=release)](https://github.com/JoeyBryson/joey_AES/releases)
![License: none](https://img.shields.io/badge/license-none-lightgrey)

JOEYAES is an educational C command-line project for encrypting and decrypting individual files with a personal AES-CBC implementation and custom file formats; it is not suitable for protecting sensitive data.

The project was created to explore how AES, key handling, file I/O, and a command-line workflow fit together in a working program. The CLI provides a repeatable way to generate keys and process files while studying the implementation.

JOEYAES is not a replacement for a mature tool such as 7-Zip, which supports encrypted archives as well as compression. This implementation has not been independently reviewed, and CBC encryption alone does not authenticate data or detect tampering. Use it for learning and experimentation; for sensitive information, choose a maintained, independently reviewed security tool.

The `joeyaes` program has no third-party library dependencies: the AES implementation, CBC mode, and file formats are written from scratch using only the C standard library plus operating-system facilities (POSIX headers and `/dev/urandom` on Linux; Windows APIs, including `bcrypt` for random numbers, on Windows). Unit tests use the bundled [Unity](test-framework/unity.h) framework, and OpenSSL is used only by a developer script that generates test vectors, so neither is needed to build or run the program.

See:

- [Program Overview](PROGRAM_OVERVIEW.md) for code structure and function
- [Build](BUILD.md) for compilation and testing instructions using Linux
- [Personal Reflections](REFLECTIONS.md) for takeaways from the project

The sections below cover installation and usage.

## Installation

### Linux

Download the binary

```bash
wget https://github.com/JoeyBryson/joey_AES/releases/download/v1.0.0/joeyaes
```

make it executable

```bash
chmod +x joeyaes
```

move to usr/local/bin (for PATH access)

```bash
sudo mv joeyaes /usr/local/bin/
```

### Windows

For my windows using friends:
Go to the release page and download windows_install.cmd. Double click the file to run the installer. This will open a Command Prompt window. Once installed, open a *new* Command Prompt window:

- press Windows Key + R
- type 'cmd', press enter

continue with the usage instructions in the next section.

## Usage

Test whether correctly installed by entering

```bash
joeyaes help
```

this should display the following text which explains the four commands available:

```text
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

I recommend creating a new directory

```bash
mkdir joeyaes_files
```

then move into the directory with

```bash
cd joeyaes_files
```

make subdirectories

```bash
mkdir plain_files
mkdir ciphers
mkdir keys
mkdir decrypted
```

now select a file you want to encrypt and copy across to the joeyaes/plain_files directory. Next, we need to generate a private key. The key is saved as a .jky file.

```bash
joeyaes genkey ./keys key1 128
```

this creates a random key which will be your private key. You can use 128, 192 or 256 bits for your key.
Now encrypt your file:

```bash
joeyaes encrypt plain_files/<your file name> ./ciphers cipher1 ./keys/key1.jky
```

cipher1.jwy should now be in your cipher directory. The .jwy file stores the encrypted file data and original file name. Because this project is educational and has not been security-reviewed, do not rely on it to protect confidential files or send sensitive data. To decrypt the file, run

```bash
joeyaes decrypt ciphers/cipher1.jwy ./decrypted ./keys/key1.jky
```

Your original file should now have been decrypted into the joeyaes_files/decrypted directory.
