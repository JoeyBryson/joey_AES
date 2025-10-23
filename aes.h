#ifndef AES_H
#define AES_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t byte_t;
typedef struct {
    byte_t b[4]; 
} word_t;

typedef struct {
    word_t w[4];
} state_t;
typedef struct 
{
    uint8_t num_key_words;
    word_t* words;
}aes_key_t;

typedef struct 
{
    uint8_t num_rounds;
    word_t* words;
}round_keys_t;

byte_t two_times(byte_t byte);
byte_t GF_mul(byte_t a, byte_t b);

extern byte_t Sbox[256];
extern bool Sbox_is_initialized;



//Sbox_gen functions
byte_t affine_transform(byte_t byte);
byte_t inverse(byte_t byte);
void init_Sbox(void);

word_t rot_word(word_t word);
round_keys_t expand_key(aes_key_t key);
word_t sub_word(word_t word);
word_t add_words(word_t word1, word_t word2);

state_t* cipher(round_keys_t round_keys, state_t* input);
void add_round_key(state_t* state, state_t* round_key);
void sub_bytes(state_t* state);
void shift_rows(state_t* state);
void mix_columns(state_t* state);


#endif
