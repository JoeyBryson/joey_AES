#include "aes.h"
#include "utils.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void print_key(const aes_key_t key)
{
	printf("Key (%d words):\n", key.num_key_words);
	for (int i = 0; i < key.num_key_words; i++) {
		printf("word %d: ", i);
		for (int b = 0; b < 4; b++) {
			printf("%02X", key.words[i].byte[b]);
		}
		printf("\n");
	}
}

void test_key_gen_basic128(void)
{
	aes_key_t key = create_random_key(AES128);

	TEST_ASSERT_EQUAL_UINT8(4, key.num_key_words);

	print_key(key);
	free(key.words);
}

void test_key_gen_basic192(void)
{
	aes_key_t key = create_random_key(AES192);

	TEST_ASSERT_EQUAL_UINT8(6, key.num_key_words);

	print_key(key);
	free(key.words);
}

void test_key_gen_basic256(void)
{
	aes_key_t key = create_random_key(AES256);

	TEST_ASSERT_EQUAL_UINT8(8, key.num_key_words);

	print_key(key);
	free(key.words);
}

void test_write_and_read_key_file128(void)
{
	char* key_dir = "test-files/key-files";
	char* key_name = "key1";
	char outpath[256];

	aes_key_t key = create_random_key(AES128);
	FILE* key_ptr = build_key_file_ptr(key_dir, key_name, outpath);
	write_key_file(key_ptr, key);
	fclose(key_ptr);
	aes_key_t key2 = create_key_from_file("test-files/key-files/key1.jky");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(
	    memcmp(key.words, key2.words, key.num_key_words * sizeof(word_t)),
	    0,
	    "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.num_key_words,
	                                key2.num_key_words,
	                                "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.alg, key2.alg, "Key read/write unsuccessful");
	free(key.words);
	free(key2.words);
}

void test_write_and_read_key_file192(void)
{
	char* key_dir = "test-files/key-files";
	char* key_name = "key2";
	char outpath[256];

	aes_key_t key = create_random_key(AES192);
	FILE* key_ptr = build_key_file_ptr(key_dir, key_name, outpath);
	write_key_file(key_ptr, key);
	fclose(key_ptr);
	aes_key_t key2 = create_key_from_file("test-files/key-files/key2.jky");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(
	    memcmp(key.words, key2.words, key.num_key_words * sizeof(word_t)),
	    0,
	    "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.num_key_words,
	                                key2.num_key_words,
	                                "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.alg, key2.alg, "Key read/write unsuccessful");
	free(key.words);
	free(key2.words);
}
void test_write_and_read_key_file256(void)
{
	char* key_dir = "test-files/key-files";
	char* key_name = "key3";
	char outpath[256];

	aes_key_t key = create_random_key(AES256);
	FILE* key_ptr = build_key_file_ptr(key_dir, key_name, outpath);
	write_key_file(key_ptr, key);
	fclose(key_ptr);
	system("ls -l test-files/key-files/key3.jky");
	aes_key_t key2 = create_key_from_file("test-files/key-files/key3.jky");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(
	    memcmp(key.words, key2.words, key.num_key_words * sizeof(word_t)),
	    0,
	    "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.num_key_words,
	                                key2.num_key_words,
	                                "Key read/write unsuccessful");
	TEST_ASSERT_EQUAL_INT32_MESSAGE(key.alg, key2.alg, "Key read/write unsuccessful");
	free(key.words);
	free(key2.words);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_key_gen_basic128);
	RUN_TEST(test_key_gen_basic192);
	RUN_TEST(test_key_gen_basic256);
	RUN_TEST(test_write_and_read_key_file128);
	RUN_TEST(test_write_and_read_key_file192);
	RUN_TEST(test_write_and_read_key_file256);
	return UNITY_END();
}