# Program Overview

## The AES Algorithm

AES is the most widely used block cipher worldwide. It encrypts and decrypts individual 16-byte blocks using a specific private key. It has three versions using keys of length 128, 192, and 256 bits, with longer keys offering more theoretical security—although all are regarded as secure unless your adversaries have a quantum computer.

The source of the specifications for building this algorithm was the official [FIPS 197 document](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf) published in 2001 (and updated 2023) by the National Institute of Standards and Technology (NIST). This is an excellent resource for a project like this, as it has both detailed explanation and specifications, and examples that were used for testing. Terminology and names (e.g., words, state) have mostly been pulled directly from this document.

## Data Types: `byte_t`, `word_t`, and `state_t`

This AES implementation mirrors the structure described in FIPS-197. Each byte is stored as a `uint8_t` (`byte_t`), four of which are grouped into a `word_t`, and four words form the `state_t` struct (where words form the *columns* of the state matrix). This layout keeps the code close to the mathematical notation and makes the transformations easy to follow.

The trade-off is performance: acting on individual bytes leads to more instructions and less parallelism than word-oriented operations. Implementations like OpenSSL use 32-bit words and optimized bitwise and table-driven operations to achieve much higher throughput.

**See:** [`./include/aes.h`](./include/aes.h) | [`./src/core`](./src/core)  

## S-box Generation

The AES Substitution Box is an easily copyable 16 × 16 array of bytes. This program goes through the unnecessary effort of generating the S-box from its [construction instructions defined in FIPS 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf#page=21) each time it runs. This takes minimal computation and is performed for completeness and satisfaction.

**See:** [`./src/core/Sbox_gen.c`](./src/core/Sbox_gen.c)

## Mode of Operation

In order to create full confidentiality for a string of blocks (like a file) while still using the same key, the program must encrypt each block—even identical ones—to different cipher blocks. To achieve this, it's paired with a Mode of Operation. JOEYAES uses CBC (Cipher Block Chaining), described in the NIST [Recommendation for Block Cipher Modes of Operation](https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38a.pdf) publication.

Each block is XOR'ed with the previous cipher block—this is a way we can "inject entropy" into the algorithm. The first block is XOR'd with an initialization vector (IV) block, which must be included with the cipher blocks for decryption.

## Custom File Formats: `.jwy` and `.jky`

This project introduces two custom file extensions: **`.jwy`** for encrypted data and **`.jky`** for key storage.

### `.jwy` – Encrypted Container

A `.jwy` file packages encrypted content and includes:

- **7-byte magic header** (`JOEY128`, `JOEY192`, or `JOEY256`) identifying both format and encryption variant
- **Initialization vector** (IV)
- **Cipherblocks** which, when decrypted, contain:
  - A 1-byte field indicating the length of the original file name
  - The full file name (including its extension)
  - The raw file data
  - PKCS#7 padding to align the final block to 16 bytes

### `.jky` – Key File

A `.jky` file stores the encryption key and contains:

- **7-byte magic header** (`JKEY128`, `JKEY192`, or `JKEY256`) identifying format and key size
- **Key material** (16, 24, or 32 bytes)

### Encryption/Decryption Flow

The transformation pipeline for processing a file is:

```text
file
 ↔ byte_arr (file name + data)
 ↔ 16-byte buffers (PKCS#7 padded)
 ↔ state_arr (plaintext)
 ↔ state_arr (ciphertext)
 ↔ .jwy file
```

`byte_arr` is an array of `byte_t`, while `state_arr` is an array of `state_t`.  
The encryption and decryption routines operate directly on each `state_t` element in place.  
**See:** [`./include/utils.h`](./include/utils.h) | [`./src/file-utilities/file_encrypt.c`](./src/file-utilities/file_encrypt.c) | [`./src/file-utilities/file_decrypt.c`](./src/file-utilities/file_decrypt.c)

## Command-Line Interface (CLI)

The CLI is the user interface of the program. It is run by executing the `joeyaes` (or `joeyaes.exe`) binary and has four commands: **encrypt**, **decrypt**, **genkey**, and **help**. The first three are direct function calls to the `./file-utilities` functions: `encrypt_file`, `decrypt_file`, and `create_and_save_key`.

**See:** [`./include/cli.h`](./include/cli.h) | [`./src/cli/cli.c`](./src/cli/cli.c)  
