#include "utils.h"
#ifdef _WIN32
    #include <windows.h>
    #include <bcrypt.h>
#else
    #include <sys/stat.h>
#endif
#include <errno.h>

// Forward declarations
static void encrypt_states_CBC(state_arr_t state_arr,
                                state_t init_vector,
                                aes_key_t key,
                                progress_callback_t progress_cb,
                                void* user_data);

// magic numbers for ciper .jwy files
const char* magics[] = {"JOEY128", "JOEY192", "JOEY256"};

long find_file_size(FILE* file_ptr)
{
	fseek(file_ptr, 0, SEEK_END);
	long file_size = ftell(file_ptr);

	if (file_size < 0) {
		fprintf(stderr, "Error: File size not known\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
	return file_size;
}

FILE* open_file(const char* path)
{
	FILE* file_ptr;
	file_ptr = fopen(path, "rb");

	if (file_ptr == NULL) {
		fprintf(stderr, "Error: Opening file failed ('%s'): %s\n", path, strerror(errno));
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
	return file_ptr;
}

const char* get_file_name(const char* path)
{
#ifdef _WIN32
	const char* slash_ptr = strrchr(path, '\\');
	const char* fwd_slash = strrchr(path, '/');
	if (fwd_slash && (!slash_ptr || fwd_slash > slash_ptr)) {
		slash_ptr = fwd_slash;
	}
#else
	const char* slash_ptr = strrchr(path, '/');
#endif
	return (slash_ptr == NULL) ? path : (slash_ptr + 1);
}

// sets byte_arr to contain all the bytes to be encrypted: the header(name length & name) and file
// contents itself
byte_arr_t plain_file_to_byte_arr(FILE* file_ptr, const char* plain_file_name)
{
	long file_size = find_file_size(file_ptr);
	size_t name_len = strlen(plain_file_name);

	byte_arr_t byte_arr;
	byte_arr.num_bytes = (1 + name_len) + file_size; // 1 for the byte specifying name_len itself

	byte_arr.bytes = malloc(byte_arr.num_bytes * sizeof(byte_t));

	if (byte_arr.bytes == NULL) {
		fprintf(stderr, "Error: Memory allocation for byte_arr.bytes failed\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}

	byte_arr.bytes[0] = (byte_t)name_len;
	for (size_t i = 0; i < name_len; i++) {
		byte_arr.bytes[i + 1] = (byte_t)plain_file_name[i];
	}

	fseek(file_ptr, 0, SEEK_SET);
	/* write file contents after header byte + name */
	if (fread(byte_arr.bytes + 1 + name_len, sizeof(byte_t), file_size, file_ptr) !=
	    (size_t)file_size) {
		fprintf(stderr, "Error: failed to read file contents into byte_arr\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}

	return byte_arr;
}

// sets state_arr to contain the data in byte_arr formatted into state_t blocks with PKCS#7 padding
state_arr_t byte_arr_to_state_arr(byte_arr_t byte_arr)
{
	state_arr_t state_arr;
	state_arr.num_states = (size_t)byte_arr.num_bytes / 16 + 1;
	size_t num_padding_bytes = byte_arr.num_bytes % 16 > 0 ? 16 - byte_arr.num_bytes % 16 : 16;
	state_arr.num_padding_bytes = num_padding_bytes;

	state_arr.states = malloc(state_arr.num_states * sizeof(state_t));

	if (state_arr.states == NULL) {
		fprintf(stderr, "Error: Memory allocation for state_arr failed\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}

	int byte_idx = 0;
	byte_t buffer[16];

	for (size_t i = 0; i < state_arr.num_states - 1; i++) {
		memcpy(&buffer, &(byte_arr.bytes[byte_idx]), 16);
		byte_idx += 16;
		buffer_to_state(&(state_arr.states[i]), buffer);
	}
	/* last partial block: copy remaining bytes then PKCS#7 pad */
	size_t last_bytes = 16 - state_arr.num_padding_bytes;
	memcpy(&buffer, &(byte_arr.bytes[byte_idx]), last_bytes);
	for (size_t i = 0; i < state_arr.num_padding_bytes; i++) {
		buffer[last_bytes + i] = (byte_t)state_arr.num_padding_bytes;
	}
	buffer_to_state(&(state_arr.states[state_arr.num_states - 1]), buffer);

	return state_arr;
}

// creating a .jwy file consisting of magic numbers, cipher name, initialization vector(plain) and
// cipher-bytes respectively
void write_cipher_file(FILE* file_ptr,
                       alg_t algorithm,
                       state_t init_vector,
                       state_arr_t state_arr,
                       const char* cipher_file_name)
{
	fwrite(magics[algorithm], sizeof(byte_t), 7, file_ptr);

	int name_len = (int)strlen(cipher_file_name);
	if (name_len > 255) {
		fprintf(stderr, "Error: Name too long\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
	fwrite(&name_len, sizeof(byte_t), 1, file_ptr);
	fwrite(cipher_file_name, sizeof(byte_t), name_len, file_ptr);

	byte_t buffer[16];
	state_to_buffer(&init_vector, buffer);
	fwrite(buffer, sizeof(byte_t), 16, file_ptr);

	for (size_t i = 0; i < state_arr.num_states; i++) {
		state_to_buffer(&state_arr.states[i], buffer);
		fwrite(buffer, sizeof(byte_t), 16, file_ptr);
	}
}

state_t gen_iv(void)
{
	byte_t buffer[16];
#ifdef _WIN32
	NTSTATUS status = BCryptGenRandom(NULL, buffer, 16, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(status)) {
		fprintf(stderr, "Error: Failed to generate random bytes\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
#else
	FILE* urandom_ptr = fopen("/dev/urandom", "rb");
	if (urandom_ptr == NULL) {
		fprintf(stderr, "Error: Failed to open /dev/urandom\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
	fread(buffer, 1, 16, urandom_ptr);
	fclose(urandom_ptr);
#endif
	state_t init_vector;
	buffer_to_state(&init_vector, buffer);
	return init_vector;
}

static void encrypt_states_CBC(state_arr_t state_arr,
                                state_t init_vector,
                                aes_key_t key,
                                progress_callback_t progress_cb,
                                void* user_data)
{
	init_Sbox();
	round_keys_t round_keys = expand_key(key);
	state_t prev = init_vector;

	for (size_t i = 0; i < state_arr.num_states; i++) {
		state_t input_state = x_or_states(prev, state_arr.states[i]);
		state_t cipher_state = cipher(round_keys, input_state);
		state_arr.states[i] = cipher_state;
		prev = cipher_state;
		
		if (progress_cb) {
			progress_cb(i + 1, state_arr.num_states, user_data);
		}
	}
	free(round_keys.words);
}

FILE* build_cipher_file_ptr(const char* dir, const char* name, char* outpath)
{
#ifndef _WIN32
	struct stat st;
	if (!(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))) {
		fprintf(stderr, "Error: Invalid cipher directory \'%s\'\n", dir);
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
#else
	DWORD attrib = GetFileAttributesA(dir);
	if (attrib == INVALID_FILE_ATTRIBUTES || !(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
		fprintf(stderr, "Error: Invalid cipher directory \'%s\'\n", dir);
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
#endif

	// generate cipher file path
	size_t path_len = strlen(dir) + strlen(name) + strlen(".jwy") + 2; // 2 for '/' + '\0'
	char path[path_len];
	strcpy(path, dir);
	if (dir[strlen(dir) - 1] != '/') {
		strcat(path, "/");
	}

	strcat(path, name);
	strcat(path, ".jwy");

	// generate file for writing
	FILE* file_ptr;
	file_ptr = fopen(path, "wb");
	strcpy(outpath, path);
	if (file_ptr == NULL) {
		fprintf(stderr, "Error: Cipherfile creation failed\n");
		SUGGEST_HELP_IF_CLI();
		exit(EXIT_FAILURE);
	}
	return file_ptr;
}

void encrypt_file(const char* plain_file_path,
                  const char* cipher_file_dir,
                  const char* cipher_file_name,
                  aes_key_t key,
                  char* outpath,
                  progress_callback_t progress_cb,
                  void* user_data)
{
	state_t init_vector = gen_iv();
	FILE* plain_file_ptr = open_file(plain_file_path);
	const char* plain_file_name = get_file_name(plain_file_path);
	byte_arr_t byte_arr = plain_file_to_byte_arr(plain_file_ptr, (char*)plain_file_name);
	fclose(plain_file_ptr);
	state_arr_t state_arr = byte_arr_to_state_arr(byte_arr);
	free(byte_arr.bytes);
	encrypt_states_CBC(state_arr, init_vector, key, progress_cb, user_data);
	FILE* cipher_ptr = build_cipher_file_ptr(cipher_file_dir, cipher_file_name, outpath);
	write_cipher_file(cipher_ptr, key.alg, init_vector, state_arr, cipher_file_name);
	free(state_arr.states);
	fclose(cipher_ptr);
}
