#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//utility for printing jwy files for verification

static void print_hex_ascii_line(const unsigned char* buf, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		printf("%02X ", buf[i]);
	}
	for (size_t i = len; i < 16; ++i)
		printf("   ");
	printf("  ");
	for (size_t i = 0; i < len; ++i) {
		unsigned char c = buf[i];
		putchar((c >= 32 && c <= 126) ? c : '.');
	}
	putchar('\n');
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s file.jwy\n", argv[0]);
		return 2;
	}
	const char* path = argv[1];
	FILE* f = fopen(path, "rb");
	if (!f) {
		perror("fopen");
		return 1;
	}

	unsigned char magic[8] = {0};
	if (fread(magic, 1, 7, f) != 7) {
		fprintf(stderr, "Failed to read magic\n");
		fclose(f);
		return 1;
	}
	magic[7] = '\0';
	printf("Magic: %s\n", magic);

	unsigned char iv[16];
	size_t r = fread(iv, 1, 16, f);
	if (r != 16) {
		fprintf(stderr, "IV read short: %zu bytes\n", r);
	}
	printf("\nIV (hex / ASCII):\n");
	print_hex_ascii_line(iv, r);

	printf("\nCiphertext blocks:\n");
	unsigned char buf[16];
	size_t block = 0;
	while ((r = fread(buf, 1, 16, f)) > 0) {
		printf("Block %zu: ", block++);
		print_hex_ascii_line(buf, r);
	}

	fclose(f);
	return 0;
}