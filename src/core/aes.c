#include "aes.h"
#include <stdio.h>
#include <stdlib.h>

byte_t Sbox(byte_t byte)
{
	if (!Sbox_is_initialized) {
		fprintf(stderr, "Error: Sbox not initialized\n");
		exit(EXIT_FAILURE);
	}
	return Sbox_arr[byte];
}

byte_t inv_Sbox(byte_t byte)
{
	if (!Sbox_is_initialized) {
		fprintf(stderr, "Error: Sbox not initialized\n");
		exit(EXIT_FAILURE);
	}
	return inv_Sbox_arr[byte];
}

byte_t two_times(byte_t byte)
{
	if (byte & 0x80) {
		byte <<= 1;
		byte ^= 0x1B;
	} else {
		byte <<= 1;
	}
	return byte;
}

byte_t GF_mul(byte_t byte1, byte_t byte2)
{
	byte_t result = 0;
	byte_t x_pow_two = byte2;
	for (int i = 0; i < 8; i++) {
		if (byte1 & 1) {
			result ^= x_pow_two;
		}
		x_pow_two = two_times(x_pow_two);
		byte1 >>= 1;
	}
	return result;
}

word_t add_words(word_t word1, word_t word2)
{
	word_t output;
	for (int i = 0; i < 4; i++) {
		output.byte[i] = word1.byte[i] ^ word2.byte[i];
	}
	return output;
}

// key_ptr points to the memory location of the first of the four words making up the round key
void add_round_key(state_t* state, word_t* key_ptr)
{
	for (int c = 0; c < 4; c++) {
		state->col[c] = add_words(state->col[c], *key_ptr);
		key_ptr++;
	}
}

void sub_bytes(state_t* state)
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			state->col[c].byte[r] = Sbox(state->col[c].byte[r]);
		}
	}
}

void shift_rows(state_t* state)
{
	state_t temp;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			temp.col[c].byte[r] = state->col[(4 + c + r) % 4].byte[r];
		}
	}
	*state = temp;
}

// matrix multiplication by a matrix with the form decribed in section 4.3
word_t mul_matrix(word_t word, word_t mat_col)
{
	word_t output = {{0x00, 0x00, 0x00, 0x00}};
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			output.byte[r] ^= GF_mul(word.byte[c], mat_col.byte[(r - c + 4) % 4]);
		}
	}
	return output;
}

void mix_columns(state_t* state)
{
	word_t mixing_word = (word_t){{0x02, 0x01, 0x01, 0x03}};
	for (int c = 0; c < 4; c++) {
		state->col[c] = mul_matrix(state->col[c], mixing_word);
	}
}

void inv_sub_bytes(state_t* state)
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			state->col[c].byte[r] = inv_Sbox(state->col[c].byte[r]);
		}
	}
}

void inv_shift_rows(state_t* state)
{
	state_t temp;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			temp.col[c].byte[r] = state->col[(4 + c - r) % 4].byte[r];
		}
	}
	*state = temp;
}

void inv_mix_columns(state_t* state)
{
	word_t mixing_word = (word_t){{0x0E, 0x09, 0x0D, 0x0B}};
	for (int c = 0; c < 4; c++) {
		state->col[c] = mul_matrix(state->col[c], mixing_word);
	}
}

state_t cipher(round_keys_t round_keys, state_t plain_text)
{

	state_t state = plain_text;
	int num_rounds = round_keys.num_rounds;

	add_round_key(&state, &(round_keys.words[0]));

	for (int round = 1; round < num_rounds; round++) {
		sub_bytes(&state);
		shift_rows(&state);
		mix_columns(&state);
		add_round_key(&state, &(round_keys.words[4 * round]));
	}

	sub_bytes(&state);
	shift_rows(&state);
	add_round_key(&state, &(round_keys.words[4 * num_rounds]));

	return state;
}

state_t inv_cipher(round_keys_t round_keys, state_t plain_text)
{
	state_t state = plain_text;
	int num_rounds = round_keys.num_rounds;

	add_round_key(&state, &(round_keys.words[4 * num_rounds]));

	for (int round = num_rounds - 1; round > 0; round--) {
		inv_shift_rows(&state);
		inv_sub_bytes(&state);
		add_round_key(&state, &(round_keys.words[4 * round]));
		inv_mix_columns(&state);
	}

	inv_shift_rows(&state);
	inv_sub_bytes(&state);
	add_round_key(&state, &(round_keys.words[0]));

	return state;
}
