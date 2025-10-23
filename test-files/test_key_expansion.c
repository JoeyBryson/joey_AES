#include "aes.h"
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/aes.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "test-framework/unity.h"

// -------------------------------------------------------------
// Unity setup/teardown
// -------------------------------------------------------------
void setUp(void) {}
void tearDown(void) {}

// -------------------------------------------------------------
// === rot_word ===
// -------------------------------------------------------------
void test_rot_word(void) {
    word_t input = {{0x01, 0x02, 0x03, 0x04}};
    word_t expected = {{0x02, 0x03, 0x04, 0x01}};
    word_t result = rot_word(input);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected.b[i], result.b[i], "rot_word failed");
    }
}

// -------------------------------------------------------------
// === sub_word ===
// -------------------------------------------------------------
void test_sub_word(void) {
    word_t input = {{0x00, 0x11, 0x22, 0x33}};
    word_t expected;
    for (int i = 0; i < 4; i++) {
        expected.b[i] = Sbox[input.b[i]];
    }

    word_t result = sub_word(input);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected.b[i], result.b[i], "sub_word failed");
    }
}

// -------------------------------------------------------------
// === add_words ===
// -------------------------------------------------------------
void test_add_words(void) {
    word_t word1 = {{0xFF, 0x00, 0x55, 0xAA}};
    word_t word2 = {{0x0F, 0xF0, 0x55, 0x0A}};
    word_t expected = {{0xF0, 0xF0, 0x00, 0xA0}}; // XOR result
    word_t result = add_words(word1, word2);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected.b[i], result.b[i], "add_words failed");
    }
}

// -------------------------------------------------------------
// Comparison with OpenSSL (no printing, use Unity asserts)
// -------------------------------------------------------------
void compare_with_openssl(aes_key_t key, round_keys_t rk) {
    int Nk = key.num_key_words;
    int Nr = Nk + 6;
    int total_words = 4 * (Nr + 1);

    uint8_t flat_key[32] = {0}; // max 256-bit key
    for (int i = 0; i < Nk; i++)
        for (int j = 0; j < 4; j++)
            flat_key[i * 4 + j] = key.words[i].b[j];

    AES_KEY enc_key;
    AES_set_encrypt_key(flat_key, Nk * 32, &enc_key);

    for (int i = 0; i < total_words; i++) {
        unsigned int openssl_word = enc_key.rd_key[i];
        uint8_t bytes[4] = {
            openssl_word & 0xFF,
            (openssl_word >> 8) & 0xFF,
            (openssl_word >> 16) & 0xFF,
            (openssl_word >> 24) & 0xFF
        };

        for (int j = 0; j < 4; j++) {
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(
                bytes[j],
                rk.words[i].b[j],
                "AES key expansion mismatch with OpenSSL"
            );
        }
    }
}

// -------------------------------------------------------------
// Helper: create AES key, optionally random
// -------------------------------------------------------------
aes_key_t make_test_key(int num_key_words, int randomize) {
    aes_key_t key;
    key.num_key_words = num_key_words;
    key.words = malloc(sizeof(word_t) * key.num_key_words);

    for (int i = 0; i < num_key_words; i++) {
        for (int j = 0; j < 4; j++) {
            if (randomize)
                key.words[i].b[j] = rand() % 256;
            else
                key.words[i].b[j] = i * 4 + j;
        }
    }
    return key;
}

// -------------------------------------------------------------
// Test AES key expansion for a given Nk
// -------------------------------------------------------------
void run_expand_key_test(int num_key_words, int randomize) {
    aes_key_t key = make_test_key(num_key_words, randomize);
    round_keys_t rk = expand_key(key);
    compare_with_openssl(key, rk);
    free(key.words);
    free(rk.words);
}

// -------------------------------------------------------------
// Wrapper Unity tests for AES-128/192/256 key expansion
// -------------------------------------------------------------
void test_expand_key_128(void) {
    run_expand_key_test(4, 0); // sequential key
    run_expand_key_test(4, 1); // random key #1
    run_expand_key_test(4, 1); // random key #2
}

void test_expand_key_192(void) {
    run_expand_key_test(6, 0);
    run_expand_key_test(6, 1);
    run_expand_key_test(6, 1);
}

void test_expand_key_256(void) {
    run_expand_key_test(8, 0);
    run_expand_key_test(8, 1);
    run_expand_key_test(8, 1);
}

// -------------------------------------------------------------
// Main
// -------------------------------------------------------------
int main(void) {
    srand((unsigned int)time(NULL));  // Seed RNG for random keys
    UNITY_BEGIN();
    init_Sbox();

    RUN_TEST(test_rot_word);
    RUN_TEST(test_sub_word);
    RUN_TEST(test_add_words);

    RUN_TEST(test_expand_key_128);
    RUN_TEST(test_expand_key_192);
    RUN_TEST(test_expand_key_256);

    return UNITY_END();
}
