#include "utils.h"
#include "useful_functions.h"
#include "unity.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void setUp(void) {}
void tearDown(void) {}

bool file_exists(const char* path)
{
    FILE *f = fopen(path, "rb");
    if (f) {
        fclose(f);
        return true;
    } else {
        return false;
}

}
int files_are_equal(const char* path1, const char* path2)
{
    FILE *f1 = fopen(path1, "rb");
    FILE *f2 = fopen(path2, "rb");
    if (!f1 || !f2) return 0;

    int c1, c2;
    do {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            return 0;  // mismatch
        }
    } while (c1 != EOF && c2 != EOF);

    fclose(f1);
    fclose(f2);
    return 1;  // identical
}

void print_states(state_t* states, int num_states)
{
    for (int n = 0; n < num_states; n++) {
        printf("State %d:\n", n);
        for (int j = 0; j < 4; j++) {          
            for (int i = 0; i < 4; i++) {       
                byte_t b = states[n].col[i].byte[j];
                printf("%02x(%c) ", b, (b >= 32 && b <= 126) ? b : '.');
            }
            printf("\n");
        }
        printf("\n");
    }
}

void test_plain_file_to_byte_arr_to_file_again()
{
    const char* in_file_path = "test-files/in-files/test_text1.txt";
    const char* out_dir = "test-files/out-files";

    FILE* in_file_ptr = open_file(in_file_path);
    const char* in_file_name = get_file_name(in_file_path);
    byte_arr_t byte_arr = plain_file_to_byte_arr(in_file_ptr, (char*)in_file_name);
    fclose(in_file_ptr);
    FILE* out_file_ptr = build_plain_file_ptr(out_dir, byte_arr);
    rewrite_plain_file_from_byte_arr(out_file_ptr, byte_arr);
    free(byte_arr.bytes);
    fclose(out_file_ptr);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal(in_file_path, "test-files/out-files/test_text1.txt"),  "in file and out files different");
}

void test_plain_file_to_byte_arr_to_state_arr_to_byte_arr_to_file_again()
{
    const char* in_file_path = "test-files/in-files/test_text2.txt";
    const char* out_dir = "test-files/out-files";

    FILE* in_file_ptr = open_file(in_file_path);
    const char* in_file_name = get_file_name(in_file_path);
    byte_arr_t byte_arr = plain_file_to_byte_arr(in_file_ptr, (char*)in_file_name);
    state_arr_t state_arr = byte_arr_to_state_arr(byte_arr);
    byte_arr_t byte_arr_2 = state_arr_to_byte_arr(state_arr);    
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, memcmp(byte_arr.bytes, byte_arr_2.bytes, byte_arr.num_bytes),  "state to byte transition is non inverse");
    free(byte_arr.bytes);
    fclose(in_file_ptr);
    FILE* out_file_ptr = build_plain_file_ptr(out_dir, byte_arr_2);
    rewrite_plain_file_from_byte_arr(out_file_ptr, byte_arr_2);
    free(byte_arr_2.bytes);
    fclose(out_file_ptr);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal(in_file_path, "test-files/out-files/test_text2.txt"),  "failed to copy contents correctly");
}

void test_encrypt_file_creates_a_file()
{
    char* plaintext_path = "test-files/in-files/test_text3.txt";
    char* cipher_dir = "test-files/cipher-files";
    char* cipher_name = "test_3_cipher";
    aes_key_t aes_key;
    aes_key.num_key_words = 4; 
    aes_key.alg = AES128;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}};
    encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key); 
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, file_exists("test-files/cipher-files/test_3_cipher.joey"),  "failed to generate file");
    free(aes_key.words);
}

void test_cipher_file_to_states_to_file_again()
{
    const char* in_path = "test-files/cipher-files/test_3_cipher.joey";
    const char* cipher_dir = "test-files/cipher-files-copied";

    FILE* fin = fopen(in_path, "rb");
    TEST_ASSERT_NOT_NULL_MESSAGE(fin, "failed to open input cipher file");

    alg_t algorithm;
    state_t init_vector;
    char* cipher_name[MAX_NAME_LEN];
    state_arr_t state_arr = read_cipher_file_to_states(fin, &algorithm, &init_vector, cipher_name);
    fclose(fin);

    /* write using the same embedded name so the bytes match exactly */
    FILE* fout = build_cipher_file_ptr(cipher_dir, cipher_name);
    write_cipher_file(fout, algorithm, init_vector, state_arr, cipher_name);
    fclose(fout);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal("test-files/cipher-files/test_3_cipher.joey", "test-files/cipher-files-copied/test_3_cipher.joey"),  "failed to copy contents correctly");
    free(state_arr.states);
}

void test_decrypt_previously_encrypted_file()
{
    const char* plain_out_dir = "test-files/out-files";
    const char* cipher_path = "test-files/cipher-files/test_3_cipher.joey";
    aes_key_t aes_key;
    aes_key.num_key_words = 4; 
    aes_key.alg = AES128;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}}; //same key as test 2
    decrypt_file(cipher_path, (char*)plain_out_dir, aes_key);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal("test-files/out-files/test_text3.txt", "test-files/in-files/test_text3.txt"), "decrypted file does not match original");
    free(aes_key.words);
}

void test_encrypt_decrypt_large_jpeg()
{
    const char* plaintext_path = "test-files/in-files/wales.jpg";
    const char* cipher_dir = "test-files/cipher-files";
    const char* cipher_name = "big1";
    const char* plain_out_dir = "test-files/out-files";
    const char* expected_cipher_path = "test-files/cipher-files/big1.joey";
    aes_key_t aes_key;
    aes_key.num_key_words = 4;
    aes_key.alg = AES128;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);
    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}};

    clock_t _start = clock();
    encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key);
    decrypt_file(expected_cipher_path, plain_out_dir, aes_key);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal("test-files/in-files/wales.jpg", "test-files/out-files/wales.jpg"), "decrypted file does not match original");
    clock_t _end = clock();
    double _elapsed = (double)(_end - _start) / (double)CLOCKS_PER_SEC;
    struct stat _st_orig, _st_dec;
    if (stat(plaintext_path, &_st_orig) == 0) printf("wales.jpg original file size: %lld bytes\n", (long long)_st_orig.st_size);
    if (stat("test-files/out-files/wales.jpg", &_st_dec) == 0) printf("wales.jpg decrypted file size: %lld bytes\n", (long long)_st_dec.st_size);
    printf("test_encrypt_decrypt_large_jpeg elapsed time: %.3f seconds\n", _elapsed);
}

void test_encrypt_decrypt_large_png()
{
    TEST_IGNORE();
    const char* plaintext_path = "test-files/in-files/attractor.png";
    const char* cipher_dir = "test-files/cipher-files";
    const char* cipher_name = "big2";
    const char* plain_out_dir = "test-files/out-files";
    const char* expected_cipher_path = "test-files/cipher-files/big2.joey";
    aes_key_t aes_key;
    aes_key.num_key_words = 4;
    aes_key.alg = AES128;
    aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);
    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}};

    clock_t _start2 = clock();
    encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key);
    decrypt_file(expected_cipher_path, plain_out_dir, aes_key);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal("test/test-files/in-files/attractor.png", "test/test-files/out-files/attractor.png"), "decrypted file does not match original");
    clock_t _end2 = clock();
    double _elapsed2 = (double)(_end2 - _start2) / (double)CLOCKS_PER_SEC;
    struct stat _st_orig2, _st_dec2;
    if (stat(plaintext_path, &_st_orig2) == 0) printf("attractor.png original file size: %lld bytes\n", (long long)_st_orig2.st_size);
    if (stat("test/test-files/out-files/attractor.png", &_st_dec2) == 0) printf("attractor.png decrypted file size: %lld bytes\n", (long long)_st_dec2.st_size);
    printf("test_encrypt_decrypt_large_png elapsed time: %.3f seconds\n", _elapsed2);
}

void test_encrypt_decrypt_large_png_with_stack_key()
{
    TEST_IGNORE();
    const char* plaintext_path = "test-files/in-files/attractor.png";
    const char* cipher_dir = "test-files/cipher-files";
    const char* cipher_name = "big2";
    const char* plain_out_dir = "test-files/out-files";
    const char* expected_cipher_path = "test-files/cipher-files/big2.joey";
    aes_key_t aes_key;
    aes_key.num_key_words = 4;
    aes_key.alg = AES128;
    word_t words[aes_key.num_key_words];
    aes_key.words = words;
    aes_key.words[0] = (word_t){{0x2B,0x7E,0x15,0x16}};
    aes_key.words[1] = (word_t){{0x28,0xAE,0xD2,0xA6}};
    aes_key.words[2] = (word_t){{0xAB,0xF7,0x15,0x88}};
    aes_key.words[3] = (word_t){{0x09,0xCF,0x4F,0x3C}};

    clock_t _start2 = clock();
    encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key);
    decrypt_file(expected_cipher_path, plain_out_dir, aes_key);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, files_are_equal("test-files/in-files/attractor.png", "test-files/out-files/attractor.png"), "decrypted file does not match original");
    clock_t _end2 = clock();
    double _elapsed2 = (double)(_end2 - _start2) / (double)CLOCKS_PER_SEC;
    struct stat _st_orig2, _st_dec2;
    if (stat(plaintext_path, &_st_orig2) == 0) printf("attractor.png original file size: %lld bytes\n", (long long)_st_orig2.st_size);
    if (stat("test-files/out-files/attractor.png", &_st_dec2) == 0) printf("attractor.png decrypted file size: %lld bytes\n", (long long)_st_dec2.st_size);
    printf("test_encrypt_decrypt_large_png elapsed time: %.3f seconds\n", _elapsed2);
}





int main() {
    RUN_TEST(test_plain_file_to_byte_arr_to_file_again);
    RUN_TEST(test_plain_file_to_byte_arr_to_state_arr_to_byte_arr_to_file_again);
    RUN_TEST(test_encrypt_file_creates_a_file);
    RUN_TEST(test_cipher_file_to_states_to_file_again);
    RUN_TEST(test_decrypt_previously_encrypted_file);
    RUN_TEST(test_encrypt_decrypt_large_jpeg);
    RUN_TEST(test_encrypt_decrypt_large_png);
    RUN_TEST(test_encrypt_decrypt_large_png_with_stack_key);
    return UNITY_END();
}

