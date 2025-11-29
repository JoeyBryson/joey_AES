#include "cli.h"
#include "utils.h"
#include "aes.h"

static const char* alg_str[] = {"128", "192", "256"};

#define PROGRESS_BAR_WIDTH 50

void print_progress_bar(size_t current, size_t total, void* user_data)
{
	(void)user_data; // unused
	
	if (total == 0) return;
	
	int percentage = (int)((current * 100) / total);
	int filled = (int)((current * PROGRESS_BAR_WIDTH) / total);
	
	fprintf(stderr, "\r[");
	for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
		if (i < filled) {
			fprintf(stderr, "=");
		} else if (i == filled && current < total) {
			fprintf(stderr, ">");
		} else {
			fprintf(stderr, " ");
		}
	}
	fprintf(stderr, "] %3d%% (%zu/%zu blocks)", percentage, current, total);
	fflush(stderr);
	
	if (current == total) {
		fprintf(stderr, "\n");
	}
}

void print_help(void)
{
	printf("Joey AES - AES Encryption Tool\n");
	printf("================================\n\n");
	printf("Usage: joeyaes <command> [arguments]\n\n");
	printf("Commands:\n");
	printf("  genkey <directory> <name> <algorithm>\n");
	printf("      Generate a new AES key\n");
	printf("      - directory:  Directory to save the key file\n");
	printf("      - name:       Name for the key file (without extension)\n");
	printf("      - algorithm:  Key size (128, 192, or 256)\n");
	printf("      Example: joeyaes genkey ./keys mykey 256\n\n");
	
	printf("  encrypt <plainfile> <cipher_dir> <cipher_name> <keyfile>\n");
	printf("      Encrypt a file using AES\n");
	printf("      - plainfile:   Path to the file to encrypt\n");
	printf("      - cipher_dir:  Directory to save the encrypted file\n");
	printf("      - cipher_name: Name for the encrypted file (without extension)\n");
	printf("      - keyfile:     Path to the key file (.jky)\n");
	printf("      Example: joeyaes encrypt file.txt ./encrypted secret ./keys/mykey.jky\n\n");
	
	printf("  decrypt <cipherfile> <output_dir> <keyfile>\n");
	printf("      Decrypt an encrypted file\n");
	printf("      - cipherfile:  Path to the encrypted file (.jwy)\n");
	printf("      - output_dir:  Directory to save the decrypted file\n");
	printf("      - keyfile:     Path to the key file (.jky)\n");
	printf("      Example: joeyaes decrypt ./encrypted/secret.jwy ./output ./keys/mykey.jky\n\n");
	
	printf("  help\n");
	printf("      Display this help message\n\n");
}

int cmd_encrypt(int argc, char** argv)
{
	if (argc != 4) {
		fprintf(stderr, "Error: Invalid number of arguments for 'encrypt'\n");
		fprintf(stderr, "Usage: encrypt <plainfile> <cipher_dir> <cipher_name> <keyfile>\n");
		fprintf(stderr, "Run 'help' for more information.\n");
		exit(EXIT_FAILURE);
	}

	char* plain_path = argv[0];
	char* cipher_dir = argv[1];
	char* cipher_name = argv[2];
	char* key_path = argv[3];
	aes_key_t key = create_key_from_file(key_path);
	char outpath[MAX_NAME_LEN];
	
	fprintf(stderr, "Encrypting: %s\n", plain_path);
	encrypt_file(plain_path, cipher_dir, cipher_name, key, outpath, 
	             print_progress_bar, NULL);
	free(key.words);
	printf("✓ Encrypted file created: %s\n", outpath);
	return 0;
}

int cmd_decrypt(int argc, char** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid number of arguments for 'decrypt'\n");
		fprintf(stderr, "Usage: decrypt <cipherfile> <output_dir> <keyfile>\n");
		fprintf(stderr, "Run 'help' for more information.\n");
		exit(EXIT_FAILURE);
	}

	char* cipher_path = argv[0];
	char* plain_dir = argv[1];
	char* key_path = argv[2];
	aes_key_t key = create_key_from_file(key_path);
	char outpath[MAX_NAME_LEN];
	
	fprintf(stderr, "Decrypting: %s\n", cipher_path);
	decrypt_file(cipher_path, plain_dir, key, outpath, 
	             print_progress_bar, NULL);
	free(key.words);
	printf("✓ Decrypted file created: %s\n", outpath);
	return 0;
}
int cmd_genkey(int argc, char** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid number of arguments for 'genkey'\n");
		fprintf(stderr, "Usage: genkey <directory> <name> <algorithm>\n");
		fprintf(stderr, "Run 'help' for more information.\n");
		exit(EXIT_FAILURE);
	}

	char* dir = argv[0];
	char* name = argv[1];
	char* alg_name = argv[2];

	alg_t alg;
	if (strcmp(alg_name, alg_str[0]) == 0) {
		alg = AES128;
	} else if (strcmp(alg_name, alg_str[1]) == 0) {
		alg = AES192;
	} else if (strcmp(alg_name, alg_str[2]) == 0) {
		alg = AES256;
	} else {
		fprintf(stderr, "Error: Invalid argument 'algorithm', must be '128', '192' or '256'\n");
		fprintf(stderr, "Run 'joeyaes help' for usage information.\n");
		exit(EXIT_FAILURE);
	}

	
	char outpath[MAX_NAME_LEN];
	create_and_save_key(alg, dir, name, outpath);
	printf("✓ AES-%s key created: %s\n", alg_str[alg], outpath);
	return 0;

}int main(int argc, char** argv)
{
	is_cli_mode = true;
	
	if (argc < 2) {
		fprintf(stderr, "Error: Too few arguments\n");
		fprintf(stderr, "Run 'joeyaes help' for usage information.\n");
		exit(EXIT_FAILURE);
	}
	
	const char* cmd = argv[1];

	if (strcmp(cmd, "encrypt") == 0) {
		return cmd_encrypt(argc - 2, argv + 2);
	} else if (strcmp(cmd, "decrypt") == 0) {
		return cmd_decrypt(argc - 2, argv + 2);
	} else if (strcmp(cmd, "genkey") == 0) {
		return cmd_genkey(argc - 2, argv + 2);
	} else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "--help") == 0 || strcmp(cmd, "-h") == 0) {
		print_help();
		return 0;
	} else {
		fprintf(stderr, "Error: Unknown command '%s'\n", cmd);
		fprintf(stderr, "Run 'joeyaes help' for usage information.\n");
		exit(EXIT_FAILURE);
	}
}
