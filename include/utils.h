#ifndef AES_FILE_UTIL_H
#define AES_FILE_UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "aes.h"

#define MAX_NAME_LEN 255

#define SUGGEST_HELP_IF_CLI() \
    do { \
        if (is_cli_mode) { \
            fprintf(stderr, "Run 'joeyaes help' for usage information.\n"); \
        } \
    } while(0)

typedef void (*progress_callback_t)(size_t current, size_t total);

extern const char* magics[];
extern bool is_cli_mode;

typedef struct {
    byte_t* bytes;
    size_t num_bytes;
} byte_arr_t;

typedef struct {
    state_t* states;
    size_t num_states;
    size_t num_padding_bytes;
} state_arr_t;



//encrypt file and sub functions

void encrypt_file(const char* plaintext_path,
                  const char* cipher_dir,
                  const char* cipher_name,
                  aes_key_t key,
                  char* outpath,
                  progress_callback_t progress_bar_cb);

state_t gen_iv(void);
FILE* open_file(const char* path);
const char* get_file_name(const char* path);
byte_arr_t plain_file_to_byte_arr(FILE* file_ptr, const char* plain_file_name);
state_arr_t byte_arr_to_state_arr(byte_arr_t byte_arr);
void encrypt_states_CBC(state_arr_t state_arr,
                        state_t init_vector,
                        aes_key_t key,
                        progress_callback_t progress_bar_cb);
FILE* build_cipher_file_ptr(const char* cipher_dir, const char* cipher_name, char* outpath);
void write_states_to_cipher_file(FILE* file_ptr,
                                 alg_t algorithm,
                                 state_t init_vector,
                                 state_arr_t state_arr,
                                 const char* cipher_file_name);



//decrypt file and sub functions

void decrypt_file(const char* cipher_path,
                  const char* plain_dir,
                  aes_key_t key,
                  char* outpath,
                  progress_callback_t progress_bar_cb);

state_arr_t read_cipher_file_to_states(FILE* file_ptr,
                                       alg_t* file_alg,
                                       state_t* init_vector,
                                       char* cipher_name);
void decrypt_states_CBC(state_arr_t state_arr,
                        state_t init_vector,
                        aes_key_t key,
                        progress_callback_t progress_bar_cb);
byte_arr_t state_arr_to_byte_arr(state_arr_t state_arr);
FILE* build_plain_file_ptr(const char* plain_dir, byte_arr_t byte_arr, char* outpath);
void rewrite_plain_file_from_byte_arr(FILE* plain_file_ptr, byte_arr_t byte_arr);

//key gen
void create_and_save_key(alg_t alg, char* dir, char* name, char* outpath);
aes_key_t create_random_key(alg_t alg);
aes_key_t create_key_from_file(char* key_path);
void write_key_file(FILE* file_ptr, aes_key_t key);
FILE* build_key_file_ptr(const char* dir, const char* name, char* outpath);

//for CBC
state_t x_or_states(state_t state_a, state_t state_b);

//used in both encrypt and decrypt
void buffer_to_state(state_t* state, byte_t* buffer);
void state_to_buffer(state_t* state, byte_t* buffer);

#endif
