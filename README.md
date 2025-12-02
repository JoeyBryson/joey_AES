# JOEYAES  

JOEYAES is is a command line based file encryption tool written in C. It uses a naive personal implimentation of the (Advanced Encryption Standard) AES algorithm running in Cipher-Block-Chaining(CBC) mode to encrypt/decrypt individual files along with their file names. It has not been sufficiently vetted for security issues to be used for any sensitive application. It has been created for personal educational purposes only. See:  

- [Program Overview](PROGRAM_OVERVIEW.md) for code structure and function
- [Build](BUILD.md) for compilation and testing instructions using Linux
- [Personal Refeclections](PERSONAL_REFLECTIONS.md) for my takeaways from this project

Continue for installation of precompiled binaries and usage:

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
Go to the realease page and download windows_install.cmd. Double click the file to run the installer. This will open a Command Prompt window. Once installed, open a *new* Command Prompt window:

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

I recomend creating a new directory

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

cipher1.jwy should now be in your cipher directory. .jwy files are the cipher files which contain the data and name of your original file but in cipher code. If you moved your key1.jky file somewhere secure, and you also deleted your original plain file, no-one would ever be able to read the file's original contents even if they could read the .jwy file on your computer and they had a copy of joeyaes. You could also send this file to someone(who you securely gave the private key) over a non-secure channel. Unless an observer had the private key, it would be completely impossible to interpret the contents of the original file. To decrypt the file with the private key, run

```bash
joeyaes decrypt ciphers/cipher1.jwy ./decrypted ./keys/key1.jky
```

Your original file should now have been decrypted into the joeyaes_files/decrypted directory.
