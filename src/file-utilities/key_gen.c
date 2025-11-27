#include "utils.h"
#include "aes.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int key_lengths[] = {4, 6, 8};
static const char* key_magics[] = {"JKEY128", "JKEY192", "JKEY256"};

// tested inside test_key_expansion.c
aes_key_t create_random_key(alg_t alg)
{
	printf("D");
	aes_key_t key;
	key.num_key_words = key_lengths[alg];
	key.words = malloc(key.num_key_words * sizeof(word_t));
	if (!key.words) {
		fprintf(stderr, "Error: Out of memory while allocating key\n");
		exit(EXIT_FAILURE);
	}

	FILE* urandom_ptr = fopen("/dev/urandom", "rb");

	fread(key.words, sizeof(word_t), key.num_key_words, urandom_ptr);

	key.alg = alg;

	fclose(urandom_ptr);
	return key;
}

aes_key_t create_key_from_file(char* key_path)
{
	FILE* file_ptr = fopen(key_path, "rb");
	aes_key_t key;

	if (file_ptr == NULL) {
		fprintf(stderr, "Error: Key file not accessible\n");
		exit(EXIT_FAILURE);
	}

	char magic[8];
	if (fread(magic, 1, 7, file_ptr) != 7) {
		fprintf(stderr, "Error: Unexpected EOF in key file\n");
		exit(EXIT_FAILURE);
	}
	magic[7] = '\0';

	if (strcmp(magic, key_magics[AES128]) == 0) {
		key.alg = AES128;
	} else if (strcmp(magic, key_magics[AES192]) == 0) {
		key.alg = AES192;
	} else if (strcmp(magic, key_magics[AES256]) == 0) {
		key.alg = AES256;
	} else {
		fprintf(stderr, "Error: Key file magic invalid\n");
		exit(EXIT_FAILURE);
	}

	fread(&(key.num_key_words), sizeof(uint8_t), 1, file_ptr);

	if (key.num_key_words == 0 || key.num_key_words > 8) {
		fprintf(stderr, "Error: Invalid key size in file\n");
		exit(EXIT_FAILURE);
	}

	key.words = malloc(key.num_key_words * sizeof(word_t));
	if (!key.words) {
		fprintf(stderr, "Error: Out of memory while allocating key\n");
		exit(EXIT_FAILURE);
	}

	fread(key.words, key.num_key_words * sizeof(word_t), 1, file_ptr);
	return key;
}

void write_key_file(FILE* file_ptr, aes_key_t key)
{
	fwrite(key_magics[key.alg], sizeof(byte_t), 7, file_ptr);
	fwrite(&(key.num_key_words), sizeof(uint8_t), 1, file_ptr);
	fwrite(key.words, key.num_key_words * sizeof(word_t), 1, file_ptr);
}

FILE* build_key_file_ptr(const char* dir, const char* name, char* outpath)
{
	struct stat st;
	if (!(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))) {
		fprintf(stderr, "Error: Invalid key directory \'%s\'\n", dir);
		exit(EXIT_FAILURE);
	}

	size_t path_len = strlen(dir) + strlen(name) + strlen(".jky") + 2; // 2 for '/' + '\0'
	char path[path_len];
	strcpy(path, dir);
	if (dir[strlen(dir) - 1] != '/') {
		strcat(path, "/");
	}

	strcat(path, name);
	strcat(path, ".jky"); // joey-key

	// generate file for writing
	FILE* file_ptr;
	file_ptr = fopen(path, "wb");
	strcpy(outpath, path);

	if (file_ptr == NULL) {
		fprintf(stderr, "Error: key file creation failed\n");
		exit(EXIT_FAILURE);
	}

	return file_ptr;
}

void create_and_save_key(alg_t alg, char* dir, char* name, char* outpath)
{
	aes_key_t key = create_random_key(alg);
	FILE* file_ptr = build_key_file_ptr(dir, name, outpath);
	write_key_file(file_ptr, key);
	fclose(file_ptr);
	free(key.words);
}
