## Program Overview
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

### **CLI** - ./src/cli

The CLI is the user interface of the program. It is run by executing the joeyaes (or joeyaes.exe) binary it has four commands: encrypt, decrypt, genkey and help