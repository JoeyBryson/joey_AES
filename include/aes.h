#ifndef AES_H
#define AES_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
typedef enum { AES128, AES192, AES256 } alg_t;

typedef uint8_t byte_t;
typedef struct {
	byte_t byte[4];
} word_t;

typedef struct {
	word_t col[4]; // column
} state_t;
typedef struct {
	uint8_t num_key_words;
	word_t* words;
	alg_t alg;
} aes_key_t;

typedef struct {
	uint8_t num_rounds;
	word_t* words;
	alg_t alg;
} round_keys_t;

byte_t two_times(byte_t byte);
byte_t GF_mul(byte_t a, byte_t b);

extern byte_t Sbox_arr[256]; // one mapping for each of the 256 possible byte values
extern byte_t inv_Sbox_arr[256];
extern bool Sbox_is_initialized;

// Sbox_gen functions
byte_t affine_transform(byte_t byte);
byte_t inverse(byte_t byte);
void init_Sbox(void);
byte_t Sbox(byte_t byte);
byte_t inv_Sbox(byte_t byte);

//key expansion
word_t rot_word(word_t word);
round_keys_t expand_key(aes_key_t key);
word_t sub_word(word_t word);
word_t add_words(word_t word1, word_t word2);

//cipher and subfunctions
state_t cipher(round_keys_t round_keys, state_t input);
void add_round_key(state_t* state, word_t* key_ptr);
void sub_bytes(state_t* state);
void shift_rows(state_t* state);
word_t mul_matrix(word_t word, word_t mat_row);
void mix_columns(state_t* state);

//inverse cipher and sub functions
state_t inv_cipher(round_keys_t round_keys, state_t input);
void inv_sub_bytes(state_t* state);
void inv_shift_rows(state_t* state);
word_t inv_mul_matrix(word_t word, word_t mat_row);
void inv_mix_columns(state_t* state);

#endif
