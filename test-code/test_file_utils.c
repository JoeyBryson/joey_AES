#include "utils.h"
#include "useful_functions.h"
#include "unity.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

void setUp(void) {}
void tearDown(void) {}

bool file_exists(const char* path)
{
	FILE* f = fopen(path, "rb");
	if (f) {
		fclose(f);
		return true;
	} else {
		return false;
	}
}

int files_are_equal(const char* path1, const char* path2)
{
	FILE* f1 = fopen(path1, "rb");
	FILE* f2 = fopen(path2, "rb");
	if (!f1 || !f2)
		return 0;

	int c1, c2;
	do {
		c1 = fgetc(f1);
		c2 = fgetc(f2);
		if (c1 != c2) {
			fclose(f1);
			fclose(f2);
			return 0;
		}
	} while (c1 != EOF && c2 != EOF);

	fclose(f1);
	fclose(f2);
	return 1;
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

	char outpath[256];

	FILE* in_file_ptr = open_file(in_file_path);
	const char* in_file_name = get_file_name(in_file_path);
	byte_arr_t byte_arr = plain_file_to_byte_arr(in_file_ptr, (char*)in_file_name);
	fclose(in_file_ptr);

	FILE* out_file_ptr = build_plain_file_ptr(out_dir, byte_arr, outpath);
	rewrite_plain_file_from_byte_arr(out_file_ptr, byte_arr);

	free(byte_arr.bytes);
	fclose(out_file_ptr);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(1,
	                                files_are_equal(in_file_path, outpath),
	                                "in file and out files different");
}

void test_plain_file_to_byte_arr_to_state_arr_to_byte_arr_to_file_again()
{
	const char* in_file_path = "test-files/in-files/test_text2.txt";
	const char* out_dir = "test-files/out-files";

	char outpath[256];

	FILE* in_file_ptr = open_file(in_file_path);
	const char* in_file_name = get_file_name(in_file_path);
	byte_arr_t byte_arr = plain_file_to_byte_arr(in_file_ptr, (char*)in_file_name);

	state_arr_t state_arr = byte_arr_to_state_arr(byte_arr);
	byte_arr_t byte_arr_2 = state_arr_to_byte_arr(state_arr);
	free(state_arr.states);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(0,
	                                memcmp(byte_arr.bytes, byte_arr_2.bytes, byte_arr.num_bytes),
	                                "state to byte transition is non inverse");

	free(byte_arr.bytes);
	fclose(in_file_ptr);

	FILE* out_file_ptr = build_plain_file_ptr(out_dir, byte_arr_2, outpath);
	rewrite_plain_file_from_byte_arr(out_file_ptr, byte_arr_2);
	free(byte_arr_2.bytes);
	fclose(out_file_ptr);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(1,
	                                files_are_equal(in_file_path, outpath),
	                                "failed to copy contents correctly");
}

void test_encrypt_file_creates_a_file()
{
	char* plaintext_path = "test-files/in-files/test_text3.txt";
	char* cipher_dir = "test-files/cipher-files";
	char* cipher_name = "test_3_cipher";
	char outpath[256];

	aes_key_t aes_key;
	aes_key.num_key_words = 4;
	aes_key.alg = AES128;
	aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

	aes_key.words[0] = (word_t){{0x2B, 0x7E, 0x15, 0x16}};
	aes_key.words[1] = (word_t){{0x28, 0xAE, 0xD2, 0xA6}};
	aes_key.words[2] = (word_t){{0xAB, 0xF7, 0x15, 0x88}};
	aes_key.words[3] = (word_t){{0x09, 0xCF, 0x4F, 0x3C}};

	encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key, outpath);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(1, file_exists(outpath), "failed to generate file");

	free(aes_key.words);
}

void test_cipher_file_to_states_to_file_again()
{
	const char* in_path = "test-files/cipher-files/test_3_cipher.jwy";
	const char* cipher_dir = "test-files/cipher-files-copied";
	char outpath[256];

	FILE* fin = fopen(in_path, "rb");
	TEST_ASSERT_NOT_NULL_MESSAGE(fin, "failed to open input cipher file");

	alg_t algorithm;
	state_t init_vector;
	char cipher_name[MAX_NAME_LEN];
	state_arr_t state_arr = read_cipher_file_to_states(fin, &algorithm, &init_vector, cipher_name);
	fclose(fin);

	FILE* fout = build_cipher_file_ptr(cipher_dir, cipher_name, outpath);
	write_cipher_file(fout, algorithm, init_vector, state_arr, cipher_name);
	fclose(fout);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(1,
	                                files_are_equal(in_path, outpath),
	                                "failed to copy contents correctly");

	free(state_arr.states);
}

void test_decrypt_previously_encrypted_file()
{
	const char* plain_out_dir = "test-files/out-files";
	const char* cipher_path = "test-files/cipher-files/test_3_cipher.jwy";
	char outpath[256];

	aes_key_t aes_key;
	aes_key.num_key_words = 4;
	aes_key.alg = AES128;
	aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

	aes_key.words[0] = (word_t){{0x2B, 0x7E, 0x15, 0x16}};
	aes_key.words[1] = (word_t){{0x28, 0xAE, 0xD2, 0xA6}};
	aes_key.words[2] = (word_t){{0xAB, 0xF7, 0x15, 0x88}};
	aes_key.words[3] = (word_t){{0x09, 0xCF, 0x4F, 0x3C}};

	decrypt_file(cipher_path, (char*)plain_out_dir, aes_key, outpath);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(1,
	                                files_are_equal("test-files/out-files/test_text3.txt",
	                                                "test-files/in-files/test_text3.txt"),
	                                "decrypted file does not match original");

	free(aes_key.words);
}

void test_encrypt_decrypt_large_jpeg()
{
	const char* plaintext_path = "test-files/in-files/wales.jpg";
	const char* cipher_dir = "test-files/cipher-files";
	const char* cipher_name = "big1";
	const char* plain_out_dir = "test-files/out-files";

	char cipher_out[256];
	char plain_out[256];

	aes_key_t aes_key;
	aes_key.num_key_words = 4;
	aes_key.alg = AES128;
	aes_key.words = malloc(sizeof(word_t) * aes_key.num_key_words);

	aes_key.words[0] = (word_t){{0x2B, 0x7E, 0x15, 0x16}};
	aes_key.words[1] = (word_t){{0x28, 0xAE, 0xD2, 0xA6}};
	aes_key.words[2] = (word_t){{0xAB, 0xF7, 0x15, 0x88}};
	aes_key.words[3] = (word_t){{0x09, 0xCF, 0x4F, 0x3C}};

	clock_t start = clock();

	encrypt_file(plaintext_path, cipher_dir, cipher_name, aes_key, cipher_out);
	decrypt_file(cipher_out, plain_out_dir, aes_key, plain_out);

	TEST_ASSERT_EQUAL_INT32_MESSAGE(
	    1,
	    files_are_equal("test-files/in-files/wales.jpg", "test-files/out-files/wales.jpg"),
	    "decrypted file does not match original");

	clock_t end = clock();
	printf("JPEG test time: %.3f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

	free(aes_key.words);
}

int main()
{
	RUN_TEST(test_plain_file_to_byte_arr_to_file_again);
	RUN_TEST(test_plain_file_to_byte_arr_to_state_arr_to_byte_arr_to_file_again);
	RUN_TEST(test_encrypt_file_creates_a_file);
	RUN_TEST(test_cipher_file_to_states_to_file_again);
	RUN_TEST(test_decrypt_previously_encrypted_file);
	RUN_TEST(test_encrypt_decrypt_large_jpeg);
	return UNITY_END();
}
