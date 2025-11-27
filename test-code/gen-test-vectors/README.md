# Test Vector Generator

This directory contains code to generate hardcoded test vectors for AES key expansion testing.

## Purpose

The test vectors are generated using OpenSSL's `AES_set_encrypt_key` function to create known-good reference values. These hardcoded vectors replace the runtime OpenSSL dependency in the actual test suite, enabling:

1. **Windows Compatibility**: MinGW cross-compilation doesn't have OpenSSL headers available
2. **Portability**: Tests can run without external crypto library dependencies
3. **Deterministic Testing**: Test vectors are fixed and reproducible

## Files

- `generate_key_expansion_vectors.c` - Source code that uses OpenSSL to generate test vectors
- `test_vectors.h` - Generated header file with hardcoded test vectors (included in test code)

## Test Vectors Included

The generator creates vectors for multiple AES key sizes with different test keys:

- **AES-128**:
  - Sequential key (0x00, 0x01, 0x02, ...)
  - Known NIST test vector (0x2B7E151628AED2A6ABF7158809CF4F3C)

- **AES-192**:
  - Sequential key (0x00, 0x01, 0x02, ...)

- **AES-256**:
  - Sequential key (0x00, 0x01, 0x02, ...)
  - Known NIST test vector (0x603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4)

## Regenerating Test Vectors

If you need to regenerate `test_vectors.h`:

```bash
cd test-code/gen-test-vectors
gcc -o generate_vectors generate_key_expansion_vectors.c -lssl -lcrypto
./generate_vectors > test_vectors.h
```

**Note**: OpenSSL development headers are required for regeneration (`libssl-dev` on Debian/Ubuntu).

## Usage in Tests

The `test_vectors.h` file is included in `test_key_expansion.c`:

```c
#include "gen-test-vectors/test_vectors.h"
```

The test code then validates the AES key expansion implementation against these hardcoded expected values instead of calling OpenSSL at runtime.
