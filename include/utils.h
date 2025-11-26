
#ifndef AES_FILE_UTIL_H
#define AES_FILE_UTIL_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

#define MAX_NAME_LEN 255


extern const char* magics[];

typedef struct {
    byte_t* bytes;
    size_t num_bytes;
} byte_arr_t;

typedef struct {
    state_t* states;
    size_t num_states;
    size_t num_padding_bytes;
} state_arr_t;

aes_key_t create_random_key(alg_t alg);
aes_key_t create_key_from_file(char* key_path);
void write_key_file(FILE* file_ptr, aes_key_t key);
FILE* build_key_file_ptr(const char* dir, const char* name);

state_t x_or_states(state_t state_a, state_t state_b);
void buffer_to_state(state_t* state, byte_t* buffer);
void state_to_buffer(state_t* state, byte_t* buffer);

/* small helpers used by tests and file utilities */
FILE* open_file(const char* path);
const char* get_file_name(const char* path);
void rewrite_plain_file_from_byte_arr(FILE* plain_file_ptr, byte_arr_t byte_arr);

/* New/Refactored APIs */
state_arr_t read_cipher_file_to_states(FILE* file_ptr, alg_t* file_alg, state_t* init_vector, char* cipher_name);
byte_arr_t plain_file_to_byte_arr(FILE* file_ptr, const char* plain_file_name);
state_arr_t byte_arr_to_state_arr(byte_arr_t byte_arr);
byte_arr_t state_arr_to_byte_arr(state_arr_t state_arr);

/* writing/reading helpers */
void write_cipher_file(FILE* file_ptr, alg_t algorithm, state_t init_vector, state_arr_t state_arr, const char* cipher_file_name);
void write_plain_states_to_file(state_arr_t state_arr, const char* path);

FILE* build_cipher_file_ptr(const char* cipher_dir, const char* cipher_name);
FILE* build_plain_file_ptr(const char* plain_dir, byte_arr_t byte_arr);

/* high-level operations */
void encrypt_file(const char* plaintext_path, const char* cipher_dir, const char* cipher_name, aes_key_t key);
state_t gen_iv(void);

#endif