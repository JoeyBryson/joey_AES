#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/aes.h>
#include <stdio.h>
#include <stdint.h>

// This program generates hardcoded test vectors for key expansion
// Compile: gcc -o gen_vectors generate_key_expansion_vectors.c -lssl -lcrypto

void generate_key_expansion(const char* name, uint8_t* key_bytes, int key_len_bytes, int Nk)
{
	AES_KEY enc_key;
	AES_set_encrypt_key(key_bytes, Nk * 32, &enc_key);
	
	int Nr = Nk + 6;
	int total_words = 4 * (Nr + 1);
	
	printf("\n// Test vector: %s (AES-%d)\n", name, Nk * 32);
	printf("uint8_t %s_key[%d] = {", name, key_len_bytes);
	for (int i = 0; i < key_len_bytes; i++) {
		if (i % 16 == 0) printf("\n\t");
		printf("0x%02X", key_bytes[i]);
		if (i < key_len_bytes - 1) printf(", ");
	}
	printf("\n};\n\n");
	
	printf("uint8_t %s_expected[%d] = {", name, total_words * 4);
	for (int i = 0; i < total_words; i++) {
		unsigned int openssl_word = enc_key.rd_key[i];
		uint8_t bytes[4] = {
			openssl_word & 0xFF,
			(openssl_word >> 8) & 0xFF,
			(openssl_word >> 16) & 0xFF,
			(openssl_word >> 24) & 0xFF
		};
		
		for (int j = 0; j < 4; j++) {
			if ((i * 4 + j) % 16 == 0) printf("\n\t");
			printf("0x%02X", bytes[j]);
			if (i < total_words - 1 || j < 3) printf(", ");
		}
	}
	printf("\n};\n");
}

int main(void)
{
	printf("// Auto-generated test vectors for AES key expansion\n");
	printf("// Generated using OpenSSL's AES_set_encrypt_key\n");
	printf("// DO NOT EDIT - regenerate if needed\n");
	
	// AES-128 test vectors
	uint8_t aes128_sequential[16] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};
	generate_key_expansion("aes128_sequential", aes128_sequential, 16, 4);
	
	uint8_t aes128_known[16] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};
	generate_key_expansion("aes128_known", aes128_known, 16, 4);
	
	// AES-192 test vectors
	uint8_t aes192_sequential[24] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
	};
	generate_key_expansion("aes192_sequential", aes192_sequential, 24, 6);
	
	// AES-256 test vectors
	uint8_t aes256_sequential[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
	};
	generate_key_expansion("aes256_sequential", aes256_sequential, 32, 8);
	
	uint8_t aes256_known[32] = {
		0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
		0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
		0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
		0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
	};
	generate_key_expansion("aes256_known", aes256_known, 32, 8);
	
	return 0;
}
