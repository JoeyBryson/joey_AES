#include "core/aes.h"
#include "useful_functions.h"
#include "test-framework/unity.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

//test vector from Appendix B from the FIPS 197 document(link in ReadMe)
void test_AES128_cipher_should_match_known_vector(void) {
    
    aes_key_t aes_key;
    aes_key.num_key_words = 4; 
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}};

    state_t plaintext = {
        .col[0] = (word_t){{0x32, 0x43, 0xF6, 0xA8}},
        .col[1] = (word_t){{0x88, 0x5A, 0x30, 0x8D}},
        .col[2] = (word_t){{0x31, 0x31, 0x98, 0xA2}},
        .col[3] = (word_t){{0xE0, 0x37, 0x07, 0x34}}
    };


    state_t expected = {
        .col[0] = (word_t){{0x39, 0x25, 0x84, 0x1D}},
        .col[1] = (word_t){{0x02, 0xDC, 0x09, 0xFB}},
        .col[2] = (word_t){{0xDC, 0x11, 0x85, 0x97}},
        .col[3] = (word_t){{0x19, 0x6A, 0x0B, 0x32}}
    };


    round_keys_t round_keys = expand_key(aes_key);

    state_t result = cipher(round_keys, plaintext);

    // // Assert equality
    TEST_ASSERT_TRUE_MESSAGE(states_equal(&result, &expected),
         "AES-128 encryption output does not match expected ciphertext");

    free(aes_key.words);
    free(round_keys.words);
}

//test vector taken from first line of plaintext on page 12 of the NIST Block Cipher Modes of Operation Electronic Codebook (ECB)
//https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_Core_All.pdf
void test_AES192_cipher_should_match_known_vector(void) {
    aes_key_t aes_key;
    aes_key.num_key_words = 6;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

    aes_key.words[0] = (word_t){{0x8E,0x73,0xB0,0xF7}};
    aes_key.words[1] = (word_t){{0xDA,0x0E,0x64,0x52}};
    aes_key.words[2] = (word_t){{0xC8,0x10,0xF3,0x2B}};
    aes_key.words[3] = (word_t){{0x80,0x90,0x79,0xE5}};
    aes_key.words[4] = (word_t){{0x62,0xF8,0xEA,0xD2}};
    aes_key.words[5] = (word_t){{0x52,0x2C,0x6B,0x7B}};

    state_t plaintext = {
        .col[0] = (word_t){{0x6B,0xC1,0xBE,0xE2}},
        .col[1] = (word_t){{0x2E,0x40,0x9F,0x96}},
        .col[2] = (word_t){{0xE9,0x3D,0x7E,0x11}},
        .col[3] = (word_t){{0x73,0x93,0x17,0x2A}}
    };

    state_t expected = {
        .col[0] = (word_t){{0xBD,0x33,0x4F,0x1D}},
        .col[1] = (word_t){{0x6E,0x45,0xF2,0x5F}},
        .col[2] = (word_t){{0xF7,0x12,0xA2,0x14}},
        .col[3] = (word_t){{0x57,0x1F,0xA5,0xCC}}
    };

    round_keys_t round_keys = expand_key(aes_key);
    state_t result = cipher(round_keys, plaintext);

    TEST_ASSERT_TRUE_MESSAGE(states_equal(&result, &expected),
        "AES-192 encryption output does not match expected ciphertext");

    free(aes_key.words);
    free(round_keys.words);
}

//test vector taken from first line of plaintext on page 25 of the NIST Block Cipher Modes of Operation Electronic Codebook (ECB)
//https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_Core_All.pdf
void test_AES256_cipher_should_match_known_vector(void) {
    aes_key_t aes_key;
    aes_key.num_key_words = 8;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

    aes_key.words[0] = (word_t){{0x60,0x3D,0xEB,0x10}};
    aes_key.words[1] = (word_t){{0x15,0xCA,0x71,0xBE}};
    aes_key.words[2] = (word_t){{0x2B,0x73,0xAE,0xF0}};
    aes_key.words[3] = (word_t){{0x85,0x7D,0x77,0x81}};
    aes_key.words[4] = (word_t){{0x1F,0x35,0x2C,0x07}};
    aes_key.words[5] = (word_t){{0x3B,0x61,0x08,0xD7}};
    aes_key.words[6] = (word_t){{0x2D,0x98,0x10,0xA3}};
    aes_key.words[7] = (word_t){{0x09,0x14,0xDF,0xF4}};

    state_t plaintext = {
        .col[0] = (word_t){{0x6B,0xC1,0xBE,0xE2}},
        .col[1] = (word_t){{0x2E,0x40,0x9F,0x96}},
        .col[2] = (word_t){{0xE9,0x3D,0x7E,0x11}},
        .col[3] = (word_t){{0x73,0x93,0x17,0x2A}}
    };

    state_t expected = {
        .col[0] = (word_t){{0xF3,0xEE,0xD1,0xBD}},
        .col[1] = (word_t){{0xB5,0xD2,0xA0,0x3C}},
        .col[2] = (word_t){{0x06,0x4B,0x5A,0x7E}},
        .col[3] = (word_t){{0x3D,0xB1,0x81,0xF8}}
    };

    round_keys_t round_keys = expand_key(aes_key);
    state_t result = cipher(round_keys, plaintext);

    TEST_ASSERT_TRUE_MESSAGE(states_equal(&result, &expected),
        "AES-256 encryption output does not match expected ciphertext");

    free(aes_key.words);
    free(round_keys.words);
}

int main() {
    init_Sbox();
    RUN_TEST(test_AES128_cipher_should_match_known_vector);
    RUN_TEST(test_AES192_cipher_should_match_known_vector);
    RUN_TEST(test_AES256_cipher_should_match_known_vector);
    return UNITY_END();
}
