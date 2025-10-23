#include "aes.h"
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/aes.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// -------------------------------------------------------------
// Helper functions for printing
// -------------------------------------------------------------
void printBinary(uint8_t n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

void print_word(word_t w) {
    for (int i = 0; i < 4; i++) {
        printf("%02X ", w.b[i]);
    }
}

// Helper to print all round keys
void print_round_keys(round_keys_t rk, int num_words_per_round) {
    for (int i = 0; i < (4 * (rk.num_rounds + 1)); i++) {
        printf("W[%2d]: ", i);
        print_word(rk.words[i]);
        printf("\n");
        if ((i + 1) % num_words_per_round == 0) printf("\n");
    }
}

// -------------------------------------------------------------
// Comparison with OpenSSL
// -------------------------------------------------------------
void compare_with_openssl(aes_key_t key, round_keys_t rk) {
    int Nk = key.num_key_words;
    int Nr = Nk + 6;
    int total_words = 4*(Nr+1);

    // Flatten key into uint8_t array for OpenSSL
    uint8_t flat_key[32] = {0}; // max 256-bit key
    for (int i = 0; i < Nk; i++)
        for (int j = 0; j < 4; j++)
            flat_key[i*4 + j] = key.words[i].b[j];

    AES_KEY enc_key;
    AES_set_encrypt_key(flat_key, Nk*32, &enc_key); // key size in bits

    printf("Comparing with OpenSSL:\n");
    int pass = 1;

    for (int i = 0; i < total_words; i++) {
        unsigned int openssl_word = enc_key.rd_key[i];
        uint8_t bytes[4] = {
        openssl_word & 0xFF,
        (openssl_word >> 8) & 0xFF,
        (openssl_word >> 16) & 0xFF,
        (openssl_word >> 24) & 0xFF
        };


        for (int j = 0; j < 4; j++) {
            if (rk.words[i].b[j] != bytes[j]) {
                pass = 0;
                printf("Mismatch at W[%d][%d]: mine=%02X, OpenSSL=%02X\n",
                       i, j, rk.words[i].b[j], bytes[j]);
            }
        }
    }

    if (pass) printf("All words match OpenSSL! ✅\n\n");
    else       printf("Some words do not match OpenSSL. ❌\n\n");
}

// -------------------------------------------------------------
// Test AES key expansion
// -------------------------------------------------------------
void test_expand_key_generic(int num_key_words) {
    printf("Testing expand_key (%d-bit key)...\n", num_key_words*32);

    aes_key_t key;
    key.num_key_words = num_key_words;
    key.words = malloc(sizeof(word_t) * key.num_key_words);

    // Simple sequential key: 0x00..0x?? 
    for (int i = 0; i < key.num_key_words; i++)
        for (int j = 0; j < 4; j++)
            key.words[i].b[j] = i*4 + j;

    round_keys_t rk = expand_key(key);

    print_round_keys(rk, key.num_key_words);
    compare_with_openssl(key, rk);

    free(key.words);
    free(rk.words);
}

// -------------------------------------------------------------
// Optional: Test GF_mul, inverse, affine transform, S-box
// -------------------------------------------------------------
void test_GF_mul() {
    printf("Testing GF_mul:\n");
    printf("0x57 * 0x83 = 0xC1 ? Result: 0x%02X\n", GF_mul(0x57, 0x83));
    printf("0x01 * 0x01 = 0x01 ? Result: 0x%02X\n", GF_mul(0x01, 0x01));
    printf("0xFF * 0x02 = 0xE5 ? Result: 0x%02X\n", GF_mul(0xFF, 0x02));
    printf("\n");
}

void test_inverse() {
    printf("Testing inverse:\n");
    for (int i = 1; i <= 16; i++) {
        byte_t inv = inverse((byte_t)i);
        byte_t check = GF_mul((byte_t)i, inv);
        printf("%02X^-1 = %02X, check: %02X\n", i, inv, check);
    }
    printf("\n");
}

void test_affine_transform() {
    printf("Testing affine_transform:\n");
    byte_t inputs[] = {0x01};
    for (int i = 0; i < sizeof(inputs); i++) {
        byte_t out = affine_transform(inputs[i]);
        printf("affine_transform(%02X) = %02X\n", inputs[i], out);
    }
    printf("\n");
}

void test_Sbox() {
    printf("Generating S-box...\n");
    init_Sbox();
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            int idx = i*16 + j;
            printf("%02X ", Sbox[idx]);
        }
        printf("\n");
    }
    printf("\n");
}

// -------------------------------------------------------------
// Main
// -------------------------------------------------------------
int main() {
    // Optional tests
    test_GF_mul();
    test_inverse();
    test_affine_transform();
    test_Sbox();

    // Key expansion tests
    test_expand_key_generic(4); // AES-128
    test_expand_key_generic(6); // AES-192
    test_expand_key_generic(8); // AES-256

    return 0;
}
