#include "aes.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "gen-test-vectors/test_vectors.h"

// test file for the functions in key_expansion.c

// Unity setup/teardown functions
void setUp(void) {}
void tearDown(void) {}

void test_rot_word(void)
{
	word_t input = {{0x01, 0x02, 0x03, 0x04}};
	word_t expected = {{0x02, 0x03, 0x04, 0x01}};
	word_t result = rot_word(input);

	for (int i = 0; i < 4; i++) {
		TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected.byte[i], result.byte[i], "rot_word failed");
	}
}

void test_add_words(void)
{
	word_t word1 = {{0xFF, 0x00, 0x55, 0xAA}};
	word_t word2 = {{0x0F, 0xF0, 0x55, 0x0A}};
	word_t expected = {{0xF0, 0xF0, 0x00, 0xA0}}; // XOR result
	word_t result = add_words(word1, word2);

	for (int i = 0; i < 4; i++) {
		TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected.byte[i], result.byte[i], "add_words failed");
	}
}

// compares my key expansion with hardcoded test vectors
void compare_with_expected(round_keys_t rk, uint8_t* expected_bytes, int expected_size)
{
	int total_words = expected_size / 4;
	
	for (int i = 0; i < total_words; i++) {
		for (int j = 0; j < 4; j++) {
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_bytes[i * 4 + j],
			                               rk.words[i].byte[j],
			                               "AES key expansion mismatch with expected");
		}
	}
}

// create key from byte array
aes_key_t make_key_from_bytes(uint8_t* key_bytes, int num_key_words)
{
	aes_key_t key;
	key.num_key_words = num_key_words;
	key.words = malloc(sizeof(word_t) * key.num_key_words);

	for (int i = 0; i < num_key_words; i++) {
		for (int j = 0; j < 4; j++) {
			key.words[i].byte[j] = key_bytes[i * 4 + j];
		}
	}
	return key;
}

// Test AES key expansion against hardcoded test vectors
void test_expand_key_128(void)
{
	// Test with sequential key
	aes_key_t key = make_key_from_bytes(aes128_sequential_key, 4);
	round_keys_t rk = expand_key(key);
	compare_with_expected(rk, aes128_sequential_expected, sizeof(aes128_sequential_expected));
	free(key.words);
	free(rk.words);

	// Test with known NIST test vector
	key = make_key_from_bytes(aes128_known_key, 4);
	rk = expand_key(key);
	compare_with_expected(rk, aes128_known_expected, sizeof(aes128_known_expected));
	free(key.words);
	free(rk.words);
}

void test_expand_key_192(void)
{
	aes_key_t key = make_key_from_bytes(aes192_sequential_key, 6);
	round_keys_t rk = expand_key(key);
	compare_with_expected(rk, aes192_sequential_expected, sizeof(aes192_sequential_expected));
	free(key.words);
	free(rk.words);
}

void test_expand_key_256(void)
{
	// Test with sequential key
	aes_key_t key = make_key_from_bytes(aes256_sequential_key, 8);
	round_keys_t rk = expand_key(key);
	compare_with_expected(rk, aes256_sequential_expected, sizeof(aes256_sequential_expected));
	free(key.words);
	free(rk.words);

	// Test with known NIST test vector
	key = make_key_from_bytes(aes256_known_key, 8);
	rk = expand_key(key);
	compare_with_expected(rk, aes256_known_expected, sizeof(aes256_known_expected));
	free(key.words);
	free(rk.words);
}

int main(void)
{
	UNITY_BEGIN();
	init_Sbox();

	RUN_TEST(test_rot_word);
	RUN_TEST(test_add_words);

	RUN_TEST(test_expand_key_128);
	RUN_TEST(test_expand_key_192);
	RUN_TEST(test_expand_key_256);

	return UNITY_END();
}
