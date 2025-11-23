#ifndef AES_TEST_FUNCTIONS
#define AES_TEST_FUNCTIONS
#include "aes.h"

void print_round_keys(const round_keys_t* round_keys);
bool states_equal(const state_t *a, const state_t *b);
void print_word(const word_t* w);
void print_aes_key(const aes_key_t* key);

#endif