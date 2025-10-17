#include "aes.h"
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/aes.h>
#include <stdio.h>
#include <stddef.h>

int main() {
    unsigned char key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
    unsigned char plaintext[17] = "Hello AES World!";
    unsigned char ciphertext[16];

    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(plaintext, ciphertext, &enc_key);

    printf("plaintext: %s\n\nciphertext: ", plaintext);
    for (size_t i = 0; i < sizeof(ciphertext); ++i) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");
    return 0;
}
