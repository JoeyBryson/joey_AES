#include "utils.h"
#include <unistd.h>
#include <sys/stat.h>
#define MAX_NAME_LEN 255

state_arr_t read_cipher_file_to_states(FILE* file_ptr, alg_t* file_alg, state_t* init_vector, char* cipher_name)
{
    if(file_ptr == NULL) {
        fprintf(stderr, "Error: Cipher file not accessible\n");
        exit(EXIT_FAILURE);
    }

    if(fseek(file_ptr, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: fseek failed\n");
        exit(EXIT_FAILURE);
    }
    long file_size = ftell(file_ptr);

    if(file_size < 0) {
        fprintf(stderr, "Error: Cipher file size not known\n");
        exit(EXIT_FAILURE);
    }

    state_arr_t state_arr;

    // read header fields to determine counts: magic (7), name_len (1), name (name_len), IV (16), then ciphertext blocks 
    fseek(file_ptr, 0, SEEK_SET);
    char magic[8];
    fread(magic, sizeof(byte_t), 7, file_ptr);
    magic[7] = '\0';
    if(strcmp(magic, magics[AES128]) == 0) {
        *file_alg = AES128;
    } else if (strcmp(magic, magics[AES192]) == 0) {
        *file_alg = AES192;
    } else if (strcmp(magic, magics[AES256]) == 0) {
        *file_alg = AES256; 
    } else {
        fprintf(stderr, "Error: Cipher file magic invalid\n");
        exit(EXIT_FAILURE);
    }

    // read cipher name length and name
    byte_t name_len;
    fread(&name_len, sizeof(byte_t), 1, file_ptr);
    fread(cipher_name, sizeof(byte_t), name_len, file_ptr);
    cipher_name[name_len] = '\0';

    //read remaining bytes: IV + ciphertext blocks 
    long pos_after_header = ftell(file_ptr);
    long remaining = file_size - pos_after_header;
    if (remaining < 16 || (remaining % 16) != 0) {
        fprintf(stderr, "Error: Cipher file size invalid\n");
        exit(EXIT_FAILURE);
    }

    size_t total_blocks = (size_t) remaining / 16; 
    state_arr.num_states = total_blocks - 1; 

    state_arr.states = malloc(state_arr.num_states * sizeof(state_t));

    if(state_arr.states == NULL) {
        fprintf(stderr, "Error: Memory allocation for state_arr failed\n");
        exit(EXIT_FAILURE);
    }

    byte_t buffer[16];
    fread(buffer, sizeof(byte_t), 16, file_ptr);
    buffer_to_state(init_vector, buffer);

    for(size_t i = 0; i < state_arr.num_states; i++) {
        fread(buffer, sizeof(byte_t), 16, file_ptr);
        buffer_to_state(&(state_arr.states[i]), buffer);
    }
    return state_arr;
}

void write_plain_states_to_file(state_arr_t state_arr, const char* path)
{
    FILE* file_ptr;
    file_ptr = fopen(path, "wb");

    if(file_ptr == NULL) {
        fprintf(stderr, "Error: File creation failed\n");
        exit(EXIT_FAILURE);
    }

    byte_t buffer[16];
    for (size_t i = 0; i < state_arr.num_states - 1; i++) {
        state_to_buffer(&state_arr.states[i], buffer);
        fwrite(buffer, sizeof(byte_t), 16, file_ptr);
    }

    state_to_buffer(&state_arr.states[state_arr.num_states - 1], buffer);
    size_t remainder = buffer[15];
    fwrite(buffer, sizeof(byte_t), 16 - remainder, file_ptr);
    fclose(file_ptr);  
}

byte_arr_t state_arr_to_byte_arr(state_arr_t state_arr)
{
    byte_arr_t byte_arr;
    /* Debug: print state_arr values used to compute byte array size */
    size_t num_bytes = 16*state_arr.num_states - state_arr.num_padding_bytes;
    byte_arr.bytes = malloc(num_bytes*sizeof(byte_t));

    byte_arr.num_bytes = num_bytes;

        if(byte_arr.bytes == NULL) {
            fprintf(stderr, "Error: Memory allocation for byte_arr failed\n");
            exit(EXIT_FAILURE);
        }

    int byte_idx = 0;
    byte_t buffer[16];
    for(size_t i = 0; i < state_arr.num_states - 1; i++) {
        state_to_buffer(&state_arr.states[i], buffer);
        memcpy(&byte_arr.bytes[byte_idx], buffer, 16);
        byte_idx += 16;
    }

    /* last state: copy only (16 - padding) bytes */
    state_to_buffer(&state_arr.states[state_arr.num_states - 1], buffer);
    size_t last_bytes = 16 - state_arr.num_padding_bytes;
    memcpy(&byte_arr.bytes[byte_idx], buffer, last_bytes);
    
    /* DEBUG: print reconstructed byte_arr header and lengths */
        /* silent in production: removed debug printing */
    return byte_arr;
}

FILE* build_plain_file_ptr(const char* dir, byte_arr_t byte_arr)
{
    struct stat st;
    if(!(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))) {
        fprintf(stderr, "Error: Invalid plainfile directory \'%s\'\n", dir);
        exit(EXIT_FAILURE);
    }

    size_t name_len = byte_arr.bytes[0];
    char name[name_len+1];
    for(size_t i = 0; i < name_len; i++) {
        name[i] = (char) byte_arr.bytes[i+1];
    }
    name[name_len] = '\0';


    size_t path_len = strlen(dir) + name_len + 2; // 2 for '/' + '\0'
    char path[path_len];
    strcpy(path, dir);
    if (dir[strlen(dir) - 1] != '/') {
        strcat(path, "/");
    }

    strcat(path, name);

    //generate file for writing
    FILE* file_ptr;
    file_ptr = fopen(path, "wb");

    if(file_ptr == NULL) {
        fprintf(stderr, "Error: Plainfile recreation failed (tried path: %s)\n", path);
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return file_ptr;
}


void rewrite_plain_file_from_byte_arr(FILE* plain_file_ptr, byte_arr_t byte_arr)
{
    size_t name_len = byte_arr.bytes[0];
    byte_t* content = &byte_arr.bytes[1 + name_len];
    size_t content_len = byte_arr.num_bytes - 1 - name_len;
    if (fwrite(content, sizeof(byte_t), content_len, plain_file_ptr) != content_len) {
        fprintf(stderr, "Error: failed to write plain file contents\n");
        exit(EXIT_FAILURE);
    }
}

void decrypt_states_CBC(state_arr_t state_arr, state_t init_vector, aes_key_t key)
{
    init_Sbox();
    round_keys_t round_keys = expand_key(key);
    state_t prev = init_vector;

    for(size_t i = 0; i < state_arr.num_states; i++) {
        state_t decrypt_state = inv_cipher(round_keys, state_arr.states[i]);
        state_t plain_state = x_or_states(prev, decrypt_state);
        prev = state_arr.states[i];
        state_arr.states[i] = plain_state;
    }
    free(round_keys.words);
}

void decrypt_file(const char* cipher_path, const char* plain_dir, aes_key_t key) 
{
    FILE* cipher_file_ptr = fopen(cipher_path, "rb");
    alg_t file_alg;
    state_t init_vec;
    char cipher_file_name[MAX_NAME_LEN];
    state_arr_t state_arr = read_cipher_file_to_states(cipher_file_ptr, &file_alg, &init_vec, cipher_file_name);
    fclose(cipher_file_ptr);
    if(file_alg != key.alg) {
        fprintf(stderr, "Error: Key and file different algorithm types\n key type %s and file type %s", magics[key.alg], magics[file_alg]);
        exit(EXIT_FAILURE);
    }
    decrypt_states_CBC(state_arr, init_vec, key);
    /* now that states are decrypted, read padding byte from last state */
    state_arr.num_padding_bytes = state_arr.states[state_arr.num_states - 1].col[3].byte[3];
    if (state_arr.num_padding_bytes == 0 || state_arr.num_padding_bytes > 16) {
        fprintf(stderr, "Error: invalid padding byte (%zu) after decryption\n", state_arr.num_padding_bytes);
        exit(EXIT_FAILURE);
    }
    byte_arr_t byte_arr = state_arr_to_byte_arr(state_arr);
    free(state_arr.states);
    FILE* plain_file_ptr = build_plain_file_ptr(plain_dir, byte_arr);
    rewrite_plain_file_from_byte_arr(plain_file_ptr, byte_arr);
    free(byte_arr.bytes);
    fclose(plain_file_ptr);
}
