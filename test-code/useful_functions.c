#include "aes.h"
#include "useful_functions.h"
#include "unity.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void print_round_keys(const round_keys_t* round_keys)
{
	if (!round_keys || !round_keys->words) {
		printf("Invalid round_keys pointer.\n");
		return;
	}

	printf("=== AES Round Keys ===\n");
	for (int round = 0; round <= round_keys->num_rounds; ++round) {
		printf("Round %2d:\n", round);
		for (int w = 0; w < 4; ++w) {
			int idx = 4 * round + w;
			printf("  Word %d: ", w);
			for (int b = 0; b < 4; ++b) {
				printf("%02X ", round_keys->words[idx].byte[b]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

bool states_equal(const state_t* a, const state_t* b)
{
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			if (a->col[c].byte[r] != b->col[c].byte[r])
				return false;
		}
	}
	return true;
}

void print_word(const word_t* w)
{
	for (int i = 0; i < 4; i++) {
		printf("%02X ", w->byte[i]);
	}
}

void print_aes_key(const aes_key_t* key)
{
	if (key == NULL) {
		printf("AES Key: (null)\n");
		return;
	}

	printf("AES Key:\n");
	printf("  Number of key words: %u\n", key->num_key_words);

	if (key->words == NULL) {
		printf("  Words: (null)\n");
		return;
	}

	for (uint8_t i = 0; i < key->num_key_words; i++) {
		printf("  Word[%u]: ", i);
		print_word(&key->words[i]);
		printf("\n");
	}
}